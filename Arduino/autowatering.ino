#include <WiFiClientSecure.h>
#include <DHT.h>
#include <HTTPClient.h>

#define DHTPIN 27
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

String h, t;
String MAC;

const char* ssid = "****";
const char* password = "****";

//Your Domain name with URL path or IP address with path
const char* serverName = "https://autowatering.leomirco.repl.co/post";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 8000;

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  MAC = WiFi.macAddress();

  Serial.println("Timer set to 8 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  delay(8000);
  h = dht.readHumidity();
  t = dht.readTemperature();
  Serial.print("Umidità: ");
  Serial.print(h);
  Serial.print(" %, Temperatura: ");
  Serial.print(t);
  Serial.println(" Celsius");

  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClientSecure client;
      HTTPClient http;
      client.setInsecure(); //Per bypassare SSL
      http.begin(client, serverName);

      // If you need an HTTP request with a content type: application/json, use the following:
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST("{\"mac\":\"" + MAC + "\",\"temp\":\"" + t + "\",\"hum\":\"" + h + "\"}");

      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}