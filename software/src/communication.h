/* oled-128x64-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/bootloader/bootloader.h"

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants
#define OLED_128X64_V2_BOOTLOADER_MODE_BOOTLOADER 0
#define OLED_128X64_V2_BOOTLOADER_MODE_FIRMWARE 1
#define OLED_128X64_V2_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define OLED_128X64_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define OLED_128X64_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define OLED_128X64_V2_BOOTLOADER_STATUS_OK 0
#define OLED_128X64_V2_BOOTLOADER_STATUS_INVALID_MODE 1
#define OLED_128X64_V2_BOOTLOADER_STATUS_NO_CHANGE 2
#define OLED_128X64_V2_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define OLED_128X64_V2_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define OLED_128X64_V2_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define OLED_128X64_V2_STATUS_LED_CONFIG_OFF 0
#define OLED_128X64_V2_STATUS_LED_CONFIG_ON 1
#define OLED_128X64_V2_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define OLED_128X64_V2_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_WRITE_PIXELS_LOW_LEVEL 1
#define FID_CLEAR_DISPLAY 2
#define FID_SET_DISPLAY_CONFIGURATION 3
#define FID_GET_DISPLAY_CONFIGURATION 4
#define FID_WRITE_LINE 5
#define FID_DRAW_BUFFERED_FRAME 6


typedef struct {
	TFPMessageHeader header;
	uint8_t x_start;
	uint8_t y_start;
	uint8_t x_end;
	uint8_t y_end;
	uint16_t pixels_length;
	uint16_t pixels_chunk_offset;
	bool pixels_chunk_data[448];
} __attribute__((__packed__)) WritePixelsLowLevel;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) ClearDisplay;

typedef struct {
	TFPMessageHeader header;
	uint8_t contrast;
	uint8_t backlight;
	bool invert;
	bool automatic_draw;
} __attribute__((__packed__)) SetDisplayConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetDisplayConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint8_t contrast;
	uint8_t backlight;
	bool invert;
	bool automatic_draw;
} __attribute__((__packed__)) GetDisplayConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t line;
	uint8_t position;
	char text[22];
} __attribute__((__packed__)) WriteLine;

typedef struct {
	TFPMessageHeader header;
	bool force_complete_redraw;
} __attribute__((__packed__)) DrawBufferedFrame;


// Function prototypes
BootloaderHandleMessageResponse write_pixels_low_level(const WritePixelsLowLevel *data);
BootloaderHandleMessageResponse clear_display(const ClearDisplay *data);
BootloaderHandleMessageResponse set_display_configuration(const SetDisplayConfiguration *data);
BootloaderHandleMessageResponse get_display_configuration(const GetDisplayConfiguration *data, GetDisplayConfiguration_Response *response);
BootloaderHandleMessageResponse write_line(const WriteLine *data);
BootloaderHandleMessageResponse draw_buffered_frame(const DrawBufferedFrame *data);

// Callbacks


#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 0
#define COMMUNICATION_CALLBACK_LIST_INIT \


#endif
