cmake_minimum_required(VERSION 3.10)
project(rpi_eyes)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Options to enable support for libgpiod or wiringPi
option(ENABLE_LIBGPIOD "Enable support for libgpiod" ON)
option(ENABLE_WIRINGPI "Enable support for wiringPi" OFF)

# Include all subdirectories in lib/ for headers
include_directories(lib lib/fonts)

# Find all .c files in lib/ and subdirectories
file(GLOB_RECURSE LIB_SOURCES "lib/*.c*")

# Add main application source
set(SRC "src/main.cpp")
set(CMAKE_BUILD_TYPE Debug)

# Check if PkgConfig is available
find_package(PkgConfig)

if(ENABLE_LIBGPIOD)
    # Enable compiler definition
    add_compile_definitions(USE_LIBGPIOD_LIB)

    # Check for libgpiod library
    if(PkgConfig_FOUND)
        pkg_check_modules(LIBGPIOD REQUIRED IMPORTED_TARGET libgpiod)
        message(STATUS "libgpiod library found:")
        message(STATUS "    version: ${LIBGPIOD_VERSION}")
        message(STATUS "    libraries: ${LIBGPIOD_LINK_LIBRARIES}")

        pkg_check_modules(OpenCV REQUIRED IMPORTED_TARGET opencv4)
        message(STATUS "opencv library found:")
        message(STATUS "    version: ${OPENCV_VERSION}")
        message(STATUS "    libraries: ${OpenCV_LINK_LIBRARIES}")

        # Link libraries
        set(EXTRA_LIBS PkgConfig::LIBGPIOD PkgConfig::OpenCV)
    else()
        message(FATAL_ERROR "PkgConfig not found, required for libgpiod support.")
    endif()
endif()

# Conditional logic for wiringPi
if(ENABLE_WIRINGPI)
  # Enable compiler definition
  add_compile_definitions(USE_WIRINGPI_LIB)

  # Check for wiringPi library
  find_library(WIRINGPI_LIB wiringPi)
  if(WIRINGPI_LIB)
    message(STATUS "wiringPi library found:")
    message(STATUS "    version: ${WIRINGPI_VERSION}")
    message(STATUS "    libraries: ${WIRINGPI_LINK_LIBRARIES}")

    # Link wiringPi library
    list(APPEND EXTRA_LIBS ${WIRINGPI_LIB})
  else()
    message(FATAL_ERROR "wiringPi library not found.")
  endif()
endif()

# Add executable target
add_executable(${PROJECT_NAME} ${SRC} ${LIB_SOURCES})
if(EXTRA_LIBS)
    target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})
endif()