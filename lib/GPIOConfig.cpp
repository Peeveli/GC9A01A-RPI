#include "GPIOConfig.h"
#include <gpiod.h>
#include <iostream>

GPIOConfig::GPIOConfig(uint16_t cs, uint16_t rst, uint16_t dc, uint16_t bl, uint32_t spiSpeed, uint8_t spiNum)
  : csPin(cs), rstPin(rst), dcPin(dc), blPin(bl), spiSpeed(spiSpeed), spiNum(spiNum) {
  spiInit();
  gpioInit();
}

GPIOConfig::~GPIOConfig() {
  if (chip) {
    gpiod_chip_close(chip);
    std::cout << "chip closed!\n";
  }
  if (spi_fd >= 0) {
    close(spi_fd);
    std::cout << "spi_fd closed!\n";
  }
}

void GPIOConfig::gpioPinMode(uint16_t pin, uint16_t mode) {
  line = gpiod_chip_get_line(chip, pin);                // Get pin address "line"
  if (line) {
    if (mode == 0) {                                  // Modes: 0 = input, 1 = output
        gpiod_line_request_input(line, "dev_input");
    } else {
        gpiod_line_request_output(line, "dev_output", 0);
    }
  }
}

void GPIOConfig::gpioPinWrite(uint16_t pin, uint8_t value) {
  line = gpiod_chip_get_line(chip, pin);
  if (line){
    gpiod_line_set_value(line, value);
    //std::cout << "gpio: " << pin << " line: " << line << "\n";
  }
}

void GPIOConfig::spiWriteByte(uint8_t Value) {
  if (spi_fd >= 0) {
      write(spi_fd, &Value, 1);
  }
}
void GPIOConfig::spiWriteByteN(uint8_t *pdata, uint32_t len) {
  if (spi_fd >= 0) {
    // Send the data in a single transaction
    if (write(spi_fd, pdata, len) != len) {
      perror("SPI write error");
    }
  }
}

void GPIOConfig::gpioInit() {
  chip = gpiod_chip_open_by_name("gpiochip0");
  if (!chip) {
    std::cerr << "Failed to open GPIO chip\n";
    return;
  }
  gpioPinMode(csPin, 1);
  gpioPinMode(rstPin, 1);
  gpioPinMode(dcPin, 1);
  gpioPinMode(blPin, 1);
  gpioPinWrite(csPin, 1);
}

void GPIOConfig::spiInit() {  // Init SPI (only hardware SPIs 0.0/0.1 supported)
  if (spiNum == 0) spi_fd = open("/dev/spidev0.0", O_RDWR);
  else if (spiNum == 1) spi_fd = open("/dev/spidev0.1", O_RDWR);

  if (spi_fd < 0) {
    perror("Failed to open SPI device");
    return;
  }
  uint8_t mode = SPI_MODE_0;
  uint8_t bits = 8;
  ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
  ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
  ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spiSpeed);
}