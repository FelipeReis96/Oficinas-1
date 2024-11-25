#include <WiFi.h>
#include <ESP32Servo.h>

Servo myservo;  // Servo object to control a servo

// GPIO to which the servo is attached
static const int servoPin = 13;

// Replace with your network credentials
const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  myservo.attach(servoPin);  // Attach servo to the GPIO pin

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // Print a message in the serial port
    String currentLine = "";                // Make a string to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // Loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // If there's bytes to read from the client,
        char c = client.read();             // Read a byte, then
        Serial.write(c);                    // Print it out to the serial monitor
        header += c;
        if (c == '\n') {                    // If the byte is a newline character
          // If the current line is blank, you got two newline characters in a row.
          // That's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto; }");
            client.println("button { padding: 20px; font-size: 20px; background-color: #4CAF50; color: white; border: none; border-radius: 5px; cursor: pointer; }");
            client.println("button:hover { background-color: #45a049; }</style>");
            client.println("</head><body><h1>ESP32 Servo Control</h1>");
            client.println("<button onclick=\"location.href='/rotate'\">Rotate 360 Degrees</button>");
            client.println("</body></html>");

            // Handle the /rotate command
            if (header.indexOf("GET /rotate") >= 0) {
              Serial.println("Rotating 360 degrees...");
              myservo.write(180); // Set servo to rotate forward
              delay(2000);       // Allow enough time for a full 360-degree rotation
              myservo.write(90); // Stop servo (90 is neutral for continuous servos)
            }

            // Break out of the while loop
            break;
          } else { // If you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // If you got anything else but a carriage return character,
          currentLine += c;      // Add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
