avr-gcc -g -Os -mmcu=atmega32 -c adxl.c
avr-gcc -g -mmcu=atmega32 -o adxl.elf adxl.o
avr-objcopy -j .text -j .data -O ihex adxl.elf adxl.hex
avr-size --format=avr --mcu=atmega32 adxl.elf 
avrdude -c usbasp -p m32 -P /dev/ttyACM0 -B10 -U flash:w:adxl.hex -F
