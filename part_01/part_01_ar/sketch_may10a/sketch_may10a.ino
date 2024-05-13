#include <ESP8266WiFi.h>

const char* ssid = "Dialog 4G 906";
const char* password = "1Ab226AD";
const int ledPin = D3;  
WiFiServer server(80);

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("NodeMCU is connected to the WiFi");
  Serial.println(WiFi.localIP());

  server.begin();  // Start the HTTP server
}

void loop() {
  WiFiClient client = server.available();

  if (client) {  // Check if a client has connected
    Serial.println("New client connected");

    // Read the HTTP request headers
    String request = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;

        // Check for end of HTTP request headers
        if (request.endsWith("\r\n\r\n")) {
          break;  // Stop reading once headers are complete
        }
      }
    }

    Serial.println(request);

    // Handle LED control based on HTTP request
    if (request.indexOf("GET /ledon") != -1) {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED is now ON");
    } else if (request.indexOf("GET /ledoff") != -1) {
      digitalWrite(ledPin, LOW);
      Serial.println("LED is now OFF");
    }

    // Send HTTP response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html><head><title>ESP8266 LED Control</title></head><body>");
    client.println("<h1>ESP8266 LED Control</h1>");
    client.println("<p>LED is currently " + String(digitalRead(ledPin) == HIGH ? "ON" : "OFF") + "</p>");
    client.println("<p><a href=\"/ledon\">Turn LED ON</a></p>");
    client.println("<p><a href=\"/ledoff\">Turn LED OFF</a></p>");
    client.println("</body></html>");

    client.stop(); // Close the connection
    Serial.println("Client disconnected");
  }
}
