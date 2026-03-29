# 🌿 Projeto 8: Sistema de Irrigação Inteligente com ESP32

Este projeto consiste em um sistema autônomo de monitoramento de umidade do solo e irrigação automatizada. Utilizando um ESP32, o sistema decide o momento ideal para acionar uma bomba de água, evitando o desperdício e garantindo a saúde da planta.

## 🚀 Funcionalidades
- **Monitoramento em Tempo Real:** Leitura de umidade via sensor capacitivo.
- **Automação com Histerese:** Liga a irrigação abaixo de 30% e desliga acima de 70%.
- **Interface Web:** Servidor interno para controle manual e visualização remota.
- **Display Local:** LCD 16x2 para acompanhamento rápido no local.
- **Modo Manual:** Possibilidade de forçar a irrigação via interface web.

## 🛠️ Hardware Utilizado
- ![Circuito](circuito.png)
- [ESP32 DevKit V1](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-devkit-v1.html)
- Sensor de Umidade de Solo (Capacitivo)
- Módulo Relé 5V (1 Canal)
- Display LCD 16x2 com Módulo I2C
- Mini Bomba de Água DC ou LED (para simulação)
- Potenciômetro (para simulação de umidade no Wokwi)

## 📂 Estrutura do Repositório
- `/src`: Código-fonte principal (.ino)
- `/docs`: Relatório técnico e diagramas.
- `diagram.json`: Arquivo de configuração para simulação no Wokwi.

## 🔧 Como Instalar e Rodar
1. Instale a IDE do Arduino ou use o VS Code com PlatformIO.
2. Instale as bibliotecas necessárias:
   - `LiquidCrystal_I2C`
   - `WiFi` (Nativo)
   - `WebServer` (Nativo)
3. Conecte os componentes conforme o esquema descrito no relatório.
4. Altere as credenciais do Wi-Fi no código (se usar modo Station) ou conecte-se à rede `Projeto_Irrigacao`.
5. Acesse o endereço IP `192.168.4.1` no seu navegador.

## 💻 Simulação Online
Você pode testar este projeto sem hardware físico através do link abaixo:
👉 [Link para o Projeto no Wokwi](https://wokwi.com/projects/459837680685163521)

## 📄 Relatório Técnico
O relatório completo com fluxograma, arquitetura de hardware e detalhes de calibração está disponível na pasta `/docs`.

---
*Projeto desenvolvido para fins acadêmicos com o auxílio de IA (Gemini).*