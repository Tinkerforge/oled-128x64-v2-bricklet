/* oled-128x64-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
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

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/utility/util_definitions.h"
#include "bricklib2/protocols/tfp/tfp.h"

#include "ssd1306.h"
#include "font.inc"

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_WRITE_PIXELS_LOW_LEVEL: return write_pixels_low_level(message);
		case FID_READ_PIXELS_LOW_LEVEL: return read_pixels_low_level(message, response);
		case FID_CLEAR_DISPLAY: return clear_display(message);
		case FID_SET_DISPLAY_CONFIGURATION: return set_display_configuration(message);
		case FID_GET_DISPLAY_CONFIGURATION: return get_display_configuration(message, response);
		case FID_WRITE_LINE: return write_line(message);
		case FID_DRAW_BUFFERED_FRAME: return draw_buffered_frame(message);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}


BootloaderHandleMessageResponse write_pixels_low_level(const WritePixelsLowLevel *data) {
	if((data->x_start > data->x_end) || (data->y_start > data->y_end)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if((data->x_end >= OLED_MAX_COLUMNS) || (data->y_end >= (OLED_MAX_ROWS*8))) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	const uint16_t length = MIN(data->pixels_length - data->pixels_chunk_offset, 448);
	const uint8_t columns = data->x_end - data->x_start + 1;

	uint8_t column = data->x_start + (data->pixels_chunk_offset % columns);
	uint8_t row = data->y_start + (data->pixels_chunk_offset / columns);

	uint16_t counter = 0;
	for(; row <= data->y_end; row++) {
		const uint8_t row_index = row / 8;
		const uint8_t display_bit = row % 8;
		for(; column <= data->x_end; column++) {
			const uint8_t data_index = counter / 8;
			const uint8_t data_bit = counter % 8;
			if(data->pixels_chunk_data[data_index] & (1 << data_bit)) {
				if(!(ssd1306.display[row_index][column] & (1 << display_bit))) {
					ssd1306.display[row_index][column] |= (1 << display_bit);
					ssd1306.display_mask[row_index][column] |= (1 << display_bit);
				}
			} else {
				if((ssd1306.display[row_index][column] & (1 << display_bit))) {
					ssd1306.display[row_index][column] &= ~(1 << display_bit);
					ssd1306.display_mask[row_index][column] |= (1 << display_bit);
				}
			}

			counter++;
			if(counter == length) {
				break;
			}
		}

		if(counter == length) {
			break;
		}
		column = data->x_start;
	}

	if(data->pixels_chunk_offset + length >= data->pixels_length) {
		if(ssd1306.automatic_draw) {
			ssd1306.display_mask_changed = true;
		}
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse read_pixels_low_level(const ReadPixelsLowLevel *data, ReadPixelsLowLevel_Response *response) {
	if((data->x_start > data->x_end) || (data->y_start > data->y_end)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if((data->x_end >= OLED_MAX_COLUMNS) || (data->y_end >= (OLED_MAX_ROWS*8))) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	memset(response->pixels_chunk_data, 0, 60);

	response->header.length = sizeof(ReadPixelsLowLevel_Response);
	response->pixels_length = (data->x_end - data->x_start + 1)*(data->y_end - data->y_start + 1);
	response->pixels_chunk_offset = ssd1306.read_chunk_offset;

	const uint8_t columns = data->x_end - data->x_start + 1;

	uint8_t column = data->x_start + (response->pixels_chunk_offset % columns);
	uint8_t row = data->y_start + (response->pixels_chunk_offset / columns);

	uint16_t counter = 0;
	for(; row <= data->y_end; row++) {
		const uint16_t row_index = row / 8;
		const uint16_t row_bit   = row % 8;
		for(; column <= data->x_end; column++) {
			const uint16_t data_index = counter / 8;
			const uint16_t data_bit   = counter % 8;
			if(ssd1306.display[row_index][column] & (1 << row_bit)) {
				response->pixels_chunk_data[data_index] |= (1 << data_bit);
			}

			counter++;
			if(counter == 60*8) {
				break;
			}
		}
		if(counter == 60*8) {
			break;
		}

		column = data->x_start;
	}

	ssd1306.read_chunk_offset += counter;
	if(ssd1306.read_chunk_offset == response->pixels_length) {
		ssd1306.read_chunk_offset = 0;
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse clear_display(const ClearDisplay *data) {
	for(uint8_t row = 0; row < OLED_MAX_ROWS; row++) {
		for(uint8_t column = 0; column < OLED_MAX_COLUMNS; column++) {
			ssd1306.display_mask[row][column] = 0xFF;
			ssd1306.display[row][column] = 0;
		}
	}

	ssd1306.display_mask_changed = true;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse set_display_configuration(const SetDisplayConfiguration *data) {
	if(data->contrast > 63) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(ssd1306.display_configuration_contrast != data->contrast) {
		ssd1306.new_contrast  = true;
		ssd1306.display_configuration_contrast = data->contrast;
	}

	if(ssd1306.display_configuration_invert != data->invert) {
		ssd1306.new_invert    = true;
		ssd1306.display_configuration_invert = data->invert;
	}

	ssd1306.automatic_draw = data->automatic_draw;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_display_configuration(const GetDisplayConfiguration *data, GetDisplayConfiguration_Response *response) {
	response->header.length  = sizeof(GetDisplayConfiguration_Response);
	response->contrast       = ssd1306.display_configuration_contrast;
	response->invert         = ssd1306.display_configuration_invert;
	response->automatic_draw = ssd1306.automatic_draw;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse write_line(const WriteLine *data) {
	if((data->line > 7) || (data->position > 21)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	for(uint8_t i = 0; i < 21 - data->position; i++) {
		if(data->text[i] == 0) {
			if(ssd1306.automatic_draw) {
				ssd1306.display_mask_changed = true;
			}
			return HANDLE_MESSAGE_RESPONSE_EMPTY;
		}

		for(uint8_t j = 0; j < 6; j++) {
			uint8_t new_data = ((j == 5) ? 0 : font[(uint8_t)data->text[i]*5 + j]);
			uint8_t column = (data->position+i)*6 + j;
			if(column >= OLED_MAX_COLUMNS) {
				if(ssd1306.automatic_draw) {
					ssd1306.display_mask_changed = true;
				}
				return HANDLE_MESSAGE_RESPONSE_EMPTY;
			}

			if(ssd1306.display[data->line][column] != new_data) {
				ssd1306.display_mask[data->line][column] = (ssd1306.display[data->line][column] ^ new_data);
				ssd1306.display[data->line][column] = new_data;
			}
		}
	}

	if(ssd1306.automatic_draw) {
		ssd1306.display_mask_changed = true;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse draw_buffered_frame(const DrawBufferedFrame *data) {
	if(data->force_complete_redraw) {
		memset(ssd1306.display_mask, 0xFF, OLED_MAX_COLUMNS*OLED_MAX_ROWS);
	}

	ssd1306.display_mask_changed = true;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}



void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	communication_callback_init();
}
