#include <Leds.h>

// Converte uma cor RGB para um formato de 32 bits para a matriz de LEDs
uint32_t RGBMatrix(RGB color)
{
    unsigned char R, G, B;
    R = color.red;
    G = color.green;
    B = color.blue;
    return (G << 24) | (R << 16) | (B << 8);
}

// Desenha um padrão na matriz de LEDs com a cor especificada
void Draw(double *drawing, uint32_t valorLed, refs pio, RGB *color)
{
    // Estrutura de cor temporária para cada LED
    RGB finalColor;

    for (int16_t i = (NUM_PIXELS)-1; i >= 0; i--)
    {
        // Define a cor com base no desenho
        if (drawing[i] == 1.0)
        {
            finalColor = color[0]; // Usa diretamente a cor fornecida
        }
        else if (drawing[i] == 2.0)
        {
            finalColor = color[1]; // Usa diretamente a cor fornecida
        }
        else if (drawing[i] == 3.0)
        {
            finalColor = color[2]; // Usa diretamente a cor fornecida
        }
        else
        {
            // Mantém o LED apagado
            finalColor.red = 0;
            finalColor.green = 0;
            finalColor.blue = 0;
        }

        // Converte a cor para formato 32 bits e envia ao PIO
        valorLed = RGBMatrix(finalColor);
        pio_sm_put_blocking(pio.ref, pio.stateMachine, valorLed);
    }
}

// Retorna um ponteiro para um array de pixels representando um número de 0 a 9
double *Drawing(int drawing)
{
    // Matrizes que representam os números de 0 a 9
    static double drawing000[] = {
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0};

    static double drawing100[] = {
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        1.0, 0.0, 0.0, 0.0, 0.0};

    static double drawing200[] = {
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 0.0};

    static double drawing300[] = {
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        1.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 0.0};

    static double drawing400[] = {
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 0.0};

    static double drawing500[] = {
        1.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 0.0};

    static double drawing510[] = {
        1.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 2.0, 0.0, 0.0};

    static double drawing520[] = {
        1.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 2.0, 0.0, 1.0,
        1.0, 0.0, 2.0, 0.0, 0.0};

    static double drawing530[] = {
        1.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 2.0, 0.0, 0.0,
        0.0, 0.0, 2.0, 0.0, 1.0,
        1.0, 0.0, 2.0, 0.0, 0.0};

    static double drawing540[] = {
        1.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 2.0, 0.0, 1.0,
        1.0, 0.0, 2.0, 0.0, 0.0,
        0.0, 0.0, 2.0, 0.0, 1.0,
        1.0, 0.0, 2.0, 0.0, 0.0};

    static double drawing550[] = {
        1.0, 0.0, 2.0, 0.0, 0.0,
        0.0, 0.0, 2.0, 0.0, 1.0,
        1.0, 0.0, 2.0, 0.0, 0.0,
        0.0, 0.0, 2.0, 0.0, 1.0,
        1.0, 0.0, 2.0, 0.0, 0.0};

    static double drawing551[] = {
        1.0, 0.0, 2.0, 0.0, 0.0,
        0.0, 0.0, 2.0, 0.0, 1.0,
        1.0, 0.0, 2.0, 0.0, 0.0,
        0.0, 0.0, 2.0, 0.0, 1.0,
        1.0, 0.0, 2.0, 0.0, 3.0};

    static double drawing552[] = {
        1.0, 0.0, 2.0, 0.0, 0.0,
        0.0, 0.0, 2.0, 0.0, 1.0,
        1.0, 0.0, 2.0, 0.0, 0.0,
        3.0, 0.0, 2.0, 0.0, 1.0,
        1.0, 0.0, 2.0, 0.0, 3.0};

    static double drawing553[] = {
        1.0, 0.0, 2.0, 0.0, 0.0,
        0.0, 0.0, 2.0, 0.0, 1.0,
        1.0, 0.0, 2.0, 0.0, 3.0,
        3.0, 0.0, 2.0, 0.0, 1.0,
        1.0, 0.0, 2.0, 0.0, 3.0};

    static double drawing554[] = {
        1.0, 0.0, 2.0, 0.0, 0.0,
        3.0, 0.0, 2.0, 0.0, 1.0,
        1.0, 0.0, 2.0, 0.0, 3.0,
        3.0, 0.0, 2.0, 0.0, 1.0,
        1.0, 0.0, 2.0, 0.0, 3.0};

    static double drawing555[] = {
        1.0, 0.0, 2.0, 0.0, 3.0,
        3.0, 0.0, 2.0, 0.0, 1.0,
        1.0, 0.0, 2.0, 0.0, 3.0,
        3.0, 0.0, 2.0, 0.0, 1.0,
        1.0, 0.0, 2.0, 0.0, 3.0};

    // Retorna o desenho correspondente ao número solicitado
    switch (drawing)
    {
    case 100:
        return drawing100;
    case 200:
        return drawing200;
    case 300:
        return drawing300;
    case 400:
        return drawing400;
    case 500:
        return drawing500;
    case 510:
        return drawing510;
    case 520:
        return drawing520;
    case 530:
        return drawing530;
    case 540:
        return drawing540;
    case 550:
        return drawing550;
    case 551:
        return drawing551;
    case 552:
        return drawing552;
    case 553:
        return drawing553;
    case 554:
        return drawing554;
    case 555:
        return drawing555;
    default:
        return drawing000;
    }
}