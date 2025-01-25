#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
// #include <conio.h>

// clrscr
void clrscr(void) {
  system("clear");
}

// getch - "any key to continue"
void getch(void) {
  system("read -n 1 -s -p \"Press any key to continue...\n\"");
}

// delay (ms)
void delay(unsigned int millis) {
  time_t start_time = clock();
  while (true) { if (clock() > start_time + millis) { break; } }
}

// sleep (seconds)
void sleep(unsigned int seconds) {
  delay(seconds * 1000);
}

// sound
void sound(int freq) { return; }

// nosound
void nosound(void) { return; }