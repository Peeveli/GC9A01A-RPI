# GC9A01A-RPI

Lazy GC9A01A c++ driver based on waveshare examples for Raspberry Pi 3/4/5.
- Utilizes the now streamlined libgpiod 
- Supports only the 2 hardware SPI lanes (spidev0.0/1)
- made this mainly for myself as a learning experience
- might not follow any best practices :3
- feel free to use, modify and redistribute
````
sudo apt install libgpiod-dev libopencv-dev
cd GC9A01A-rpi/
mkdir build && cd build/
cmake ..
make
````
Usage:
````
./gc9a01a-rpi -s ../gifs/apex.gif
````
![alt text](https://github.com/rhadov/GC9A01A-RPI/blob/main/gifs/demo.gif "demo")