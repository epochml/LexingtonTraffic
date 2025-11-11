#include <WiFi.h>
#include <HTTPClient.h>

constexpr const char* SSID = "IMSApublic";
constexpr const char* PASSWORD = "IMSAfall24";
constexpr const char* SERVER_NAME_1 = "http://google.com";
//Replace 143.195.82.223:5000 with your IP and port
constexpr const char* SERVER_NAME_2 = "http://143.195.82.223:5000/upload_data";
constexpr unsigned long FRAME_LENGTH_MS = 5000;
unsigned int loopNum = 0;
unsigned long lastMillis = 0;
unsigned long lastTime = 0;
unsigned int passes = 0; // Number of times beam breaker changed state in the current frame
unsigned long timeTriggered = 0; // Amount of time the beam breaker was triggered in the current frame

void setup() {
  Serial.begin(115200);
  Serial.println("Successfully connected to Serial Monitor!");
  WiFi.setHostname("ESP32_Device");
  WiFi.begin(SSID, PASSWORD);
  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000) {
    int code = WiFi.status();
    while (code != 3) {
      code = WiFi.status();
      if ( code == 0) {
      Serial.println("Not connected (0)...");
      } else if (code == 6) {
        Serial.println("Initiating connection (6)...");
      }
      delay(500);
    }
    Serial.println("Connected to wifi!");

  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.print("Timer set to ");
  Serial.print(FRAME_LENGTH_MS);
  Serial.println(" milliseconds");

  lastMillis = millis();
}

void loop() {
  Serial.print("Loop number #");
  Serial.println(loopNum);
  
  bool measurement = getMeasurement();
  unsigned long deltaTime = millis() - lastMillis;
  timeTriggered += measurement ? deltaTime : 0;

  if ((millis() - lastTime) > FRAME_LENGTH_MS) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
    // First request
    {
        WiFiClient client1;
        HTTPClient http;
        http.begin(client1, SERVER_NAME_1);
        http.addHeader("Content-Type", "application/json");
        
        int code = http.GET();
        if (code >0) Serial.println("Able to reach Google!");
        http.end();
    }

    // Second request
    {
        WiFiClient client2;
        HTTPClient http;
        http.begin(client2, SERVER_NAME_2);
        http.addHeader("Content-Type", "application/json");

        String payload = "{timestamp: 1741315109,ESP: 2,broken-time: 20.6220,broken-count: 8}"; 
        http.addHeader("Content-Type", "application/json");
        int code = http.POST(payload);

        Serial.print("2nd HTTP Response code: ");
        Serial.println(code);
        http.end();
    }
}

    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  loopNum +=1;
  delay(1000);
}

/**
 * Returns true if something is blocking the beam breaker.
 */
bool getMeasurement() {
  return false;
}


