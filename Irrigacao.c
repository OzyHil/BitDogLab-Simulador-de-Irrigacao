/**
 * Sistema de monitoramento ambiental com controle de temperatura, umidade e luminosidade
 * Utiliza display OLED SSD1306, matriz de LEDs RGB, botões e joystick
 */

 #include "General.h"
 #include "Leds.h"
 #include "ssd1306.h"
 #include "Font.h"
 
 // ==================== DEFINIÇÃO DE ESTRUTURAS ====================
 
 /**
  * Estrutura para armazenar os estados do sistema
  */
 typedef struct {
     bool temperatureControl;   // Controle de temperatura ativo
     bool humidityControl;      // Controle de umidade ativo
     bool brightnessControl;    // Controle de luminosidade ativo
     bool soundAlert;           // Alerta sonoro ativo
     uint8_t temperature;       // Valor atual da temperatura
     uint8_t humidity;          // Valor atual da umidade
     uint8_t brightness;        // Valor atual da luminosidade
 } SystemState;
 
 // ==================== VARIÁVEIS GLOBAIS ====================
 
 refs pio;                               // Referência do PIO para controle da matriz de LEDs
 uint32_t valorLed;                      // Valor do LED a ser enviado para a matriz
 RGB color[3];                           // Configuração de cores dos LEDs (RGB)
 double *drawing;                        // Ponteiro para o desenho atual (sequência de LEDs)
 ssd1306_t ssd;                          // Estrutura de controle do display OLED
 
 // Variáveis para debounce dos botões
 static volatile uint32_t lastTimeA = 0; // Tempo da última interrupção do botão A
 static volatile uint32_t lastTimeB = 0; // Tempo da última interrupção do botão B
 static volatile uint32_t lastTimeJ = 0; // Tempo da última interrupção do botão Joystick
 
 // Estado do sistema
 static volatile SystemState systemState = {
     .temperatureControl = false,
     .humidityControl = false,
     .brightnessControl = false,
     .soundAlert = false,
     .temperature = 25,
     .humidity = 60,
     .brightness = 50
 };
 
 // ==================== PROTÓTIPOS DE FUNÇÕES ====================
 
 // Configuração e inicialização
 void InitSystem(void);                                   // Inicializa todos os componentes do sistema
 void ConfigureInputs(void);                              // Configura entradas (botões e joystick)
 void ConfigureOutputs(void);                             // Configura saídas (LEDs e buzzer)
 void ConfigureDisplay(void);                             // Configura o display OLED
 void SetDefaultLedColors(void);                          // Define as cores padrão dos LEDs
 
 // Interrupções e controle de entrada
 void SetInterruption(int pin);                           // Configura interrupção para um pino
 void HandleInterruption(uint gpio, uint32_t events);     // Manipula interrupções dos botões
 void ReadJoystick(uint16_t *vrx_value, uint16_t *vry_value); // Lê valores do joystick
 
 // Funções de atualização
 void UpdateSystemState(uint16_t vrx_value);               // Atualiza o estado do sistema com base no joystick
 void UpdateDrawing(int patternCode);                      // Atualiza o padrão na matriz de LEDs
 void UpdateDisplay(void);                                 // Atualiza as informações no display OLED
 void UpdateIndicators(void);                              // Atualiza LEDs indicadores e alarme
 
 // ==================== FUNÇÃO PRINCIPAL ====================
 
 int main(void)
 {
     // Inicializa componentes do sistema
     InitSystem();
     
     uint16_t vrx_value, vry_value;
     
     // Loop principal
     while (true)
     {
         // Lê valores do joystick
         ReadJoystick(&vrx_value, &vry_value);
         
         // Atualiza estados do sistema baseado nos controles ativos
         UpdateSystemState(vrx_value);
         
         // Atualiza indicadores visuais e sonoros
         UpdateIndicators();
         
         // Atualiza display OLED
         UpdateDisplay();
         
         // Controla o buzzer
         pwm_set_gpio_level(BUZZER_A, systemState.soundAlert ? 20000 : 0);
     }
 }
 
 // ==================== IMPLEMENTAÇÃO DAS FUNÇÕES ====================
 
 /**
  * Inicializa todos os componentes do sistema
  */
 void InitSystem(void)
 {
     // Inicializa o PIO para controle da matriz de LEDs
     pio = InitPIO();
     
     // Configura entradas, saídas e interrupções
     ConfigureInputs();
     ConfigureOutputs();
     
     // Configura ADC para joystick
     adc_init();
     adc_gpio_init(VRX_PIN);
     adc_gpio_init(VRY_PIN);
     
     // Define as cores padrão para a matriz de LEDs
     SetDefaultLedColors();
     
     // Inicializa desenho padrão (código 554)
     drawing = Drawing(554);
     Draw(drawing, valorLed, pio, color);
     
     // Configura display OLED
     ConfigureDisplay();
     
     // Inicializa PWM para buzzer
     pwm_init_gpio(BUZZER_A);
 }
 
 /**
  * Configura as entradas do sistema (botões e joystick)
  */
 void ConfigureInputs(void)
 {
     // Configura botões A e B
     SetInput(BUTTON_A);
     SetInterruption(BUTTON_A);
     
     SetInput(BUTTON_B);
     SetInterruption(BUTTON_B);
     
     // Configura botão do joystick
     SetInput(JOYSTICK_BUTTON);
     SetInterruption(JOYSTICK_BUTTON);
 }
 
 /**
  * Configura as saídas do sistema (LEDs)
  */
 void ConfigureOutputs(void)
 {
     SetOutput(RED_LED);
     SetOutput(GREEN_LED);
 }
 
 /**
  * Configura o display OLED
  */
 void ConfigureDisplay(void)
 {
     // Inicializa comunicação I2C com o display OLED a 400kHz
     i2c_init(I2C_PORT, 400 * 1000);
     
     // Configuração dos pinos SDA e SCL
     gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
     gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
     gpio_pull_up(I2C_SDA);
     gpio_pull_up(I2C_SCL);
     
     // Inicializa o display OLED
     ssd1306_init(&ssd, WIDTH, HEIGHT, false, ADRESS, I2C_PORT);
     ssd1306_config(&ssd);
     ssd1306_send_data(&ssd);
     
     // Atualização inicial do display
     UpdateDisplay();
 }
 
 /**
  * Define as cores padrão dos LEDs da matriz RGB
  */
 void SetDefaultLedColors(void)
 {
     // Define cor principal (índice 0)
     color[0].red = 2;
     color[0].green = 4;
     color[0].blue = 8;
     
     // As outras cores podem ser descomentas se necessário
     /*
     color[1].red = 7;
     color[1].green = 1;
     color[1].blue = 3;
     
     color[2].red = 5;
     color[2].green = 9;
     color[2].blue = 6;
     */
 }
 
 /**
  * Configura a interrupção para um pino específico
  * @param pin Número do pino GPIO a ser configurado
  */
 void SetInterruption(int pin)
 {
     // Ativa interrupção de borda de descida para o pino
     gpio_set_irq_enabled(pin, GPIO_IRQ_EDGE_FALL, true);
     
     // Registra a função de callback para interrupções
     gpio_set_irq_callback(&HandleInterruption);
     
     // Ativa interrupções no banco de GPIOs
     irq_set_enabled(IO_IRQ_BANK0, true);
 }
 
 /**
  * Manipula as interrupções dos botões com debounce
  * @param gpio Pino que gerou a interrupção
  * @param events Tipo de evento ocorrido
  */
 void HandleInterruption(uint gpio, uint32_t events)
 {
     uint32_t currentTime = to_us_since_boot(get_absolute_time());
     
     // Botão A (Temperatura) - GPIO 5
     if (gpio == BUTTON_A && currentTime - lastTimeA > 250000)
     {
         lastTimeA = currentTime;
         systemState.temperatureControl = !systemState.temperatureControl;
         
         // Desativa outros controles quando este é ativado
         if (systemState.temperatureControl)
         {
             systemState.humidityControl = false;
             systemState.brightnessControl = false;
         }
     }
     // Botão B (Umidade) - GPIO 6
     else if (gpio == BUTTON_B && currentTime - lastTimeB > 250000)
     {
         lastTimeB = currentTime;
         systemState.humidityControl = !systemState.humidityControl;
         
         // Desativa outros controles quando este é ativado
         if (systemState.humidityControl)
         {
             systemState.temperatureControl = false;
             systemState.brightnessControl = false;
         }
     }
     // Botão do Joystick (Luminosidade) - GPIO 22
     else if (gpio == JOYSTICK_BUTTON && currentTime - lastTimeJ > 250000)
     {
         lastTimeJ = currentTime;
         systemState.brightnessControl = !systemState.brightnessControl;
         
         // Desativa outros controles quando este é ativado
         if (systemState.brightnessControl)
         {
             systemState.temperatureControl = false;
             systemState.humidityControl = false;
         }
     }
 }
 
 /**
  * Atualiza o desenho da matriz de LEDs
  * @param patternCode Código do padrão a ser desenhado
  */
 void UpdateDrawing(int patternCode)
 {
     drawing = Drawing(patternCode);
     Draw(drawing, valorLed, pio, color);
 }
 
 /**
  * Atualiza as informações mostradas no display OLED
  */
 void UpdateDisplay(void)
 {
     // Limpa o display
     ssd1306_fill(&ssd, false);
     
     char buffer[32]; // Buffer para strings formatadas
     
     // Título
     ssd1306_draw_string(&ssd, "DADOS", 45, 5);
     
     // Informações com indicadores visuais de estado de controle
     sprintf(buffer, "TEMPERATURA %d%s", systemState.temperature, 
             systemState.temperatureControl ? " *" : "");
     ssd1306_draw_string(&ssd, buffer, 5, 20);
     
     sprintf(buffer, "UMIDADE %d%s", systemState.humidity, 
             systemState.humidityControl ? " *" : "");
     ssd1306_draw_string(&ssd, buffer, 5, 35);
     
     sprintf(buffer, "LUMINOSIDADE %d%s", systemState.brightness, 
             systemState.brightnessControl ? " *" : "");
     ssd1306_draw_string(&ssd, buffer, 5, 50);
     
     // Atualiza o display
     ssd1306_send_data(&ssd);
 }
 
 /**
  * Lê os valores do joystick através do ADC
  * @param vrx_value Ponteiro para armazenar o valor do eixo X
  * @param vry_value Ponteiro para armazenar o valor do eixo Y
  */
 void ReadJoystick(uint16_t *vrx_value, uint16_t *vry_value)
 {
     // Lê valor do eixo X (ADC1)
     adc_select_input(1);
     *vrx_value = adc_read();
     
     // Lê valor do eixo Y (ADC0)
     adc_select_input(0);
     *vry_value = adc_read();
 }
 
 /**
  * Atualiza o estado do sistema com base nos valores do joystick
  * @param vrx_value Valor atual do eixo X do joystick
  */
 void UpdateSystemState(uint16_t vrx_value)
 {
     // Calcula o intervalo útil do joystick
     uint16_t range = HIGHEST_AXIS_VALUE - LOWEST_AXIS_VALUE;
     
     // Atualiza valores com base nos controles ativos
     if (systemState.temperatureControl)
     {
         systemState.temperature = (vrx_value - LOWEST_AXIS_VALUE) * 50 / range;
     }
     
     if (systemState.humidityControl)
     {
         systemState.humidity = (vrx_value - LOWEST_AXIS_VALUE) * 60 / range;
     }
     
     if (systemState.brightnessControl)
     {
         systemState.brightness = (vrx_value - LOWEST_AXIS_VALUE) * 70 / range;
     }
 }
 
 /**
  * Atualiza os indicadores de estado com base nos valores atuais
  * Controla LEDs indicadores, padrão da matriz e alarme sonoro
  */
 void UpdateIndicators(void)
 {
     uint8_t temp = systemState.temperature;
     uint8_t hum = systemState.humidity;
     uint8_t bri = systemState.brightness;
     
     // Verifica condição crítica alta (LED vermelho + alarme)
     if (temp > TEMP_MEDIUM_MAX || hum > HUMIDITY_MEDIUM_MAX || bri > BRIGHTNESS_MEDIUM_MAX)
     {
         gpio_put(RED_LED, true);
         gpio_put(GREEN_LED, false);
         UpdateDrawing(2);  // Padrão de alerta crítico
         systemState.soundAlert = true;
     }
     // Verifica condição crítica baixa (LED vermelho + alarme)
     else if (temp < TEMP_NORMAL_MIN || hum < HUMIDITY_NORMAL_MIN || bri < BRIGHTNESS_NORMAL_MIN)
     {
         gpio_put(RED_LED, true);
         gpio_put(GREEN_LED, false);
         UpdateDrawing(1);  // Padrão de alerta
         systemState.soundAlert = true;
     }
     // Verifica condição de alerta baixa (LED amarelo)
     else if (temp < TEMP_NORMAL_MAX || hum < HUMIDITY_NORMAL_MAX || bri < BRIGHTNESS_NORMAL_MAX)
     {
         gpio_put(RED_LED, true);
         gpio_put(GREEN_LED, true);
         UpdateDrawing(1);  // Padrão de alerta
         systemState.soundAlert = false;
     }
     // Verifica condição de alerta alta (LED amarelo)
     else if (temp > TEMP_NORMAL_MAX || hum > HUMIDITY_NORMAL_MAX || bri > BRIGHTNESS_NORMAL_MAX)
     {
         gpio_put(RED_LED, true);
         gpio_put(GREEN_LED, true);
         UpdateDrawing(1);  // Padrão de alerta
         systemState.soundAlert = false;
     }
     // Condição normal (LED verde)
     else
     {
         gpio_put(RED_LED, false);
         gpio_put(GREEN_LED, true);
         UpdateDrawing(0);  // Padrão normal
         systemState.soundAlert = false;
     }

     printf("TEMPERATURA %d", temp);
     printf("HUMIDADE %d", hum);
     printf("LUMINOSIDADE %d", bri);
 }