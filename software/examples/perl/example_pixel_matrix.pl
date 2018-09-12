#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletOLED128x64V2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your OLED 128x64 Bricklet 2.0
use constant WIDTH => 128;
use constant HEIGHT => 64;

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $oled = Tinkerforge::BrickletOLED128x64V2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Clear display
$oled->clear_display();

# Draw checkerboard pattern
my @pixels = ();

foreach my $row (0..&HEIGHT - 1) {
    foreach my $column (0..&WIDTH - 1) {
        $pixels[$row*&WIDTH + $column] = (($row / 8) % 2) == (($column / 8) % 2);
    }
}

$oled->write_pixels(0, 0, &WIDTH-1, &HEIGHT-1, \@pixels);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
