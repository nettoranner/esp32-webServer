
#include <WiFi.h>
#include <WebServer.h>

// Your existing Wi-Fi network
const char* ssid = "YOUR_WIFI_NAME";
const char* wifi_password = "YOUR_WIFI_PASSWORD";

// HTTP Authentication credentials
const char* www_username = "admin";
const char* www_password = "12345678";

// LED GPIO pins
const int led1 = 26;
const int led2 = 27;

bool led1State = false;
bool led2State = false;

// HTTP server on port 80
WebServer server(80);

// Check HTTP authentication
bool checkAuth() {
  if (!server.authenticate(www_username, www_password)) {
    server.requestAuthentication();
    return false;
  }
  return true;
}

// Generate web page
String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Control Panel</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      background: #f2f2f2;
      padding: 30px 10px;
    }
    .card {
      background: white;
      max-width: 350px;
      margin: 15px auto;
      padding: 20px;
      border-radius: 12px;
    }
    button {
      padding: 12px 30px;
      border: none;
      border-radius: 8px;
      color: white;
      font-size: 18px;
      cursor: pointer;
    }
    .on { background: #16a34a; }
    .off { background: #dc2626; }
  </style>
</head>
<body>
  <h1>ESP32 Web Server</h1>

  <div class="card">
    <h2>LED 1 (GPIO 26)</h2>
    <p>State: %LED1%</p>
    <a href="/led1/on"><button class="on">ON</button></a>
    <a href="/led1/off"><button class="off">OFF</button></a>
  </div>

  <div class="card">
    <h2>LED 2 (GPIO 27)</h2>
    <p>State: %LED2%</p>
    <a href="/led2/on"><button class="on">ON</button></a>
    <a href="/led2/off"><button class="off">OFF</button></a>
  </div>
</body>
</html>
)rawliteral";

  html.replace("%LED1%", led1State ? "ON" : "OFF");
  html.replace("%LED2%", led2State ? "ON" : "OFF");

  return html;
}

// Main page
void handleRoot() {
  if (!checkAuth()) return;
  server.send(200, "text/html", makePage());
}

// LED 1 handlers
void handleLED1On() {
  if (!checkAuth()) return;
  led1State = true;
  digitalWrite(led1, HIGH);
  server.send(200, "text/html", makePage());
}

void handleLED1Off() {
  if (!checkAuth()) return;
  led1State = false;
  digitalWrite(led1, LOW);
  server.send(200, "text/html", makePage());
}

// LED 2 handlers
void handleLED2On() {
  if (!checkAuth()) return;
  led2State = true;
  digitalWrite(led2, HIGH);
  server.send(200, "text/html", makePage());
}

void handleLED2Off() {
  if (!checkAuth()) return;
  led2State = false;
  digitalWrite(led2, LOW);
  server.send(200, "text/html", makePage());
}

void setup() {
  Serial.begin(115200);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);

  // Connect to existing Wi-Fi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, wifi_password);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi connected!");

  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  // Register routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/led1/on", HTTP_GET, handleLED1On);
  server.on("/led1/off", HTTP_GET, handleLED1Off);
  server.on("/led2/on", HTTP_GET, handleLED2On);
  server.on("/led2/off", HTTP_GET, handleLED2Off);

  server.onNotFound([]() {
    if (!checkAuth()) return;
    server.send(404, "text/plain", "404 - Not Found");
  });

  server.begin();
  Serial.println("HTTP server started!");
}

void loop() {
  server.handleClient();
}