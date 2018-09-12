function octave_example_pixel_matrix()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your OLED 128x64 Bricklet 2.0
    SCREEN_WIDTH = 128;
    SCREEN_HEIGHT = 64;

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    oled = javaObject("com.tinkerforge.BrickletOLED128x64V2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Clear display
    oled.clearDisplay();

    % Draw checkerboard pattern
    for h = 0:SCREEN_HEIGHT-1
        for w = 0:SCREEN_WIDTH-1
            pixels(h*SCREEN_WIDTH + w + 1) = mod(floor(h / 8), 2) == mod(floor(w / 8), 2);
        end
    end

	oled.writePixels(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, pixels);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end
