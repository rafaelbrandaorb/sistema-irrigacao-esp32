#include <WiFi.h>
#include <WebServer.h>
#include <LiquidCrystal_I2C.h>

// Configurações do LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pinos
const int sensorPin = 34;
const int relayPin = 26;

// Variáveis de controle
int umidade = 0;
bool modoAutomatico = true;
bool bombaStatus = false;

WebServer server(80);

void handleRoot() {
  String html = "<html><head><meta charset='UTF-8'><title>Irrigação Smart</title>";
  html += "<style>body{font-family:sans-serif; text-align:center; background:#f0f0f0;}";
  html += ".btn{padding:15px; background:#2ecc71; color:white; border:none; border-radius:5px; cursor:pointer;}</style>";
  html += "<script>setInterval(() => { location.reload(); }, 3000);</script></head><body>";
  html += "<h1>Sistema de Irrigação</h1>";
  html += "<h2>Umidade Atual: " + String(umidade) + "%</h2>";
  html += "<h3>Status da Bomba: " + String(bombaStatus ? "LIGADA" : "DESLIGADA") + "</h3>";
  html += "<form action='/toggle'><input type='submit' class='btn' value='Alternar Bomba Manual'></form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleToggle() {
  bombaStatus = !bombaStatus;
  digitalWrite(relayPin, bombaStatus ? LOW : HIGH); // LOW costuma ligar o relé
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Inicia desligado (Lógica Inversa)

  lcd.init();
  lcd.backlight();
  
  // Mensagem de Boas-vindas
  lcd.setCursor(0, 0);
  lcd.print("SISTEMA SMART");
  lcd.setCursor(0, 1);
  lcd.print("INICIALIZANDO...");
  delay(2000);
  lcd.clear();

  WiFi.softAP("Projeto_Irrigacao", "12345678");
  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.begin();
}

void loop() {
  server.handleClient();

  // Leitura e mapeamento da umidade
  int leitura = analogRead(sensorPin);
  umidade = map(leitura, 0, 4095, 0, 100);
  umidade = constrain(umidade, 0, 100);

  // Lógica de Controle com Status no LCD
  lcd.setCursor(0, 0);
  lcd.print("Umidade: ");
  lcd.print(umidade);
  lcd.print("%   "); // Espaços extras limpam resíduos de números anteriores

  lcd.setCursor(0, 1);
  if (umidade < 30) {
    digitalWrite(relayPin, LOW); // Liga Relé/LED
    bombaStatus = true;
    lcd.print("STATUS: IRRIGANDO");
  } 
  else if (umidade > 70) {
    digitalWrite(relayPin, HIGH); // Desliga Relé/LED
    bombaStatus = false;
    lcd.print("STATUS:BOMBA(OFF)");
  } 
  else {
    // Mantém o status atual se estiver entre 30% e 70%
    lcd.print("STATUS: ");
    lcd.print(bombaStatus ? "IRRIGANDO" : "AGUARDANDO");
  }

  delay(500); 
}