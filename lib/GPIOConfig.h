#ifndef GPIOCONFIG_H
#define GPIOCONFIG_H

#include <cstdint>
#include <gpiod.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

class GPIOConfig {
public:
    GPIOConfig(uint16_t cs, uint16_t rst, uint16_t dc, uint16_t bl, uint32_t spiSpeed, uint8_t spiNum);
    ~GPIOConfig();

    void gpioPinMode(uint16_t pin, uint16_t mode);      // Set the mode of a GPIO pin
    void gpioPinWrite(uint16_t pin, uint8_t value);     // Write a value to a GPIO pin

    void spiWriteByte(uint8_t Value);                   // Write single byte to SPI
    void spiWriteByteN(uint8_t *pdata, uint32_t len);   // Write n bytes to SPI

    // Getter functions (might just use globals but whatever)
    uint16_t getCsPin() const { return csPin; }
    uint16_t getRstPin() const { return rstPin; }
    uint16_t getDcPin() const { return dcPin; }
    uint16_t getBlPin() const { return blPin; }

private:
    void gpioInit();   // Internal GPIO initialization
    void spiInit();    // Internal SPI initialization

    uint16_t csPin, rstPin, dcPin, blPin;
    uint32_t spiSpeed; // stable up to 100Mhz
    uint8_t spiNum;    // 0 or 1 (only hardware SPI supported) 
    int spi_fd = -1;
    
    struct gpiod_chip *chip = nullptr;
    struct gpiod_line *line = nullptr;
};
#endif  // GPIOCONFIG_H
