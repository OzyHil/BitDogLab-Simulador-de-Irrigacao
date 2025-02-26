#include <General.h>

// Inicializa a configuração do PIO e do clock do sistema
refs InitConf()
{
    refs pio;
    pio.ref = pio0;
    stdio_init_all();
    if (set_sys_clock_khz(128000, false))
        printf("Clock configurado para %ld\n", clock_get_hz(clk_sys));
    pio.offset = pio_add_program(pio.ref, &pio_matrix_program);
    pio.stateMachine = pio_claim_unused_sm(pio.ref, true);
    return pio;
}

// Inicializa o PIO e configura o programa da matriz de LEDs
refs InitPIO()
{
    refs pio = InitConf();
    pio_matrix_program_init(pio.ref, pio.stateMachine, pio.offset, LED_MATRIX);
    return pio;
}

// Inicialiaza e configura um pino como entrada com pull-up ativado
void SetInput(int pin)
{
    gpio_init(pin);
    gpio_set_dir(pin, false);
    gpio_pull_up(pin);
}

// Inicializa e configura um pino como saída e define o valor inicial como false
void SetOutput(int pin)
{
    gpio_init(pin);
    gpio_set_dir(pin, true);
    gpio_put(pin, false);
}

// Inicializa o PWM em um pino GPIO
uint pwm_init_gpio(uint gpio)
{
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, PWM_WRAP);
    pwm_set_enabled(slice_num, true);
    return slice_num;
}