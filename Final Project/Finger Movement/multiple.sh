avr-gcc -g -Os -mmcu=atmega32 -c multiple_motor_by_servo.c
avr-gcc -g -mmcu=atmega32 -o multiple_motor_by_servo.elf multiple_motor_by_servo.o
avr-objcopy -j .text -j .data -O ihex multiple_motor_by_servo.elf multiple_motor_by_servo.hex
avr-size --format=avr --mcu=atmega32 multiple_motor_by_servo.elf 
avrdude -c usbasp -p m32 -P /dev/ttyACM0 -B10 -U flash:w:multiple_motor_by_servo.hex -F
