#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "IMSApublic";
const char* password = "IMSAfall24";

//Your Domain name with URL path or IP address with path
const char* serverName1 = "http://google.com";
const char* serverName2 = "http://143.195.93.84:5000/upload_data";
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      HTTPClient http1;
    
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName1);
      http1.begin(client, serverName2);
      
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
