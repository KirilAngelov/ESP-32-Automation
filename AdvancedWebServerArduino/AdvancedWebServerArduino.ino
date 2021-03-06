// loading wifi library
#include <WiFi.h>

// change these with your information
const char* ssid = "YOUR_NETWORK_NAME_HERE";
const char* password = "YOUR_NETWORK_PASSWORD_HERE";

// setting port fo web server
WiFiServer server(80);

// variable to store the HTTP request
String header;

// variables to store and dispalay relay state
String RELAY_PIN_STATE = "off";

// variables for pins
const int RELAY_PIN = 4;
const int LED_PIN = 13;

// current time to display in serial monitor
unsigned long currentTime = millis();
//previous time
unsigned long previousTime = 0; 
// this timeout will equal 2 seconds
const long timeoutTime = 2000;

void setup() {
  //sets the data rate in bits per second (baud) for serial data transmission
  Serial.begin(115200);
  // set the variables for the output
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,HIGH);
  digitalWrite(RELAY_PIN, HIGH);
  

  // Connect to Wi-Fi network with credentials
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // here we loop while the client is connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              RELAY_PIN_STATE = "on";
              digitalWrite(RELAY_PIN, LOW);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              RELAY_PIN_STATE = "off";
              digitalWrite(RELAY_PIN, HIGH);
            
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off button
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Wi-Fi Controlled Relay & Led</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>Relay & Led - State " +RELAY_PIN_STATE + "</p>");
            // If the output26State is off, it displays the ON button       
            if (RELAY_PIN_STATE=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
