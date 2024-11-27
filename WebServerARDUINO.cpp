#include <ESP8266WiFi.h>
#include <Servo.h>

// Configurações do Wi-Fi
const char* ssid = "SEU_SSID";       // Nome da rede Wi-Fi
const char* password = "SUA_SENHA"; // Senha da rede Wi-Fi

WiFiServer server(80); // Porta 80 para o servidor HTTP

Servo motor;          // Objeto para o servo motor
int motorPin = D4;    // Pino do ESP8266 conectado ao servo

// Estado inicial
bool motorMove = false;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Conecta ao Wi-Fi
  Serial.println();
  Serial.print("Conectando ao Wi-Fi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("Wi-Fi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Inicia o servidor
  server.begin();

  // Configura o servo motor
  motor.attach(motorPin);
  motor.write(0); // Posição inicial
}

void loop() {
  // Verifica conexões HTTP
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("Novo cliente conectado");
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Checa o botão pressionado
    if (request.indexOf("/motor") != -1) {
      motorMove = true;
      motor.write(100); // Gira 100 graus
      delay(1000);      // Aguarda a rotação
      motor.write(0);   // Retorna à posição inicial
      motorMove = false;
    }

    // Resposta HTML para o cliente
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<h1>Controle do Motor</h1>");
    client.println("<p><a href=\"/motor\"><button>Girar Motor</button></a></p>");
    client.println("</html>");
    client.stop();
    Serial.println("Cliente desconectado");
  }
}
