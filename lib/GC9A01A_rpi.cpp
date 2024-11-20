#include "GC9A01A_rpi.h"
#include <iostream>

// Initialize the display (e.g., reset, configure settings)
void LCDDisplay::initDisplay() {
  reset();
  setAttributes(HORIZONTAL);
  initRegisters();
}

// Reset the display
void LCDDisplay::reset() {
  gpioConfig.gpioPinWrite(gpioConfig.getRstPin(),1);
  usleep(100*1000);
  gpioConfig.gpioPinWrite(gpioConfig.getRstPin(),0);
  usleep(100*1000);
  gpioConfig.gpioPinWrite(gpioConfig.getRstPin(),1);
}

// Write a byte to SPI
void LCDDisplay::writeByte(uint8_t byte) {
  gpioConfig.spiWriteByte(byte);
}

void LCDDisplay::writeByteN(uint8_t *pdata, uint32_t len) {
  gpioConfig.spiWriteByteN(pdata, len);
}

void LCDDisplay::sendCommand(uint8_t command) {
  gpioConfig.gpioPinWrite(gpioConfig.getDcPin(), 0);
  writeByte(command);
}

void LCDDisplay::sendData8(uint8_t data) {
  gpioConfig.gpioPinWrite(gpioConfig.getDcPin(), 1);
  writeByte(data);
}

void LCDDisplay::sendData16(uint16_t data) {
  gpioConfig.gpioPinWrite(gpioConfig.getDcPin(), 1);
  writeByte(data >> 8);
  writeByte(data & 0xFF);
}

void LCDDisplay::setBacklight(uint16_t blVal){
  gpioConfig.gpioPinWrite(gpioConfig.getBlPin(),blVal);
}

void LCDDisplay::setAttributes(uint8_t scanDirection) {
  // Set scan direction and update width and height as needed
  height = 220;
  width = 220;
  rotate = 0;
  mirror = MIRROR_NONE;
  sendCommand(0x36); // Set memory access control (MX, MY, RGB mode)
  sendData8((scanDirection == HORIZONTAL) ? 0xC8 : 0x68);
}

void LCDDisplay::setWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
  sendCommand(0x2A);
  sendData8(0x00);
  sendData8(xStart);
  sendData8((xEnd-1)>>8);
  sendData8(xEnd-1);

  sendCommand(0x2B);
  sendData8(0x00);
  sendData8(yStart);
  sendData8((yEnd-1)>>8);
  sendData8(yEnd-1);
  sendCommand(0x2C);
}

void LCDDisplay::clear(uint16_t color) {
  uint16_t j;
  uint16_t image[width*height];
  color = ((color<<8)&0xff00)|(color>>8);

  for (j = 0; j < height*width; j++) {
    image[j] = color;
  }

  setWindow(0, 0, width, height);
  gpioConfig.gpioPinWrite(gpioConfig.getDcPin(), 1);
  for(j = 0; j < height; j++){
    gpioConfig.spiWriteByteN((uint8_t *)&image[j*width], width*2);
  }
}

// Expects little-endian RGB565 bit array
void LCDDisplay::displayImage(uint16_t *image, uint16_t w, uint16_t h){
  uint16_t j;
  gpioConfig.gpioPinWrite(gpioConfig.getDcPin(), 1);
  for (j = 0; j < h; j++) {
    writeByteN((uint8_t *)&image[j*w], w*2);
  }
}

void LCDDisplay::displayPoint(uint16_t x, uint16_t y, u_int16_t color)
{
  setWindow(x,y,x,y);
  sendData16(color);
}

void LCDDisplay::initRegisters() {
  // Initialize command registers for GC9A01A
  // https://www.buydisplay.com/download/ic/GC9A01A.pdf
  sendCommand(0xEF);
  sendCommand(0xEB);
  sendData8(0x14);
  sendCommand(0xFE);			 
  sendCommand(0xEF); 

	sendCommand(0xEB);	
	sendData8(0x14); 

	sendCommand(0x84);			
	sendData8(0x40); 

	sendCommand(0x85);			
	sendData8(0xFF); 

	sendCommand(0x86);			
	sendData8(0xFF); 

	sendCommand(0x87);			
	sendData8(0xFF);

	sendCommand(0x88);			
	sendData8(0x0A);

	sendCommand(0x89);			
	sendData8(0x21); 

	sendCommand(0x8A);			
	sendData8(0x00); 

	sendCommand(0x8B);			
	sendData8(0x80); 

	sendCommand(0x8C);			
	sendData8(0x01); 

	sendCommand(0x8D);			
	sendData8(0x01); 

	sendCommand(0x8E);			
	sendData8(0xFF); 

	sendCommand(0x8F);			
	sendData8(0xFF); 

  sendCommand(0x21);
  sendData8(0xFF); 

	sendCommand(0xB6);
	sendData8(0x00);
	sendData8(0x20);

	sendCommand(0x36);
	sendData8(0x08); //Set as a vertical screen

	sendCommand(0x3A);			
	sendData8(0x05); 

	sendCommand(0x90);			
	sendData8(0x08);
	sendData8(0x08);
	sendData8(0x08);
	sendData8(0x08); 

	sendCommand(0xBD);			
	sendData8(0x06);
	
	sendCommand(0xBC);			
	sendData8(0x00);	

	sendCommand(0xFF);			
	sendData8(0x60);
	sendData8(0x01);
	sendData8(0x04);

	sendCommand(0xC3);			
	sendData8(0x13);
	sendCommand(0xC4);			
	sendData8(0x13);

	sendCommand(0xC9);			
	sendData8(0x22);

	sendCommand(0xBE);			
	sendData8(0x11); 

	sendCommand(0xE1);			
	sendData8(0x10);
	sendData8(0x0E);

	sendCommand(0xDF);			
	sendData8(0x21);
	sendData8(0x0c);
	sendData8(0x02);

	sendCommand(0xF0);   
	sendData8(0x45);
	sendData8(0x09);
	sendData8(0x08);
	sendData8(0x08);
	sendData8(0x26);
 	sendData8(0x2A);

 	sendCommand(0xF1);    
 	sendData8(0x43);
 	sendData8(0x70);
 	sendData8(0x72);
 	sendData8(0x36);
 	sendData8(0x37);  
 	sendData8(0x6F);

 	sendCommand(0xF2);   
 	sendData8(0x45);
 	sendData8(0x09);
 	sendData8(0x08);
 	sendData8(0x08);
 	sendData8(0x26);
 	sendData8(0x2A);

 	sendCommand(0xF3);   
 	sendData8(0x43);
 	sendData8(0x70);
 	sendData8(0x72);
 	sendData8(0x36);
 	sendData8(0x37); 
 	sendData8(0x6F);

	sendCommand(0xED);	
	sendData8(0x1B); 
	sendData8(0x0B); 

	sendCommand(0xAE);			
	sendData8(0x77);
	
	sendCommand(0xCD);			
	sendData8(0x63);		

	sendCommand(0x70);			
	sendData8(0x07);
	sendData8(0x07);
	sendData8(0x04);
	sendData8(0x0E); 
	sendData8(0x0F); 
	sendData8(0x09);
	sendData8(0x07);
	sendData8(0x08);
	sendData8(0x03);

	sendCommand(0xE8);			
	sendData8(0x34);

	sendCommand(0x62);			
	sendData8(0x18);
	sendData8(0x0D);
	sendData8(0x71);
	sendData8(0xED);
	sendData8(0x70); 
	sendData8(0x70);
	sendData8(0x18);
	sendData8(0x0F);
	sendData8(0x71);
	sendData8(0xEF);
	sendData8(0x70); 
	sendData8(0x70);

	sendCommand(0x63);			
	sendData8(0x18);
	sendData8(0x11);
	sendData8(0x71);
	sendData8(0xF1);
	sendData8(0x70); 
	sendData8(0x70);
	sendData8(0x18);
	sendData8(0x13);
	sendData8(0x71);
	sendData8(0xF3);
	sendData8(0x70); 
	sendData8(0x70);

	sendCommand(0x64);			
	sendData8(0x28);
	sendData8(0x29);
	sendData8(0xF1);
	sendData8(0x01);
	sendData8(0xF1);
	sendData8(0x00);
	sendData8(0x07);

	sendCommand(0x66);			
	sendData8(0x3C);
	sendData8(0x00);
	sendData8(0xCD);
	sendData8(0x67);
	sendData8(0x45);
	sendData8(0x45);
	sendData8(0x10);
	sendData8(0x00);
	sendData8(0x00);
	sendData8(0x00);

	sendCommand(0x67);			
	sendData8(0x00);
	sendData8(0x3C);
	sendData8(0x00);
	sendData8(0x00);
	sendData8(0x00);
	sendData8(0x01);
	sendData8(0x54);
	sendData8(0x10);
	sendData8(0x32);
	sendData8(0x98);

	sendCommand(0x74);			
	sendData8(0x10);	
	sendData8(0x85);	
	sendData8(0x80);
	sendData8(0x00); 
	sendData8(0x00); 
	sendData8(0x4E);
	sendData8(0x00);					
	
  sendCommand(0x98);			
	sendData8(0x3e);
	sendData8(0x07);

	sendCommand(0x35);	
	sendCommand(0x21);

	sendCommand(0x11);
	usleep(120*1000);
	sendCommand(0x29);
	usleep(20*1000);
}