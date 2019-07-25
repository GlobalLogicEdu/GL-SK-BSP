# BSP for GlobalLogic Starter Kit

Electrical schematic of board variants:
  * [V1.1](https://github.com/GlobalLogicEdu/GL-SK-BSP/blob/master/Documentation/Schematic_1.1.pdf)

## Prerequisites

  * Linux PC with Ubuntu 16.04+ (tested on 18.04)
  * GlobalLogic Starter Kit v1.x

## Preparing

  * Update APT caches and install common tools and libraries
```bash
sudo apt update
sudo apt install -y git vim arm-none-eabi-gcc libnewlib-arm-none-eabi minicom build-essential cmake libusb-1.0-0-dev
```

  * Install Linux [stlink tools](https://github.com/texane/stlink) using [this manual](https://github.com/texane/stlink/blob/master/doc/compiling.md)

  * Create work directory and add it to bash environment as a variable for fast access
```bash
mkdir ~/development
echo "export WORKDIR=~/development" >> ~/.bashrc
source ~/.bashrc
```
