
#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

#include <avr/io.h>
#include "ff.h"

FATFS FatFs;

int main(void)
{
	FIL fil;        /* File object */
    char line[100]; /* Line buffer */
    FRESULT fr;     /* FatFs return code */


    /* Register work area to the default drive */
    f_mount(&FatFs, "", 0);

    /* Open a text file */
    fr = f_open(&fil, "message.txt", FA_READ);
    if (fr) return (int)fr;

    /* Read all lines and display it */
    while (f_gets(line, sizeof line, &fil)) {
        printf(line);
    }

    /* Close the file */
    f_close(&fil);

    return 0;
}
