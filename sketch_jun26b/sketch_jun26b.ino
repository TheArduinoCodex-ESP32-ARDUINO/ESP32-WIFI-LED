 #include <WiFi.h>
#include <WebServer.h>

// 🔧 Change these to your Wi-Fi network credentials
const char* ssid = "YourWiFi";
const char* password = "YourPassword";

// 🟢 LED pin (you can change it)
const int ledPin = 2;

// 🌐 Web server running on port 80
WebServer server(80);

// 🖥️ HTML webpage with ON/OFF buttons
String webpage = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <title>ESP32 LED Control</title>
    <style>
      body { font-family: Arial; text-align: center; margin-top: 50px; }
      button { padding: 20px; font-size: 20px; margin: 10px; }
    </style>
  </head>
  <body>
    <h1>ESP32 LED Control</h1>
    <a href="/on"><button>Turn ON</button></a>
    <a href="/off"><button>Turn OFF</button></a>
  </body>
  </html>
)rawliteral";

// 📩 Handle the root page
void handleRoot() {
  server.send(200, "text/html", webpage);
}

// 💡 Turn LED ON
void handleOn() {
  digitalWrite(ledPin, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

// 💡 Turn LED OFF
void handleOff() {
  digitalWrite(ledPin, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(9600); // 🔧 Use 9600 baud in Serial Monitor
  pinMode(ledPin, OUTPUT);

  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  // ⏳ Wait for connection
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    counter++;
    if (counter > 20) {
      Serial.println("\nFailed to connect to Wi-Fi.");
      return;
    }
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // 🖥️ Start web server
  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  server.handleClient();
}
