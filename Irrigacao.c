#include "General.h"
#include "Leds.h"
#include "ssd1306.h"
#include "Font.h"

// Variáveis globais
refs pio;                               // Referência do PIO
uint32_t valorLed;                      // Valor do LED a ser enviado
RGB color[3];                           // Cor do LED (RGB)
double *drawing;                        // Desenho (sequência de LEDs acesos)
static volatile uint32_t lastTimeA = 0; // Tempo de última interrupção do botão A
static volatile uint32_t lastTimeB = 0; // Tempo de última interrupção do botão B
static volatile uint32_t lastTimeJ = 0; // Tempo de última interrupção do botão Joystick
ssd1306_t ssd;
static volatile bool temperatureControl = false;
static volatile bool humidityControl = false;
static volatile bool brightnessControl = false;
static volatile uint8_t temperature = 25;
static volatile uint8_t humidity = 60;
static volatile uint8_t brightness = 50;

// Declaração das funções
void SetInterruption(int pin);                          // Configura a interrupção para o botão
void HandleInterruption(uint gpio, uint32_t events);    // Função que lida com a interrupção dos botões
void UpdateLed(uint gpio, volatile uint32_t *lastTime); // Atualiza o LED com base no botão pressionado
void LedInformationMessage(uint gpio, bool ledStatus);
void UpdateDrawing(int number);
void UpdateDisplay();
void ReadJoystick(uint16_t *vrx_value, uint16_t *vry_value);
void UpdateIndicators(uint8_t temperature, uint8_t humidity, uint8_t brightness);

int main()
{
    // Inicializa o PIO (Peripheral Input/Output) para controlar a matriz de LEDs
    pio = InitPIO();

    // Configura botões A e B e atribui interrupções
    SetInput(BUTTON_A);
    SetInterruption(BUTTON_A);
    SetInput(BUTTON_B);
    SetInterruption(BUTTON_B);

    SetInput(JOYSTICK_BUTTON);
    SetInterruption(JOYSTICK_BUTTON);

    adc_init();
    adc_gpio_init(VRX_PIN);
    adc_gpio_init(VRY_PIN);

    // Configura os LED verde e azul como saída
    SetOutput(GREEN_LED);
    SetOutput(BLUE_LED);

    // Define a cor inicial dos LEDs da matriz RGB
    color[0].red = 2;
    color[0].green = 4;
    color[0].blue = 8;

    color[1].red = 7;
    color[1].green = 1;
    color[1].blue = 3;

    color[2].red = 5;
    color[2].green = 9;
    color[2].blue = 6;

    // Inicializa o desenho com o número 0 e exibe na matriz
    drawing = Drawing(554);
    Draw(drawing, valorLed, pio, color);

    // Inicializa comunicação I2C com o display OLED a 400kHz
    i2c_init(I2C_PORT, 400 * 1000);

    // Configuração dos pinos de SDA e SCL para comunicação I2C
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA);                     // Pull up the data line
    gpio_pull_up(I2C_SCL);                     // Pull up the clock line

    // Inicializa o display OLED
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ADRESS, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd);                                       // Configura o display
    ssd1306_send_data(&ssd);                                    // Envia os dados para o display

    // Atualiza o display para inciar com o desenho padrão
    UpdateDisplay();

    uint16_t vrx_value, vry_value;
    uint16_t range = HIGHEST_AXIS_VALUE - LOWEST_AXIS_VALUE;
    
    // Loop principal que mantém o sistema funcionando
    while (true)
    {
        // ReadJoystick(&vrx_value, &vry_value);

        if (temperatureControl)
        {
            temperature = 0 + (vrx_value - LOWEST_AXIS_VALUE) * (50 / range);
            printf("bbbbbb");
        }
        if (humidityControl)
        {
            humidity = 0 + (vrx_value - LOWEST_AXIS_VALUE) * (100 / range);
        }
        if (brightnessControl)
        {
            brightness = 0 + (vrx_value - LOWEST_AXIS_VALUE) * (100 / range);
        }
        UpdateIndicators(temperature, humidity, brightness);

        UpdateDisplay();
        
    }
}

// Função para configurar a interrupção no pino do botão
void SetInterruption(int pin)
{
    gpio_set_irq_enabled(pin, GPIO_IRQ_EDGE_FALL, true); // Ativa interrupção para o pino
    gpio_set_irq_callback(&HandleInterruption);          // Registra a função de callback uma vez
    irq_set_enabled(IO_IRQ_BANK0, true);                 // Ativa interrupções no banco de GPIOs
}

// Função que é chamada quando ocorre uma interrupção de botão
void HandleInterruption(uint gpio, uint32_t events)
{
    uint32_t currentTime = to_us_since_boot(get_absolute_time());

    // Verifica se o botão A foi pressionado
    if (gpio == 5)
    {
        // Verifica se o tempo de debouncing passou (250ms)
        if (currentTime - lastTimeA > 250000)
        {
            lastTimeA = currentTime; // Atualiza o tempo da última interrupção
            temperatureControl = !temperatureControl;
        }
    }
    // Verifica se o botão B foi pressionado
    else if (gpio == 6)
    {
        if (currentTime - lastTimeB > 250000)
        {
            lastTimeB = currentTime; // Atualiza o tempo da última interrupção
            humidityControl = !humidityControl;
        }
    }
    // Verifica se o botão joystick foi pressionado
    else if (gpio == 22)
    {
        if (currentTime - lastTimeJ > 250000)
        {
            lastTimeJ = currentTime; // Atualiza o tempo da última interrupção
            brightnessControl = !brightnessControl;
        }
    }
}

void LedInformationMessage(uint gpio, bool ledStatus)
{

    if (gpio == GREEN_LED)
    {
        if (ledStatus)
        {
            printf("Led Verde ligado\n");
            UpdateDisplay(' ', 0, 0);

            ssd1306_draw_string(&ssd, "LED VERDE ON", 8, 46); // Desenha uma string
            ssd1306_send_data(&ssd);                          // Atualiza o display
        }
        else
        {
            printf("Led Verde desligado\n");
            UpdateDisplay(' ', 0, 0);

            ssd1306_draw_string(&ssd, "LED VERDE OFF", 8, 46); // Desenha uma string
            ssd1306_send_data(&ssd);                           // Atualiza o display
        }
    }
    else if (gpio == BLUE_LED)
    {
        if (ledStatus)
        {
            printf("Led Azul ligado\n");
            UpdateDisplay(' ', 0, 0);

            ssd1306_draw_string(&ssd, "LED AZUL ON", 8, 46); // Desenha uma string
            ssd1306_send_data(&ssd);                         // Atualiza o display
        }
        else
        {
            printf("Led Azul desligado\n");
            UpdateDisplay(' ', 0, 0);

            ssd1306_draw_string(&ssd, "LED AZUL OFF", 8, 46); // Desenha uma string
            ssd1306_send_data(&ssd);                          // Atualiza o display
        }
    }
}

void UpdateDrawing(int number)
{
    // Atualiza o desenho de acordo com o valor
    drawing = Drawing(number);
    Draw(drawing, valorLed, pio, color);
}

void UpdateDisplay()
{
    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    char buffer[32]; // Buffer para armazenar as strings formatadas

    ssd1306_draw_string(&ssd, "DADOS", 45, 5); // Título

    sprintf(buffer, "TEMPERATURA %d", temperature);
    ssd1306_draw_string(&ssd, buffer, 5, 20); // Exibe temperatura

    sprintf(buffer, "UMIDADE %d", humidity);
    ssd1306_draw_string(&ssd, buffer, 5, 35); // Exibe umidade

    sprintf(buffer, "LUMINOSIDADE %d", brightness);
    ssd1306_draw_string(&ssd, buffer, 5, 50); // Exibe luminosidade

    ssd1306_send_data(&ssd); // Atualiza o display
}

// Lê os valores do joystick do ADC
void ReadJoystick(uint16_t *vrx_value, uint16_t *vry_value)
{
    adc_select_input(1);
    *vrx_value = adc_read();
    adc_select_input(0);
    *vry_value = adc_read();
}

void UpdateIndicators(uint8_t temp, uint8_t hum, uint8_t bri)
{
    if (temp > TEMP_MEDIUM_MAX ||
        hum > HUMIDITY_MEDIUM_MAX ||
        bri > BRIGHTNESS_MEDIUM_MAX)
    {
        gpio_put(RED_LED, true);
        gpio_put(GREEN_LED, false);
        gpio_put(BLUE_LED, false);
        // UpdateDrawing(3);
        // acionar buzzer
    }
    else if (temp > TEMP_NORMAL_MAX ||
             hum > HUMIDITY_NORMAL_MAX ||
             bri > BRIGHTNESS_NORMAL_MAX)
    {
        gpio_put(RED_LED, false);
        gpio_put(GREEN_LED, true);
        gpio_put(BLUE_LED, true);
        // UpdateDrawing(2);
    }
    else
    {
        gpio_put(RED_LED, false);
        gpio_put(GREEN_LED, true);
        gpio_put(BLUE_LED, false);
        // UpdateDrawing(1);
    }
}