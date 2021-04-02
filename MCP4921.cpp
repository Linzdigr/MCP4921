#include <stdint.h>
#include <stdio.h>
#include <unistd.h> 
#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>

  using namespace std;

#include "MCP4921.h"

/**
 * Creates and initialises an instance of MCP4921
 * Please see default values defined in header file.
 * 
 *  @param val The initial raw value to configure on the DAC.
 *  @param channel_b (!(A)/B) bool to select the channel A or B available on the MCP4922 (must be always false to choose A on MCP4921).
 *  @param buffered_output (BUF) true to use buffered output (low impedance), false otherwise (high impedance).
 *  @param gain2x (!GA) Either use 2x output gain (VOUT = 2 * VREF * D/4096) or not. True: x2; False: x1
 *  @param active_output (!SHDN) Enable (true) or disable (false) selected output.
 *  @param spi_mode Wich spi mode to use (0 to 3, see SPI_MODE_x in spidev.h).
 *  @param spi_hz Clock SPI frequency to use.
 *  @param spi_bits_per_word Usually 8 bits / 1 byte.
 *  @param spi_delay Microseconds to delay after this transfer before (optionally) changing the chipselect status, then starting the next transfer or completing the spi_message.
 * 
 *  @throw string Thrown if something went wrong on ioctl calls or on sending first data to device.
 */
MCP4921::MCP4921(uint16_t val,
    bool channel_b,
    bool buffered_output,
    bool gain2x,
    bool active_output,
    uint8_t spi_mode,
    uint32_t spi_hz,
    uint32_t spi_bits_per_word,
    uint16_t spi_delay) {
  if(val > MAX_DAC_VALUE) {
    cout << "Requested DAC value is beyong max limit of "
      << MCP4921::MAX_DAC_VALUE 
      << "! Setting to the max admissible value." 
      << endl;
    val = MCP4921::MAX_DAC_VALUE;
  }

  if(spi_hz > MCP4921::MAX_SPI_SPEED) {
    spi_hz = MCP4921::MAX_SPI_SPEED;
  }

  this->spi_speed = spi_hz;

  if(val > MCP4921::MAX_DAC_VALUE) {
    this->value = MCP4921::MAX_DAC_VALUE;
  }

  this->dac_select = channel_b ? DAC_SELECT_B : DAC_SELECT_B;
  this->output_buffered = buffered_output ? BUFFERED_OUTPUT : UNBUFFERED_OUTPUT;
  this->selected_gain = gain2x ? OUTPUT_GAIN_2X : OUTPUT_GAIN_1X;
  this->active_output = active_output ? OUTPUT_ACTIVE : OUTPUT_INACTIVE;

  const char *filename = (char*)"/dev/spidev0.0";

  if((this->spi_fd = open(filename, O_RDWR)) < 0) {
    throw string("Failed to open spi device");
  }

  if(ioctl(this->spi_fd, SPI_IOC_WR_MODE, &(this->spi_mode)) < 0) {
    throw string("Could not set SPIMode (WR)...ioctl fail");
  }

  if (ioctl(this->spi_fd, SPI_IOC_RD_MODE, &(this->spi_mode)) < 0) {
    throw string("Could not set SPIMode (RD)...ioctl fail");
  }

  if(ioctl(this->spi_fd, SPI_IOC_WR_BITS_PER_WORD, &(this->spi_bits_per_word)) < 0) {
    throw string("Could not set SPI bitsPerWord (WR)...ioctl fail");
  }

  if(ioctl(this->spi_fd, SPI_IOC_RD_BITS_PER_WORD, &(this->spi_bits_per_word)) < 0) {
    throw string("Could not set SPI bitsPerWord(RD)...ioctl fail");
  }

  if(ioctl(this->spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &(this->spi_speed)) < 0) {
    throw string("Could not set SPI speed (WR)...ioctl fail");
  }

  if(ioctl(this->spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &(this->spi_speed)) < 0) {
    throw string("Could not set SPI speed (RD)...ioctl fail");
  }

  // writing to device
  if(this->spi_tx() < 0) {
    throw string("Could not send initial data to device!");
  }
}

/**
 * Sends actual raw_buffer to configured device
 */
int MCP4921::spi_tx() {
  uint8_t rx_buf [16] = {0};

  struct spi_ioc_transfer tr = {
    .tx_buf = (unsigned long)this->raw_buffer,
    .rx_buf = (unsigned long)rx_buf,
    .len = MCP4921::REGISTER_BYTE_SIZE,
    .speed_hz = this->spi_speed,
    .delay_usecs = this->spi_delay,
    .bits_per_word = this->spi_bits_per_word
  };

  return ioctl(this->spi_fd, SPI_IOC_MESSAGE(1), &tr);
}

/**
 * Sets and requests to send the raw DAC value
 *  @param value The raw value to be sent to set the DAC output. If value is beyong max, it'll defaults to it. 
 */
void MCP4921::setRawValue(uint16_t value) {
  if(value > MCP4921::MAX_DAC_VALUE) {
    value = MCP4921::MAX_DAC_VALUE;
  }

  this->raw_buffer[0] = (this->dac_select | this->output_buffered | this->selected_gain | this->active_output) | value >> 8;
  this->raw_buffer[1] = (uint8_t)(value);

  this->spi_tx();
}

/**
 * Destructor of MCP4921
 *  @param value The raw value to be sent to set the DAC output. If value is beyong max, it'll defaults to it. 
 */
MCP4921::~MCP4921() {
  cout << "Detroying MCP4921" << endl;
  close(this->spi_fd);
  this->spi_fd = -1;
}
