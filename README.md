# picotemplate

## Building on Linux
This was the original method

```bash  
git clone https://github.com/raspberrypi/pico-sdk.git
git clone git@github.com:rs333/picotemplate.git
export PICO_SDK_PATH=`pwd`/pico-sdk
export PICO_TOOLCHAIN_PATH=<path to your arm toolchain>
cd picotemplate
mkdir build
cd build
cmake ..
make
```

Now just use VSCode and similar to windows, cmake will pull the required dependencies.

## Setting up your windows development environment
When configured, the `pico_sdk_import.cmake` file should download the necessary dependencies you don't have into your `$HOME/.pico_sdk` directory.  While this includes cmake, you will need cmake already installed to perform the initial configuration.  Necessary dependencies are
- cmake
- ninja
- openocd
- picotool
- sdk
- arm toolchain
- pioasm 

## VSCode Extensions
See `extensions.json`

## Outdated Development Tools

| Tool | Link | Description |
| ---------- | ---- | ----------- |
| gcc     | [https://developer.arm.com/downloads/-/gnu-rm](https://developer.arm.com/-/media/Files/downloads/gnu/11.2-2022.02/binrel/gcc-arm-11.2-2022.02-mingw-w64-i686-arm-none-eabi.exe) | Arm GNU Toolchain is a community supported pre-built GNU compiler toolchain for Arm based CPUs. |
| VSCode  | [https://code.visualstudio.com](https://code.visualstudio.com)| A lightweight but powerful source code editor which when run with the appriate extensions effectively becomes a great cross platform Integrated Development Environment. |
| pico-sdk| [https://github.com/raspberrypi/pico-sdk](https://github.com/raspberrypi/pico-sdk)| Headers, Libraries and build system necessary to write programs for the RP2040-based devices such as the Raspberry Pi Pico in C, C++ or assembly language.
| openocd | [https://github.com/raspberrypi/openocd](https://github.com/raspberrypi/openocd) | Provides on-chip programming and debugging support with a layered architecture of JTAG interface and TAP support. |
| cmake - | [https://cmake.org/download](https://cmake.org/download)| An open-source, cross-platform family of tools designed to build, test and package software. |
