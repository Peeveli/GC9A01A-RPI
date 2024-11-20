#include <cmath>
#include <iostream>
#include <cstdint>
#include <cstring>
#include "GC9A01A_rpi.h"
#include "GPIOConfig.h"
#include <opencv2/opencv.hpp>

struct options{
  uint32_t freq;
  uint16_t brightness;
  uint16_t xLoc;
  uint16_t yLoc;
  uint16_t width;
  uint16_t height;
  const char* videoPath;
};
std::unique_ptr<options> options_;

std::vector<uint16_t> convertToRGB565(const cv::Mat& img) {
  std::vector<uint16_t> rgb565(img.rows * img.cols);
  for (int y = 0; y < img.rows; y++) {
    for (int x = 0; x < img.cols; x++) {
      cv::Vec3b pixel = img.at<cv::Vec3b>(y, x);
      uint8_t r = pixel[2] >> 3;  // Red, 5 bits
      uint8_t g = pixel[1] >> 2;  // Green, 6 bits
      uint8_t b = pixel[0] >> 3;  // Blue, 5 bits
      uint16_t color = (r << 11) | (g << 5) | b;
      // Bit swap from big endian to little
      rgb565[y * img.cols + x] = ((color<<8)&0xff00)|(color>>8);
    }
  }
  return rgb565;
}

bool processVideo(LCDDisplay display1, LCDDisplay display2, const char* vidPath, 
                  int width, int height, uint16_t x, uint16_t y) {
  cv::VideoCapture cap(vidPath);

  if (!cap.isOpened()) {
    std::cerr << "Failed to open GIF: " << vidPath << std::endl;
    return -1;
  } else {
    display1.setWindow(x, y, width+x, height+y);
    display2.setWindow(x, y, width+x, height+y);
    cv::Mat frame, resizedFrame;
    while(true) {
      int frameIndex = 0;
      while (cap.read(frame)) {
        // Resize to fit the screen
        cv::resize(frame, resizedFrame, cv::Size(width, height));
        // Convert frame to RGB565
        std::vector<uint16_t> rgb565Frame = convertToRGB565(resizedFrame);
        // Render the 565 converted vector data
        display1.displayImage(rgb565Frame.data(), width, height);
        display2.displayImage(rgb565Frame.data(), width, height);

        frameIndex++;
      }
      cap.set(cv::CAP_PROP_POS_FRAMES, 0); // Replay frames
    }
  }

  cap.release();
  std::cout << "All frames processed!" << std::endl;
  return 0;
}

int main(int argc, char *argv[]) {

  options params = {
    .freq = 80000000,
    .brightness = 255,
		.xLoc = 0,
    .yLoc = 0,
		.width = 240,
		.height = 240,
    .videoPath = ""
	};

  int arg;
  bool hasSource=false;
	while ((arg = getopt(argc, argv, "s:w:h:x:y:f:b:")) != -1) {
		switch (arg) {
			case 's':
				params.videoPath = optarg;
        hasSource=true;
				break;
			case 'w':
				params.width = std::stoi(optarg);
				break;
			case 'h':
				params.height = std::stoi(optarg);
				break;
			case 'x':
			  params.xLoc = std::stoi(optarg);
				break;
			case 'y':
				params.yLoc = std::stoi(optarg);
				break;
			case 'f':
				params.freq = std::stoi(optarg);
				break;
			case 'b':
				params.brightness = std::stoi(optarg);
				break;
			default:
				printf("Usage: %s [-s source file] \n[-w width] [-h height] [-x display coordinate] [-y display coordinate] [-f frequency] [-b brightness] \n", argv[0]);
				break;
		}
	}

  if (!hasSource){
    printf("Usage: %s -s source_file (required)\nOptional: [-w width] [-h height] [-x display coordinate] [-y display coordinate] [-f frequency] [-b brightness] \n", argv[0]);
    exit(1);
  }

  GPIOConfig gpio1(8, 27, 25, 19, params.freq, 0); // CS, RST, DC, BL, speed, bus
  GPIOConfig gpio2(7, 22, 24, 14, params.freq, 1); // CS, RST, DC, BL, speed, bus

  LCDDisplay display1(gpio1);
  LCDDisplay display2(gpio2);
	
  // Initialize each display
  display1.initDisplay();
  display2.initDisplay();
  std::cout << "Initialized!\n";

  display1.setBacklight(params.brightness);
  display2.setBacklight(params.brightness);
  display1.clear(BLACK);
  display2.clear(BLACK);

  if (!processVideo(display1, display2, params.videoPath, params.width, params.height, params.xLoc, params.yLoc)) {
    std::cout<<"Failed to process video/gif/image source...\n";
    return 1;
  }

  return 0;
}