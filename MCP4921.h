/**
 * Copyright (C) 2021 by Yvan FEREZ
 * 
 * @author Yvan FEREZ
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * This program is under the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MCP4921_H_
#define MCP4921_H_
#include <stdint.h>
#include <linux/spi/spidev.h>

#define DAC_SELECT_A          0x80
#define DAC_SELECT_B          0x00

#define BUFFERED_OUTPUT       0x40
#define UNBUFFERED_OUTPUT     0x00

#define OUTPUT_GAIN_1X        0x20
#define OUTPUT_GAIN_2X        0x00

#define OUTPUT_ACTIVE         0x10
#define OUTPUT_INACTIVE       0x00

class MCP4921 {
  private:
    uint8_t raw_buffer [2];
    uint16_t value;
    uint8_t dac_select;
    uint8_t output_buffered;
    uint8_t selected_gain;
    uint8_t active_output;
    uint8_t spi_mode;
    uint32_t spi_speed;
    uint8_t spi_bits_per_word;
    uint16_t spi_delay;
    int spi_fd;
  public:
    static const uint16_t MIN_DAC_VALUE = 0;
    static const uint16_t MAX_DAC_VALUE = 4095;
    static const uint32_t MAX_SPI_SPEED = 1e7;
    static const uint32_t REGISTER_BYTE_SIZE = 2;
    MCP4921(uint16_t val = MCP4921::MIN_DAC_VALUE,
            bool channel_b = false,
            bool buffered_output = false,
            bool gain2x = false,
            bool active_output = true,
            uint8_t spi_mode = SPI_MODE_0,
            uint32_t spi_hz = MCP4921::MAX_SPI_SPEED, // Setted to 10MHz, make sure both of your slave/master can support it.
            uint32_t spi_bits_per_word = 8,
            uint16_t spi_delay = 0
          );
    void setRawValue(uint16_t value);
    int spi_tx();
    ~MCP4921();
  };

#endif /* MCP4921_H_ */
