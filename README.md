# MCP4921

C++11 Library to interface the MCP4921 DAC with a Rasberry Pi's SPI.

Tested with a Raspberry Pi 4 Rev1.4 8Go.

### Pinout

![alt test](http://domoticx.com/wp-content/uploads/2020/12/MCP4921-schema-raspberry-pi.jpg "Credits: Derek Molloy")

### Prerequisites

Enable SPI:

```sh
# Will display friendly graphical CLI, go to "5 Interfacing Options" and "P4 SPI" to enable SPI
sudo raspi-config
```

### Installing

```sh
git clone https://github.com/Linzdigr/MCP4921.git
```

### Compiling (Linux)

```shell
cd MCP4921
make && make clean
sudo ./main
```
