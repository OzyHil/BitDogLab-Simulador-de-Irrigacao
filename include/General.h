#ifndef GENERAL_H
#define GENERAL_H

#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>
#include <math.h>
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "pio_matrix.pio.h"
#include "hardware/i2c.h"

#define BUTTON_A 5   // Pino do Botão A
#define BUTTON_B 6   // Pino do Botão B
#define GREEN_LED 11 // Pino do LED verde
#define BLUE_LED 12  // Pino do LED azul
#define RED_LED 13   // Pino do LED vermelho
#define LED_MATRIX 7 // Pino da matriz de LEDs WS2812
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ADRESS 0x3C
#define JOYSTICK_BUTTON 22       // Botão do joystick
#define LOWEST_AXIS_VALUE 16     // Menor valor lido pelo ADC do joystick
#define HIGHEST_AXIS_VALUE 4082  // Maior valor lido pelo ADC do joystick
#define VRX_PIN 26               // Pino do joystick eixo X
#define VRY_PIN 27               // Pino do joystick eixo Y
#define PWM_WRAP 4066            // Resolução do PWM

#define TEMP_NORMAL_MAX 30
#define TEMP_MEDIUM_MAX 40
#define HUMIDITY_NORMAL_MAX 60
#define HUMIDITY_MEDIUM_MAX 80
#define BRIGHTNESS_NORMAL_MAX 70
#define BRIGHTNESS_MEDIUM_MAX 90

// Struct para manipulação da PIO
typedef struct PIORefs
{
    PIO ref;
    uint offset;
    uint stateMachine;
} refs;

// Struct para manipulação das cores dos LEDs
typedef struct RGB
{
    int8_t red;
    int8_t green;
    int8_t blue;
} RGB;

// Funções de configuração
refs InitConf();
refs InitPIO();
void SetInput(int);
void SetOutput(int);
uint pwm_init_gpio(uint gpio);

#endif