#include <WiFi.h>
#include <HTTPClient.h>

constexpr const char* SSID = "IMSApublic";
constexpr const char* PASSWORD = "IMSAfall24";
constexpr const char* SERVER_NAME_1 = "http://google.com";
constexpr const char* SERVER_NAME_2 = "http://143.195.93.84:5000/upload_data";
constexpr unsigned long FRAME_LENGTH_MS = 5000;

unsigned long lastMillis = 0;
unsigned long lastTime = 0;
unsigned int passes = 0; // Number of times beam breaker changed state in the current frame
unsigned long timeTriggered = 0; // Amount of time the beam breaker was triggered in the current frame

void setup() {
  Serial.begin(9600);

  WiFi.begin(SSID, PASSWORD);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.print("Timer set to ");
  Serial.print(FRAME_LENGTH_MS);
  Serial.println(" milliseconds");

  lastMillis = millis();
}

void loop() {
  bool measurement = getMeasurement();
  unsigned long deltaTime = millis() - lastMillis;
  timeTriggered += measurement ? deltaTime : 0;

  if ((millis() - lastTime) > FRAME_LENGTH_MS) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      HTTPClient http1;
    
      // Your Domain name with URL path or IP address with path
      http.begin(client, SERVER_NAME_1);
      http1.begin(client, SERVER_NAME_2);
      
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
      
      // Specify content-type header
      http.addHeader("Content-Type", "application/json");
      // Data to send with HTTP POST
      String httpRequestData = "{timestamp: 1741315109,ESP: 2,broken-time: 20.6220,broken-count: 8}";           
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData); 
      
      //posts JSON object through the second http connection
      http1.addHeader("Content-Type", "application/json");
      // Data to send with HTTP POST
      String httpRequestData1 = "{timestamp: 1741315109,ESP: 2,broken-time: 20.6220,broken-count: 8}";           
      // Send HTTP POST request
      int httpResponseCode1 = http1.POST(httpRequestData1);
      
     
      Serial.print("HTTP Response code (Google): ");
      Serial.println(httpResponseCode);
      
      Serial.print("2nd HTTP Response code (Cinna): ");
      Serial.println(httpResponseCode1);
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

/**
 * Returns true if something is blocking the beam breaker.
 */
bool getMeasurement() {
  return false;
}