avr-gcc -g -Os -mmcu=atmega32 -c main.c
avr-gcc -g -mmcu=atmega32 -o main.elf main.o
avr-objcopy -j .text -j .data -O ihex main.elf main.hex
avr-size --format=avr --mcu=atmega32 main.elf 
avrdude -c usbasp -p m32 -P /dev/ttyACM0 -B10 -U flash:w:main.hex -F
