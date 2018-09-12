#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change to your UID
WIDTH = 128 # Columns
HEIGHT = 64 # Rows

import sys
from PIL import Image
from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_oled_128x64_v2 import BrickletOLED128x64V2

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    oled = BrickletOLED128x64V2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Clear display
    oled.clear_display()

    # Convert image to black/white pixels
    image = Image.open(sys.argv[1])
    image_data = image.load()
    pixels = []

    for row in range(HEIGHT):
        for column in range(WIDTH):
            if column < image.size[0] and row < image.size[1]:
                pixel = image_data[column, row] > 0
            else:
                pixel = False

            pixels.append(pixel)

    oled.write_pixels(0, 0, WIDTH-1, HEIGHT-1, pixels)

    raw_input('Press key to exit\n') # Use input() in Python 3
    ipcon.disconnect()
