# Open-Vario

Open-Vario is an open-source multi-function connected variometer for paragliding and hang gliding.

A variometer – also known as a rate of climb and descent indicator (RCDI), rate-of-climb indicator, vertical speed indicator (VSI), or vertical velocity indicator (VVI) – is one of the flight instruments used to inform the pilot of the rate of descent or climb. This is the key information to help looking for the thermals which will allow the glider to climb up and thus to stay in the air and allow multiple hours of flight.

Along with variometer functionality, Open-Vario provides the following features:

* GNSS positioning
* Accelerometer
* Temperature
* Glide ratio computation
* Flight data recording (altitude, speed, position…)
* Local HMI to display sensor and flight data
* BLE link to configure parameter and read real-time flight data values
* USB OTG link to connect to the [XCTrack](https://xctrack.org/) smartphone application
* USB device link to connect to PC python tools to export recorded flights

# Build system

The build system is based on CMake + GCC arm-none-eabi and meant to be run on a Linux platform. An additional helper makefile is provided to simplify the use.
The reference platform to build the project is the following:

* Debian 11 (Bullseye)
* CMake 3.18.4
* GNU Make 4.3
* GCC arm-none-eabi 10.3-2021.10

Additionally, a docker file is provided to build a specific docker image containing all the necessary build tools. Then this image can be used to build the software without having to install all the build tools on the host system.

## Building without docker

Install the following pre-requisites:
* CMake
* GNU Make
* GCC arm-none-eabi
* git

Checkout the source code using the following command to also retrieve the git submodules:

`git clone --recursive https://github.com/open-vario/open-vario`

Edit the line 6 of the **cmake/targets/CMakeLists_stm32wb5mm-dk.txt** file to set the path to your installation of GCC:

`set(CMAKE_TOOLCHAIN_PATH "/opt/gcc-arm-none-eabi-10.3-2021.10/")`

Start the build with the following command:

`make DISABLE_DOCKER=1 all`

The binaries will be produced in the **bin_stm32wb5mm-dk** directory.

## Building with docker

Install the following pre-requisites:

* GNU Make
* git
* wget

Install docker for your host system by following the instructions from the docker website.

Checkout the source code using the following command to also retrieve the git submodules:

`git clone --recursive https://github.com/open-vario/open-vario`

Create the docker image containing the build tools with the following command:

`make docker-image`

Start the build with the following command:

`make all`

The binaries will be produced in the **bin_stm32wb5mm-dk** directory.
