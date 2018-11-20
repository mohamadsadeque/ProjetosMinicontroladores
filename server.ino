#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <LiquidCrystal_I2C.h>
#define SSID "LII"                      // insert your SSID
#define PASS "wifiLI2Rn"                    // insert your password
#include <Wire.h>
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);
// ******************* String form to sent to the client-browser ************************************
String form =
  "<p>"
  "<head><title>LCDWifi</title>"
  "<center>"
  "<h1>LCD Wi-Fi</h1>"
  "</center>"
  "<form action='msg'><p>Escreva o aviso <input type='text' name='msg' size=100 autofocus> <input type='submit' value='Submit'></form>"
  "</center>";

ESP8266WebServer server(80);                             // HTTP server will listen at port 80
long period;
int offset = 1, refresh = 0;
int pinCS = 0; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 8;
int numberOfVerticalDisplays = 1;
String decodedMsg;
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

String tape = "Arduino";
int wait = 250; // In milliseconds
String clearrr = "                 ";
int spacer = 2;
int width = 5 + spacer; // The font width is 5 pixels

/*
  handles the messages coming from the webbrowser, restores a few special characters and
  constructs the strings that can be sent to the oled display
*/
void handle_msg() {

  matrix.fillScreen(LOW);
  server.send(200, "text/html", form);    // Send same page so they can send another msg
  refresh = 1;
  // Display msg on Oled
  String msg = server.arg("msg");
  Serial.println(msg);
  decodedMsg = msg;
  lcd.setCursor(0,0);
  lcd.print(clearrr);
  lcd.setCursor(0,0);
  lcd.print(msg);
  delay(2000);
  // Restore special characters that are misformed to %char by the client browser
  decodedMsg.replace("+", " ");
  decodedMsg.replace("%21", "!");
  decodedMsg.replace("%22", "");
  decodedMsg.replace("%23", "#");
  decodedMsg.replace("%24", "$");
  decodedMsg.replace("%25", "%");
  decodedMsg.replace("%26", "&");
  decodedMsg.replace("%27", "'");
  decodedMsg.replace("%28", "(");
  decodedMsg.replace("%29", ")");
  decodedMsg.replace("%2A", "*");
  decodedMsg.replace("%2B", "+");
  decodedMsg.replace("%2C", ",");
  decodedMsg.replace("%2F", "/");
  decodedMsg.replace("%3A", ":");
  decodedMsg.replace("%3B", ";");
  decodedMsg.replace("%3C", "<");
  decodedMsg.replace("%3D", "=");
  decodedMsg.replace("%3E", ">");
  decodedMsg.replace("%3F", "?");
  decodedMsg.replace("%40", "@");
  //Serial.println(decodedMsg);                   // print original string to monitor
 //Serial.println(' ');                          // new line in monitor
}

void setup(void) {
 lcd.begin (16,2);
  Serial.begin(9600);
  //ESP.wdtDisable();                               // used to debug, disable wachdog timer,
  Serial.begin(9600);                           // full speed to monitor
  Serial.println("Estou vivo");
  WiFi.begin(SSID, PASS);                         // Connect to WiFi network
  while (WiFi.status() != WL_CONNECTED) {         // Wait for connection
    delay(500);
    Serial.print(".");
  }
 
  // Set up the endpoints for HTTP server,  Endpoints can be written as inline functions:
  server.on("/", []() {
    server.send(200, "text/html", form);
  });
  server.on("/msg", handle_msg);                  // And as regular external functions:
  server.begin();                                 // Start the server

  Serial.print("SSID : ");                        // prints SSID in monitor
  Serial.println(SSID);                           // to monitor

  char result[16];
  sprintf(result, "%3d.%3d.%1d.%3d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
  Serial.println();
  Serial.println(result);
  decodedMsg = result;
  Serial.println("WebServer ready!   ");

  Serial.println(WiFi.localIP());                 // Serial monitor prints localIP
  Serial.print(analogRead(A0));

}


void loop(void) {
server.handleClient();  
 delay(wait);
}
