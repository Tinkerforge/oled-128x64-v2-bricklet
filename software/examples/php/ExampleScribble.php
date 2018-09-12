<?php

if (!extension_loaded('gd')) {
    echo "Required gd extension is not available\n";
    exit;
}

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletOLED128x64V2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletOLED128x64V2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change to your UID
const WIDTH = 128; // Columns
const HEIGHT = 64; // Rows

function drawImage($oled, $startColumn, $startRow, $columnCount, $rowCount, $image)
{
    $pixels = array();

    // Convert image pixels into 8bit pages
    for ($row = 0; $row < $rowCount; $row++)
    {
        for ($column = 0; $column < $columnCount; $column++)
        {
            $index = imagecolorat($image, $column, $row);
			$color = imagecolorsforindex($image, $index);
	        $pixels[$row*$columnCount + $column] = ($color['red'] + $color['green'] + $color['blue'] > 0);
        }
    }
        
	$oled->writePixels(0, 0, WIDTH-1, HEIGHT-1, $pixels);
}

$ipcon = new IPConnection(); // Create IP connection
$oled = new BrickletOLED128x64V2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Clear display
$oled->clearDisplay();

// Draw rotating line
$image = imagecreate(WIDTH, HEIGHT);
$black = imagecolorallocate($image, 0, 0, 0);
$white = imagecolorallocate($image, 255, 255, 255);
$originX = WIDTH / 2;
$originY = HEIGHT / 2;
$length = HEIGHT / 2 - 2;
$angle = 0;

echo "Press ctrl+c to exit\n";

while (true)
{
    $radians = M_PI * $angle / 180.0;
    $x = (int)($originX + $length * cos($radians));
    $y = (int)($originY + $length * sin($radians));

    imagefilledrectangle($image, 0, 0, WIDTH, HEIGHT, $black);
    imageline($image, $originX, $originY, $x, $y, $white);

    drawImage($oled, 0, 0, WIDTH, HEIGHT, $image);
    usleep(25*1000);

    $angle++;
}

?>
