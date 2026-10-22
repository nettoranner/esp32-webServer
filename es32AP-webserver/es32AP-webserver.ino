
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32-Access-Point";
const char* password = "12345678";

const int led1 = 26;
const int led2 = 27;

bool led1State = false;
bool led2State = false;

WebServer server(80);

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
      margin: 0;
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
  <h1>ESP32 Control Panel</h1>
  <p>Access Point Mode</p>

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

void handleRoot() {
  server.send(200, "text/html", makePage());
}

void setLED1(bool state) {
  led1State = state;
  digitalWrite(led1, state ? HIGH : LOW);
  handleRoot();
}

void setLED2(bool state) {
  led2State = state;
  digitalWrite(led2, state ? HIGH : LOW);
  handleRoot();
}

void setup() {
  Serial.begin(115200);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);

  WiFi.mode(WIFI_AP);

  bool started = WiFi.softAP(ssid, password);

  if (started) {
    Serial.println("Access Point started!");
    Serial.print("Wi-Fi name: ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("Failed to start Access Point!");
  }

  server.on("/", handleRoot);

  server.on("/led1/on", []() {
    setLED1(true);
  });

  server.on("/led1/off", []() {
    setLED1(false);
  });

  server.on("/led2/on", []() {
    setLED2(true);
  });

  server.on("/led2/off", []() {
    setLED2(false);
  });

  server.onNotFound([]() {
    server.send(404, "text/plain", "404 - Not Found");
  });

  server.begin();
  Serial.println("HTTP server started!");
}

void loop() {
  server.handleClient();
}
