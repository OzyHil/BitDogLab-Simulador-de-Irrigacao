#ifndef LEDS_H
#define LEDS_H

#define NUM_PIXELS 25 // Quantidade de pixels/LEDs da matriz

#include <General.h>

// Funções de manipulação de cor e desenhos dos LEDs
uint32_t RGBMatrix(RGB color);
void Draw(double *, uint32_t, refs, RGB *);
double *Drawing(int);
void BlinkRGBLed(int);

#endif