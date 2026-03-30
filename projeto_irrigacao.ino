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
bool modoAutomatico = true; // Trava para permitir controle manual via Web
bool bombaStatus = false;

WebServer server(80);

// --- Interface Web Atualizada ---
void handleRoot() {
  String corStatus = bombaStatus ? "#2ecc71" : "#e74c3c";
  String textoBomba = bombaStatus ? "LIGADA" : "DESLIGADA";
  String textoModo = modoAutomatico ? "Automático" : "Manual (Travado)";

  String html = "<html><head><meta charset='UTF-8'><title>Irrigação Smart</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>body{font-family:sans-serif; text-align:center; background:#f4f4f9; color:#333; padding:20px;}";
  html += ".card{background:white; padding:20px; border-radius:15px; box-shadow:0 4px 6px rgba(0,0,0,0.1); display:inline-block; min-width:300px;}";
  html += ".btn{padding:15px 25px; background:#3498db; color:white; border:none; border-radius:50px; cursor:pointer; font-size:16px; margin-top:10px;}";
  html += ".btn-auto{background:#95a5a6; padding:10px; font-size:12px;}";
  html += ".status{font-size:24px; font-weight:bold; color:" + corStatus + ";}</style>";
  html += "<script>setInterval(() => { location.reload(); }, 3000);</script></head><body>";
  
  html += "<div class='card'>";
  html += "<h1>🌱 Irrigação Smart</h1>";
  html += "<p>Modo atual: <b>" + textoModo + "</b></p>";
  html += "<h2>Umidade: <span style='color:#3498db'>" + String(umidade) + "%</span></h2>";
  html += "<h3>Bomba: <span class='status'>" + textoBomba + "</span></h3>";
  
  html += "<form action='/toggle'><input type='submit' class='btn' value='Alternar Bomba (Manual)'></form>";
  html += "<form action='/auto'><input type='submit' class='btn btn-auto' value='Voltar para Automático'></form>";
  html += "</div></body></html>";
  
  server.send(200, "text/html", html);
}

// --- Rota para Alternar Bomba (Manual) ---
void handleToggle() {
  modoAutomatico = false; // Desativa o automático ao interagir manualmente
  bombaStatus = !bombaStatus;
  digitalWrite(relayPin, bombaStatus ? LOW : HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

// --- Rota para Reativar Automático ---
void handleAuto() {
  modoAutomatico = true;
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Inicia desligado

  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("SISTEMA SMART");
  lcd.setCursor(0, 1);
  lcd.print("CONECTANDO...");

  // Configura como Ponto de Acesso (IP: 192.168.4.1)
  WiFi.softAP("Projeto_Irrigacao", "12345678");
  
  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.on("/auto", handleAuto);
  server.begin();

  Serial.println("Servidor iniciado!");
  lcd.clear();
}

void loop() {
  server.handleClient();

  // Leitura do sensor
  int leitura = analogRead(sensorPin);
  // Mapeamento invertido: Solo seco (4095) -> 0% | Solo úmido (0) -> 100%
  umidade = map(leitura, 4095, 0, 0, 100);
  umidade = constrain(umidade, 0, 100);

  // Lógica Automática (Só roda se o usuário não travou no manual)
  if (modoAutomatico) {
    if (umidade < 30) {
      bombaStatus = true;
    } else if (umidade > 70) {
      bombaStatus = false;
    }
    digitalWrite(relayPin, bombaStatus ? LOW : HIGH);
  }

  // Atualização do LCD
  lcd.setCursor(0, 0);
  lcd.print("Umidade: ");
  lcd.print(umidade);
  lcd.print("%   "); 

  lcd.setCursor(0, 1);
  if (modoAutomatico) {
    lcd.print(bombaStatus ? "STATUS: IRRIGANDO" : "STATUS: AGUARDA ");
  } else {
    lcd.print("MODO: MANUAL    ");
  }

  delay(200); 
}
