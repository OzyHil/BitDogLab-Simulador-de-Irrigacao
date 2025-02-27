#include <Leds.h>
 
 /**
  * Converte uma cor RGB para o formato de 32 bits utilizado pela matriz de LEDs
  * O formato segue a ordem: G (8 bits) | R (8 bits) | B (8 bits) | 0 (8 bits)
  * 
  * @param color Estrutura RGB contendo os componentes da cor
  * @return Valor de 32 bits codificado para a matriz de LEDs
  */
 uint32_t RGBMatrix(RGB color) {
     // Extrai os componentes de cor
     unsigned char R = color.red;
     unsigned char G = color.green;
     unsigned char B = color.blue;
     
     // Combina os componentes no formato G|R|B|0
     return (G << 24) | (R << 16) | (B << 8);
 }
 
 /**
  * Desenha um padrão na matriz de LEDs utilizando as cores especificadas
  * 
  * @param drawing Ponteiro para o array que representa o padrão a ser desenhado
  * @param valorLed Variável para armazenar o valor de cada LED
  * @param pio Referência ao controlador PIO e máquina de estado
  * @param color Array de estruturas RGB contendo as cores a serem utilizadas
  */
 void Draw(double *drawing, uint32_t valorLed, refs pio, RGB *color) {
     // Estrutura de cor temporária para cada LED
     RGB finalColor;
     
     // Percorre a matriz de LEDs de trás para frente (conforme protocolo)
     for (int16_t i = (NUM_PIXELS-1); i >= 0; i--)
     {
         // Define a cor com base no valor no array drawing
         switch ((int)drawing[i]) {
             case 1:
                 finalColor = color[0]; // Primeira cor definida
                 break;
             case 2:
                 finalColor = color[1]; // Segunda cor definida
                 break;
             case 3:
                 finalColor = color[2]; // Terceira cor definida
                 break;
             default:
                 // LED apagado (preto)
                 finalColor.red = 0;
                 finalColor.green = 0;
                 finalColor.blue = 0;
                 break;
         }
         
         // Converte a cor para formato 32 bits e envia ao PIO
         valorLed = RGBMatrix(finalColor);
         pio_sm_put_blocking(pio.ref, pio.stateMachine, valorLed);
     }
 }
 
 /**
  * Retorna um ponteiro para um array que representa um padrão de desenho
  * Os valores no array determinam qual cor será utilizada:
  * 0 = desligado, 1-3 = índice da cor no array de cores
  * 
  * @param pattern Código do padrão desejado
  * @return Ponteiro para o array do padrão
  */
 double *Drawing(int pattern) {
     // Matrizes 5x5 que representam diferentes padrões
     
     // Padrão 0 - Matriz vazia (todos LEDs apagados)
     static double pattern0[] = {
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0
     };
     
     // Padrão 1 - Linha inferior acesa
     static double pattern1[] = {
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0
     };
     
     // Padrão 2 - Matriz cheia (todos LEDs acesos)
     static double pattern2[] = {
         0.0, 0.0, 0.0, 0.0, 0.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0
     };
          
     // Retorna o padrão correspondente ao código solicitado
     switch (pattern) {
         case 1:
             return pattern1;
         case 2:
             return pattern2;
         default:
             return pattern0;
     }
 }