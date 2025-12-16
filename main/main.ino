#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include "WiFiS3.h"
#include "arduino_secrets.h" 

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

int status = WL_IDLE_STATUS;
WiFiServer server(80);

ArduinoLEDMatrix matrix;

// Default message
String currentMessage = " Hello World! ";

void setup() {
  Serial.begin(115200);
  
  // Initialize Matrix
  matrix.begin();
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(100); // Adjust speed
  
  // Check for WiFi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  // Attempt to connect to WiFi
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(5000); // wait 5 seconds for connection
  }

  server.begin();
  printWifiStatus();
}

void loop() {
  // 1. Scroll the text (This uses ArduinoGraphics)
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFFFF);
  matrix.println(currentMessage);
  matrix.endText(SCROLL_LEFT);
  matrix.endDraw();

  // 2. Handle Web Server Clients
  WiFiClient client = server.available();
  if (client) {
    String currentLine = "";
    String requestLine = ""; // To store the GET request
    boolean isFirstLine = true;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        // Capture the first line of the request (GET /?msg=...)
        if (isFirstLine) {
          requestLine += c;
          if (c == '\n') isFirstLine = false; 
        }

        if (c == '\n') {
          if (currentLine.length() == 0) {
            // HTTP Headers
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Send the Web Page
            sendWebPage(client);
            
            // Check if there was a message in the URL
            parseMessage(requestLine);
            
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
  }
}

// Function to serve HTML/CSS
void sendWebPage(WiFiClient &client) {
  client.println("<!DOCTYPE HTML>");
  client.println("<html><head><meta name='viewport' content='width=device-width, initial-scale=1'>");
  client.println("<style>");
  client.println("body { font-family: sans-serif; text-align: center; background-color: #222; color: #fff; padding: 50px; }");
  client.println("input { padding: 10px; font-size: 1.2rem; width: 80%; border-radius: 5px; border: none; }");
  client.println("button { padding: 10px 20px; font-size: 1.2rem; background: #00979d; color: white; border: none; border-radius: 5px; cursor: pointer; margin-top: 10px; }");
  client.println("</style></head>");
  client.println("<body>");
  client.println("<h1>R4 WiFi Matrix</h1>");
  client.println("<form action='/' method='GET'>");
  client.println("<input type='text' name='msg' placeholder='Type a message...' maxlength='50'>");
  client.println("<br><button type='submit'>Send to Matrix</button>");
  client.println("</form>");
  client.println("</body></html>");   
}

// Simple parsing logic to extract ?msg=Value
void parseMessage(String request) {
  int msgIndex = request.indexOf("?msg=");
  if (msgIndex != -1) {
    int endSpace = request.indexOf(" ", msgIndex);
    String rawMsg = request.substring(msgIndex + 5, endSpace);
    
    // Replace URL encoded spaces (+) with real spaces
    rawMsg.replace("+", " ");
    // Basic URL decode (optional, for %20)
    rawMsg.replace("%20", " ");
    
    // Set the global message to be scrolled
    // Add spaces for smooth scrolling loops
    currentMessage = "   " + rawMsg + "   "; 
    Serial.println("New Message: " + currentMessage);
  }
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println("To view, open your browser to http://" + String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]));
}
