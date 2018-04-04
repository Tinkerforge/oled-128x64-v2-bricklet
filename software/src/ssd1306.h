/* oled-128x64-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * ssd1306.h: SSD1306 driver
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

#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>
#include <stdbool.h>

#define OLED_MAX_ROWS    8
#define OLED_MAX_COLUMNS 128

#define SSD1306_SPI_BUFFER_SIZE 256

typedef struct {
	uint8_t spi_data[SSD1306_SPI_BUFFER_SIZE];
	uint16_t spi_data_length;

    bool initialize;
	bool reset;
	bool new_invert;
	bool new_contrast;

    uint8_t display[OLED_MAX_ROWS][OLED_MAX_COLUMNS];
	uint8_t display_mask[OLED_MAX_ROWS][OLED_MAX_COLUMNS];

	uint8_t display_write[OLED_MAX_ROWS][OLED_MAX_COLUMNS];
	uint8_t display_mask_write[OLED_MAX_ROWS][OLED_MAX_COLUMNS];

	bool display_mask_changed;

	uint8_t display_configuration_contrast;
	bool display_configuration_invert;

	bool automatic_draw;

	uint16_t read_chunk_offset;
} SSD1306;

extern SSD1306 ssd1306;

void ssd1306_init(void);
void ssd1306_tick(void);


#define PIXEL_W 128
#define PIXEL_H 64
#define PIXEL_COL_START 0

#define CONTROL_CO (1 << 7)
#define CONTROL_DC (1 << 6)

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A


#endif