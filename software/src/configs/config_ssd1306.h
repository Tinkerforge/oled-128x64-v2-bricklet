/* lcd-128x64-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config_ssd1306.h: Configuration for SSD1306 lcd controller
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

#ifndef CONFIG_SSD1306_H
#define CONFIG_SSD1306_H

#include "xmc_gpio.h"

#define SSD1306_SPI_BAUDRATE         1500000
#define SSD1306_USIC_CHANNEL         USIC0_CH0
#define SSD1306_USIC                 XMC_SPI0_CH0

#define SSD1306_SCLK_PIN             P0_14
#define SSD1306_SCLK_PIN_AF          (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7 | P0_14_AF_U0C0_SCLKOUT)

#define SSD1306_SELECT_PIN           P0_13
#define SSD1306_SELECT_PIN_AF        (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT6 | P0_13_AF_U0C0_SELO4)

#define SSD1306_MOSI_PIN             P2_0
#define SSD1306_MOSI_PIN_AF          (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT6 | P2_0_AF_U0C0_DOUT0)

#define SSD1306_SERVICE_REQUEST_TX   3

#define SSD1306_IRQ_TX               12
#define SSD1306_IRQ_TX_PRIORITY      1


#define SSD1306_RST_PIN              P2_1
#define SSD1306_CD_PIN               P0_8

#endif