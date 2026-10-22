#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi settings
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

const int output26 = 26;
const int output27 = 27;

String output26State = "off";
String output27State = "off";

// Create a HTTP-server on port 80
WebServer server(80);

// Main page
void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<style>";
  html += "body{font-family:Arial;text-align:center;margin-top:40px;}";
  html += ".button{background:#4CAF50;border:none;color:white;";
  html += "padding:16px 40px;text-decoration:none;font-size:24px;}";
  html += ".button2{background:#555;}";
  html += "</style></head><body>";
  html += "<h1>ESP32 Web Server</h1>";

  // Controls GPIO 26
  html += "<p>GPIO 26 - State: " + output26State + "</p>";

  if (output26State == "off") {
    html += "<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>";
  } else {
    html += "<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>";
  }

  // Controls GPIO 27
  html += "<p>GPIO 27 - State: " + output27State + "</p>";

  if (output27State == "off") {
    html += "<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>";
  } else {
    html += "<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>";
  }

  html += "</body></html>";
  server.send(200, "text/html", html);
}

// On GPIO 26
void handleGPIO26On() {
  output26State = "on";
  digitalWrite(output26, HIGH);
  handleRoot();
}

// Off GPIO 26
void handleGPIO26Off() {
  output26State = "off";
  digitalWrite(output26, LOW);
  handleRoot();
}

// On GPIO 27
void handleGPIO27On() {
  output27State = "on";
  digitalWrite(output27, HIGH);
  handleRoot();
}

// Off GPIO 27
void handleGPIO27Off() {
  output27State = "off";
  digitalWrite(output27, LOW);
  handleRoot();
}

void setup() {
  Serial.begin(115200);

  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);

  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);

  // Connecting to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Configure endpoints
  server.on("/", handleRoot);
  server.on("/26/on", handleGPIO26On);
  server.on("/26/off", handleGPIO26Off);
  server.on("/27/on", handleGPIO27On);
  server.on("/27/off", handleGPIO27Off);

  // Run server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
