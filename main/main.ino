#include "Arduino_LED_Matrix.h"
#include "WiFiS3.h"
#include "arduino_secrets.h"
#include "matrix_page.h" 

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

int status = WL_IDLE_STATUS;
WiFiServer server(80);

ArduinoLEDMatrix matrix;
uint8_t matrixFrame[8][12] = {0};

void setup() {
  Serial.begin(115200);
  
  // Initialize Matrix
  matrix.begin();
  
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
  // Display custom pattern
  matrix.renderBitmap(matrixFrame, 8, 12);

  // Handle Web Server Clients
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
            
            // Serve matrix page
            if (requestLine.indexOf("?data=") >= 0) {
              parseMatrixData(requestLine);
            }
            sendMatrixPage(client);
            
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
// Function to serve matrix control page
void sendMatrixPage(WiFiClient &client) {
  client.print(MATRIX_PAGE);
}

// Parse matrix data from URL
void parseMatrixData(String request) {
  int dataIndex = request.indexOf("?data=");
  if (dataIndex != -1) {
    int endSpace = request.indexOf(" ", dataIndex);
    String matrixData = request.substring(dataIndex + 6, endSpace);
    
    // Decode URL encoding
    matrixData.replace("+", " ");
    matrixData.replace("%2C", ",");
    
    // Parse the matrix data (format: "000000000000,111111111111,...")
    int row = 0;
    int startPos = 0;
    while (row < 8 && startPos < matrixData.length()) {
      int commaPos = matrixData.indexOf(',', startPos);
      String rowData;
      if (commaPos == -1) {
        rowData = matrixData.substring(startPos);
      } else {
        rowData = matrixData.substring(startPos, commaPos);
      }
      
      for (int col = 0; col < 12 && col < rowData.length(); col++) {
        matrixFrame[row][col] = (rowData.charAt(col) == '1') ? 1 : 0;
      }
      
      row++;
      startPos = commaPos + 1;
    }
    
    Serial.println("Matrix updated");
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
