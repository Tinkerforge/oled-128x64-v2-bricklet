/* oled-128x64-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * ssd1306.c: SSD1306 driver
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

#include "ssd1306.h"

#include "configs/config_ssd1306.h"
#include "bricklib2/os/coop_task.h"
#include "bricklib2/utility/util_definitions.h"

#include "xmc_spi.h"

#define ssd1306_tx_irq_handler IRQ_Hdlr_12

CoopTask ssd1306_task;
SSD1306 ssd1306;

const uint8_t ssd1306_init_conf[] = {
	1, SSD1306_DISPLAYOFF,
	2, SSD1306_SETDISPLAYCLOCKDIV,  0x80,
	2, SSD1306_SETMULTIPLEX,        0x3F,
	2, SSD1306_SETDISPLAYOFFSET,    0x00,
	1, SSD1306_SETSTARTLINE | 0x00,
	2, SSD1306_CHARGEPUMP,          0x14,
	2, SSD1306_MEMORYMODE,          0x00,
	1, SSD1306_SEGREMAP     | 0x01,
	1, SSD1306_COMSCANDEC,
	2, SSD1306_SETCOMPINS,          0x12,
	2, SSD1306_SETCONTRAST,         0x8F,
	2, SSD1306_SETPRECHARGE,        0xF1,
	2, SSD1306_SETVCOMDETECT,       0x40,
	1, SSD1306_DISPLAYALLON_RESUME,
	1, SSD1306_NORMALDISPLAY,
	1, SSD1306_DISPLAYON,
	3, SSD1306_COLUMNADDR,          PIXEL_COL_START, PIXEL_COL_START + PIXEL_W-1,
	3, SSD1306_PAGEADDR,            0, 7,
};

// Use local pointer to save the time for accessing the struct
volatile uint32_t *SSD1306_USIC_IN_PTR = SSD1306_USIC->IN;

// Set pointers to read/write buffer
// With this the compiler can properly optimize the access!
uint8_t *spi_data_read = ssd1306.spi_data;
uint8_t *spi_data_write = ssd1306.spi_data;
uint8_t *spi_data_write_end = ssd1306.spi_data;

void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) ssd1306_tx_irq_handler(void) {
	// Max fill level is 32.
	const uint8_t to_send    = spi_data_write_end - spi_data_write;
	const uint8_t fifo_level = 32 - XMC_USIC_CH_TXFIFO_GetLevel(SSD1306_USIC);
	const uint8_t num        = MIN(to_send, fifo_level);

	switch(num) {
        case 32: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 31: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 30: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 29: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 28: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 27: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 26: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 25: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 24: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 23: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 22: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 21: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 20: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 19: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 18: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 17: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 16: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 15: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 14: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 13: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 12: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 11: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 10: SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 9:  SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 8:  SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 7:  SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 6:  SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 5:  SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 4:  SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 3:  SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 2:  SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
		case 1:  SSD1306_USIC_IN_PTR[0] = *spi_data_write; spi_data_write++;
	}

	if(spi_data_write >= spi_data_write_end) {
		XMC_USIC_CH_TXFIFO_DisableEvent(SSD1306_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
	}
}

void ssd1306_spi_task_transceive(const uint8_t *data, const uint32_t length, XMC_SPI_CH_SLAVE_SELECT_t slave) {
	memcpy(ssd1306.spi_data, data, length);
	ssd1306.spi_data_length = length;
	spi_data_write = ssd1306.spi_data;
	spi_data_write_end = ssd1306.spi_data + length;

	// Fill FIFO with as much data as possible
	XMC_SPI_CH_ClearStatusFlag(SSD1306_USIC, XMC_SPI_CH_STATUS_FLAG_MSLS);
	XMC_USIC_CH_TXFIFO_DisableEvent(SSD1306_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
	while((!XMC_USIC_CH_TXFIFO_IsFull(SSD1306_USIC)) && (spi_data_write < spi_data_write_end)) {
		SSD1306_USIC_IN_PTR[0] = *spi_data_write;
		spi_data_write++;
	}
	NVIC_ClearPendingIRQ(SSD1306_IRQ_TX);

	if(spi_data_write < spi_data_write_end) {
		// If we dont yet have everything written, enable fifo
		XMC_USIC_CH_TXFIFO_EnableEvent(SSD1306_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
	}

	// Enable slave select, FIFO transfer will start here
	XMC_SPI_CH_EnableSlaveSelect(SSD1306_USIC, slave);

	// Wait for data to be written through TX IRQ
	while((spi_data_write < spi_data_write_end) || !XMC_USIC_CH_TXFIFO_IsEmpty(SSD1306_USIC)) {
		coop_task_yield();
	}

	// The FIFO will automatically disable slave select when all data is written
	// We wait until slave select is low here. Otherwise there may still be up to
	// 7 bits send and we have a race condition 
	// (C/D pin can changed before the bits are out).
	while(!XMC_GPIO_GetInput(SSD1306_SELECT_PIN)) {
		// We test for this be reading the actualy slave select pin,
		// there does not seem to be any reliable status flag for this?
		coop_task_yield();
	}
}

void ssd1306_task_write_data(const uint8_t *data, const uint32_t length) {
	XMC_GPIO_SetOutputHigh(SSD1306_CD_PIN);
	ssd1306_spi_task_transceive(data, length, XMC_SPI_CH_SLAVE_SELECT_4);
}

void ssd1306_task_write_command(const uint8_t *command, const uint32_t length) {
	XMC_GPIO_SetOutputLow(SSD1306_CD_PIN);
	ssd1306_spi_task_transceive(command, length, XMC_SPI_CH_SLAVE_SELECT_4);
}

// row 0-7 (8 pixel each), column 0-127
void ssd1306_task_set_cursor(uint8_t row, uint8_t column) {
	uint8_t set_cursor_command[3] = {
		0xB0 + row,
		0x10 + ((column & 0xF0) >> 4),
		(column & 0x0F)
	};

	ssd1306_task_write_command(set_cursor_command, 3);
}

void ssd1306_init_spi(void) {
	// USIC channel configuration
	const XMC_SPI_CH_CONFIG_t channel_config = {
		.baudrate       = SSD1306_SPI_BAUDRATE,
		.bus_mode       = XMC_SPI_CH_BUS_MODE_MASTER,
		.selo_inversion = XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS,
		.parity_mode    = XMC_USIC_CH_PARITY_MODE_NONE
	};

	// MOSI pin configuration
	const XMC_GPIO_CONFIG_t mosi_pin_config = {
		.mode             = SSD1306_MOSI_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

    // SELECT pin configuration
	const XMC_GPIO_CONFIG_t select_pin_config = {
		.mode             = SSD1306_SELECT_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// SCLK pin configuration
	const XMC_GPIO_CONFIG_t sclk_pin_config = {
		.mode             = SSD1306_SCLK_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// Initialize USIC channel in SPI master mode
	XMC_SPI_CH_Init(SSD1306_USIC, &channel_config);
	SSD1306_USIC->SCTR &= ~USIC_CH_SCTR_PDL_Msk; // Set passive data level to 0

	XMC_SPI_CH_SetSlaveSelectPolarity(SSD1306_USIC, XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS);
	XMC_SPI_CH_SetBitOrderMsbFirst(SSD1306_USIC);

	XMC_SPI_CH_SetWordLength(SSD1306_USIC, (uint8_t)8U);
	XMC_SPI_CH_SetFrameLength(SSD1306_USIC, (uint8_t)64U);

	XMC_SPI_CH_SetTransmitMode(SSD1306_USIC, XMC_SPI_CH_MODE_STANDARD);

	// Configure the clock polarity and clock delay
	XMC_SPI_CH_ConfigureShiftClockOutput(SSD1306_USIC,
									     XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_1_DELAY_DISABLED,
									     XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK);

	// Configure Leading/Trailing delay
	XMC_SPI_CH_SetSlaveSelectDelay(SSD1306_USIC, 2);

	// Disable FEM, such that FIFO will automatically disable slave select
	XMC_SPI_CH_DisableFEM(SSD1306_USIC);

	// SPI Mode: CPOL=1 and CPHA=1
	SSD1306_USIC_CHANNEL->DX1CR |= USIC_CH_DX1CR_DPOL_Msk;

	// Configure transmit FIFO
	XMC_USIC_CH_TXFIFO_Configure(SSD1306_USIC, 32, XMC_USIC_CH_FIFO_SIZE_32WORDS, 8);

	// Set service request for tx FIFO transmit interrupt
	XMC_USIC_CH_TXFIFO_SetInterruptNodePointer(SSD1306_USIC, XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_STANDARD, SSD1306_SERVICE_REQUEST_TX);  // IRQ SSD1306_IRQ_TX

	//Set priority and enable NVIC node for transmit interrupt
	NVIC_SetPriority((IRQn_Type)SSD1306_IRQ_TX, SSD1306_IRQ_TX_PRIORITY);
	NVIC_EnableIRQ((IRQn_Type)SSD1306_IRQ_TX);

	// Start SPI
	XMC_SPI_CH_Start(SSD1306_USIC);

	// Configure slave select pin
	XMC_GPIO_Init(SSD1306_SELECT_PIN, &select_pin_config);

	// Configure SCLK pin
	XMC_GPIO_Init(SSD1306_SCLK_PIN, &sclk_pin_config);

	// Configure MOSI pin
	XMC_GPIO_Init(SSD1306_MOSI_PIN, &mosi_pin_config);
}

void ssd1306_trigger_draw(void) {
	if(ssd1306.drawing) {
		ssd1306.trigger_draw_after_draw = true;
		return;
	}
	// Write display and mask to double buffer
	// This way the user can already change the display again while we are still writing to it
	memcpy(ssd1306.display_write, ssd1306.display, OLED_MAX_ROWS*OLED_MAX_COLUMNS);
	for(uint8_t row = 0; row < OLED_MAX_ROWS; row++) {
		for(uint8_t column = 0; column < OLED_MAX_COLUMNS; column++) {
			ssd1306.display_mask_write[row][column] |= ssd1306.display_mask[row][column];
		}
	}
	memset(ssd1306.display_mask, 0, OLED_MAX_ROWS*OLED_MAX_COLUMNS);
	ssd1306.display_mask_changed = true;
	ssd1306.trigger_draw_after_draw = false;
}

void ssd1306_task_tick(void) {
    while(true) {
		if(ssd1306.reset) {
			ssd1306.reset = false;

			XMC_GPIO_SetOutputLow(SSD1306_RST_PIN);
			coop_task_sleep_ms(2);
			XMC_GPIO_SetOutputHigh(SSD1306_RST_PIN);
			coop_task_sleep_ms(6);
		}

		if(ssd1306.initialize) {
			ssd1306.initialize = false;

            uint16_t i = 0;
            while(i < sizeof(ssd1306_init_conf)) {
                const uint8_t length = ssd1306_init_conf[i];
                ssd1306_task_write_command(ssd1306_init_conf + i + 1, length);
                i += length + 1;
			    coop_task_sleep_ms(1);
            }
		}

		if(ssd1306.new_invert) {
			ssd1306.new_invert = false;

			uint8_t invert_data = ssd1306.display_configuration_invert ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY;
			ssd1306_task_write_command(&invert_data, 1);
		}

		if(ssd1306.new_contrast) {
			ssd1306.new_contrast = false;

			uint8_t contrast_data[2] = {SSD1306_SETCONTRAST, ssd1306.display_configuration_contrast};
			ssd1306_task_write_command(contrast_data, 2);
		}

		if(ssd1306.display_mask_changed) {
			ssd1306.display_mask_changed = false;
			ssd1306.drawing = true;

			for(uint8_t row = 0; row < OLED_MAX_ROWS; row++) {
				bool start_found = false;
				uint8_t column_start = 0;
				uint8_t column_end = 0;
				for(uint8_t column = 0; column < OLED_MAX_COLUMNS; column++) {
					if(ssd1306.display_mask_write[row][column] != 0) {
						if(!start_found) {
							start_found = true;
							column_start = column;
						}
						column_end = column;
					}
				}

				if(start_found) {
					ssd1306_task_set_cursor(row, column_start);
					ssd1306_task_write_data(&ssd1306.display_write[row][column_start], column_end - column_start + 1);
				}
			}

			memset(ssd1306.display_mask_write, 0, OLED_MAX_ROWS*OLED_MAX_COLUMNS);

			ssd1306.drawing = false;
			if(ssd1306.trigger_draw_after_draw) {
				ssd1306_trigger_draw();
			}
		}

        coop_task_yield();
    }
}

void ssd1306_init(void) {
	// pin configuration high
	const XMC_GPIO_CONFIG_t high_pin_config = {
		.mode             = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	XMC_GPIO_Init(SSD1306_RST_PIN, &high_pin_config);
	XMC_GPIO_Init(SSD1306_CD_PIN, &high_pin_config);

    memset(&ssd1306, 0, sizeof(SSD1306));
    ssd1306.reset                = true;
    ssd1306.initialize           = true;
    ssd1306.display_mask_changed = true;

    memset(ssd1306.display, 0x00, OLED_MAX_ROWS*OLED_MAX_COLUMNS);
    memset(ssd1306.display_mask, 0x00, OLED_MAX_ROWS*OLED_MAX_COLUMNS);

    memset(ssd1306.display_write, 0x00, OLED_MAX_ROWS*OLED_MAX_COLUMNS);
    memset(ssd1306.display_mask_write, 0xFF, OLED_MAX_ROWS*OLED_MAX_COLUMNS);

    ssd1306.display_configuration_contrast  = 143;
	ssd1306.display_configuration_invert    = false;
	ssd1306.automatic_draw                  = true;

    ssd1306_init_spi();
    coop_task_init(&ssd1306_task, ssd1306_task_tick);
}

void ssd1306_tick(void) {
    coop_task_tick(&ssd1306_task);
}
