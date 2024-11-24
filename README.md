# GC9A01A-RPI

Lazy GC9A01A c++ driver based on waveshare examples\
- Supports only the 2 hardware SPI lanes (spidev0.0/1)
- made this mainly for myself for learning experience
- mgith not follow all the best practices
- feel free to use, modify and redistribte :3
\
````
sudo apt install libgpiod-dev libopencv-dev
cd GC9A01A-rpi/
mkdir build && cd build/
cmake ..
make
````
\
Usage:
````
./gc9a01a-rpi -s ../gifs/apex.gif
````