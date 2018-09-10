#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your OLED 128x64 Bricklet 2.0

# Clear display
tinkerforge call oled-128x64-v2-bricklet $uid clear-display

# Write "Hello World" starting from upper left corner of the screen
tinkerforge call oled-128x64-v2-bricklet $uid write-line 0 0 "Hello World"
