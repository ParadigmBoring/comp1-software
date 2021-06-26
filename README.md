# comp1-software
Software for Competition 1 of the "Not-A-Boring" Competition

## General Setup
### Python Virtual Environment 

Linux
- Ensure you have python3 installed
- `python3 -m venv .venv` 
- `source .venv/bin/activate`

## Control Laptop - Front-end (Angular) setup Guide
Prerequisite: [Node.js(LTS version will be fine) and npm](https://docs.npmjs.com/downloading-and-installing-node-js-and-npm#using-a-node-version-manager-to-install-node-js-and-npm)

1. First, open your terminal. Navigate to `/comp1-software` directory.
2. Run `npm install -g @angular/cli`
3. Navigate to `/OnLand/ControlLaptop/angular-UI`. 
    If you are in the root of the 'comp1-software' folder, then just run:
        Windows: `cd OnLand\ControlLaptop\angular-UI`
        Linux/Mac: `cd OnLand/ControlLaptop/angular-UI`
4. When you in the angular-UI folder, run `npm install`
5. To build and serves the app locally, run `ng serve`. If it's build successfully, there should be a link that look like this: http://localhost:4200/. 
6. Navigate to the link in your browser, and your should be able to see the app.

## Flight Computer setup Guide
Prerequisites:
 - CMake (at least version 3.13)
 - A compiler:
   - MacOS - gcc 9 - `brew install gcc` or install through xcode
   - Linux - gcc 9 - `sudo apt install build-essential`
   - Arm64 Linux (Pi 4)- gcc 9.3 - Manually install from [here](https://github.com/abhiTronix/raspberry-pi-cross-compilers/wiki/64-Bit-Cross-Compiler:-Installation-Instructions)
   - Arm32 Linux (Pi 3B+) - gcc 9.3 - Manually install from [here](https://github.com/abhiTronix/raspberry-pi-cross-compilers/wiki/Cross-Compiler:-Installation-Instructions)
   - Windows - Visual Studio 2019 cl.exe should work fine
     - Note: Ensure that C++ Cmake tools for Windows is installed along with Desktop development with C++

Compiling Protobuf:
 - Unfortunately compiling protobuf and getting it to link properly can be a bit of a pain, so these instructions are a bit more manual for now
 - On Windows:
   - The easiest way to install is to use the vcpkg package manager for C++ libraries to install protobuf and the protoc compiler. The instructions [here](https://github.com/microsoft/vcpkg#quick-start-windows) shows you how to install vcpkg, and the instructions [here](https://github.com/protocolbuffers/protobuf/blob/master/src/README.md) under the header "C++ Installation - Windows" will show you how to install the packages under vcpkg.
    - Note that when cmake is invoked for the FlightComputer project, the cmake toolchain file will have to be set so that protobuf is found
    - As well, the protoc compiler binary will have to be added to the system PATH, which can be done by adding the path to the folder where vcpkg has installed protoc to the PATH variable, as shown [here](https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10/)
 - On Linux/MacOS:
   - The Unix build instructions [here](https://github.com/protocolbuffers/protobuf/blob/master/src/README.md) can be used to build from source, just clone the repo somewhere locally and then follow the instructions. If all goes well, protobuf libraries and includes should be added to the proper directories under /usr/local, and protoc should be invokable from the terminal.

Setup:
1. In a command prompt (Windows)/terminal (MacOS/Linux), navigate to `comp1-software/Protobuf`
2. On Windows, run `updateProtos.bat`, and on MacOS/Linux run `updateProtos.sh`. This will compiler the sample `Paradigm.proto` and copy the source files to the correct directories.
3. Navigate to `comp1-software/Subsurface/FlightComputer`
4. Make a build directory, and enter it
5. Building for Raspberry pi:  
   - Invoke cmake using `cmake -DCMAKE_TOOLCHAIN_FILE=../arm-linux-gnueabihf-gcc.cmake ..`, this should automatically find the cross compiler (assuming it has been added to your PATH), and generate a Makefile.
   - Building for Windows: Invoke cmake using `cmake -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg.cmake> ..`, this will ensure that the protobuf libraries are picked up correctly.
   - Building for MacOS/Linux: Invoke cmake using `cmake ..`, this should generate a Makefile (MacOS/Linux)
6. On Windows: Open the .sln file in Visual Studio, and in the solution explorer set the FlightComputer project as the startup project (right click, "set as startup project"), then build and run.  
On Linux/MacOS/Arm64 Linux: Run `make`, and this should build the project, and then run the executable with `./FlightComputer`


## Node (embedded) setup Guide
Prerequisites:
 - [STM32CubeMx](https://www.st.com/en/development-tools/stm32cubemx.html) (Version 6.1.0)
 - Windows Users will need [WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10)
 - The following packages (should be able to install from package manager):
   - build-essential (`sudo apt install build-essential`)
   - gcc-arm-none-eabi Version 9.2.1 (`sudo apt install gcc-arm-none-eabi`)
     - For macOS users, do the following:
     ```shell
     brew tap PX4/homebrew-px4
     brew update
     brew install gcc-arm-none-eabi
     ```
   - CMake (see below instructions)

CMake
 - Execute CMake --version, if your version is below 3.15.3 - perform the following
    1. Remove your current version of cmake with: `sudo apt-get remove --auto-remove cmake && sudo apt-get purge --auto-remove cmake` 
    2. **Ensure you are in the project python environment** and execute `pip install cmake` 

Setup
1. Open STM32CubeMx, select "File -> Load Project...", and open to `/comp1-software/Examples/Blink/Blink.ioc`
2. Navigate to the Project Manager tab, ensure that the "Toolchain/IDE" is set to "Makefile", and hit the "GENERATE CODE" button. This will create all the required HAL code in to allow for the project to be built
3. In a terminal window, navigate to `comp1-software/Examples/Blink`
4. First, cd into `Core/Src`, and remove `main.c`
5. Cd back to the root of the project, and issue the following commands:
   - `mkdir build && cd build`
   - `cmake -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE=Debug ..`
   - `make`

   There should now be `nucleo-f303re.out`, plus some other files with the same name, and different file extension. This confirms that you have build the project successfully.
