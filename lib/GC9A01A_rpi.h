#pragma once

#include <vector>
#include <gpiod.h>
#include <fcntl.h>
#include <cstdint>
#include <sys/ioctl.h>
#include "GPIOConfig.h"

#ifndef LCDDISPLAY_H
#define LCDDISPLAY_H

#define WHITE         0xFFFF
#define BLACK         0x0000
#define BLUE          0x001F
#define BRED          0XF81F
#define GRED 		      0XFFE0
#define GBLUE		      0X07FF
#define RED           0xF800
#define MAGENTA       0xF81F
#define GREEN         0x07E0
#define CYAN          0x7FFF
#define YELLOW        0xFFE0
#define BROWN 		    0XBC40
#define BRRED 		    0XFC07
#define GRAY  		    0X8430

#define MIRROR_NONE       0x00
#define MIRROR_HORIZONTAL 0x01
#define MIRROR_VERTICAL   0x02
#define MIRROR_ORIGIN     0x03

#define HORIZONTAL 0
#define VERTICAL   1

class LCDDisplay {
public:
  LCDDisplay(GPIOConfig& gpio) : gpioConfig(gpio) {}  // Constructor with pin assignments and SPI bus selection
  void initDisplay();                                 // Startup init
  void initRegisters();                               // Init registers per display
  void reset();
  void clear(uint16_t color);
  void writeByte(uint8_t byte);
  void writeByteN(uint8_t *pdata, uint32_t len);
  void sendData8(uint8_t data);
  void sendData16(uint16_t data);
  void sendCommand(uint8_t command);
  void setAttributes(uint8_t scanDirection);
  void setWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
  void setPixel(uint16_t xPoint, uint16_t yPoint, uint16_t color);
  void setBacklight(uint16_t blVal);
  void displayImage(uint16_t *image, uint16_t w, uint16_t h);
  void displayPoint(uint16_t x, uint16_t y, u_int16_t color);

private:
  uint16_t height, width, rotate, mirror, depth;
  GPIOConfig& gpioConfig;
};

#endif  // LCDDISPLAY_H
