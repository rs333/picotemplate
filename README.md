# picotemplate

## Setting up your windows development environment

### Development Tools

| Tool | Link | Description |
| ---------- | ---- | ----------- |
| msys2   | [https://msys2.org](https://msys2.org) | MSYS2 is a collection of tools and libraries providing you with an easy-to-use environment for building, installing and running native Windows software. |
| gcc     | [https://developer.arm.com/downloads/-/gnu-rm](https://developer.arm.com/-/media/Files/downloads/gnu/11.2-2022.02/binrel/gcc-arm-11.2-2022.02-mingw-w64-i686-arm-none-eabi.exe) | Arm GNU Toolchain is a community supported pre-built GNU compiler toolchain for Arm based CPUs. |
| VSCode  | [https://code.visualstudio.com](https://code.visualstudio.com)| A lightweight but powerful source code editor which when run with the appriate extensions effectively becomes a great cross platform Integrated Development Environment. |
| pico-sdk| [https://github.com/raspberrypi/pico-sdk](https://github.com/raspberrypi/pico-sdk)| Headers, Libraries and build system necessary to write programs for the RP2040-based devices such as the Raspberry Pi Pico in C, C++ or assembly language.
| openocd | [https://github.com/raspberrypi/openocd](https://github.com/raspberrypi/openocd) | Provides on-chip programming and debugging support with a layered architecture of JTAG interface and TAP support. |
| cmake - | [https://cmake.org/download](https://cmake.org/download)| An open-source, cross-platform family of tools designed to build, test and package software. |

### Install MSys2
1. Open a command prompt.
2. Create a common directory to install the development tools.
  
   ```
   mkdir c:\devtools
   ```
3. Download the MSys2 Installer.
4. Install MSys2 using the installer.  Instead of using the default install directory, use `C:\devtools\msys64`
5. 
   
   ```
   mkdir c:\devtools\downloads  
   ```
4. Download the installers for each development toolset into the common folder by visiting the appropriate website listed above.
   - Note: For the arm cross compiler, make sure you downlod the appropriate cross compiler.  For the Raspberry Pi Pico, it should be the `arm-none-eabi` version.
6.  
7.  


### Install MSys2

1. Download the MSys2 Installer


