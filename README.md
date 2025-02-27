# BitIrrigate - Simulador de Sistema de Irrigação Inteligente com RP2040 | BitDogLab

## 🎥 Vídeo de Demonstração

🔗 Link para o vídeo de demonstração:https://youtu.be/Jjb4iYp_kxw

## 📌 Descrição

Este projeto, desenvolvido por **Hilquias Rodrigues de Oliveira**, utiliza o microcontrolador **RP2040** para monitorar e controlar variáveis ambientais, incluindo **temperatura, umidade e luminosidade**. Além disso, emprega um **display OLED SSD1306** para exibição de informações em tempo real, uma **matriz de LEDs RGB** para indicadores visuais e um **joystick analógico** para ajuste dos controles.

Os botões físicos e o botão do joystick são gerenciados por **interrupções (IRQ)** com **debouncing via software**, garantindo maior precisão nas leituras.

## ✅ Funcionalidades

- **Monitoramento de Temperatura, Umidade e Luminosidade:**

  - Exibição dos valores no **display OLED SSD1306**.
  - Controle e ajuste das variáveis via **joystick** e **botões físicos**.

- **Controle de LEDs RGB para Indicadores Visuais:**

  - Mudança de cor e brilho proporcional aos valores lidos.
  - Controle via **PWM**, permitindo transições suaves de brilho.

- **Interação via Joystick:**

  - Ajuste de luminosidade e controle das variáveis ambientais.
  - Movimentação de elementos gráficos no **display OLED**.

- **Botão do Joystick (GPIO 22):**

  - Alterna o estado do alerta sonoro.
  - Modifica elementos gráficos na interface do **display**.

- **Botão A (GPIO 5):**

  - Ativa ou desativa o controle PWM dos LEDs RGB.

## 🔧 Componentes Utilizados

- 🟢 **Matriz de LEDs RGB** (GPIOs 11, 12, 13)
- 🌡 **Sensor de Temperatura e Umidade**
- 🎮 **Joystick Analógico** (ADC nos GPIOs 26 e 27)
- 🟩 **Botão do Joystick** (GPIO 22)
- 🅰 **Botão A** (GPIO 5)
- 📟 **Display SSD1306 OLED** (I2C - GPIOs 14 e 15)
- 🔊 **Buzzer** para alertas sonoros

## 🛠 Aspectos Técnicos do Projeto

✔️ Implementação de **interrupções (IRQ)** para os botões físicos
✔️ **Debouncing via software** para evitar leituras falsas
✔️ **Controle de LEDs via PWM** para transições suaves de brilho
✔️ **Conversão A/D (ADC)** para capturar valores do joystick
✔️ **Comunicação I2C** para exibição de dados no **display OLED**
✔️ **Código estruturado e comentado**

## 🚀 Como Executar

### ⚙️ Pré-requisitos:

- **Pico SDK instalado**
- **Extensões Raspberry Pi Pico, CMake e C/C++ no VSCode**

### 📌 Passos:

1️⃣ Clone este repositório e abra a pasta do projeto no **VSCode**.
2️⃣ A extensão **Pi Pico** criará automaticamente a pasta `build`.
3️⃣ Clique em **Compile** na barra inferior do **VSCode**.
4️⃣ Verifique se o arquivo `.uf2` foi gerado corretamente na pasta `build`.
5️⃣ Conecte a **BitDogLab** via USB e coloque-a em modo **BOOTSEL**.
6️⃣ Arraste o arquivo `.uf2` para a unidade de armazenamento da placa.
7️⃣ O código será carregado e executado automaticamente.

### 🎮 Interação com o Sistema:

- **Mova o joystick** para ajustar os valores ambientais e os LEDs RGB.
- **Pressione o botão do joystick** para alternar o alerta sonoro e modificar a interface no **display OLED**.
- **Pressione o Botão A** para ativar ou desativar o controle **PWM dos LEDs RGB**.

---

📌 **Desenvolvido por:** Hilquias Rodrigues de Oliveira

