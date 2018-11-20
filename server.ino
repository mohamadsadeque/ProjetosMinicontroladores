
#include <LiquidCrystal_I2C.h>
#include <Time.h>
#include <TimeLib.h>
#define rotaDateTime "http://api.saiot.ect.ufrn.br/v1/device/history/datetime"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
int screenWidth = 16;  
int screenHeight = 2;  
   
// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);
char dateBuffer[26];
const char* ssid="LII";
const char* password = "wifiLI2Rn";

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
String decodedMsg; 
String tape = "Arduino";
int wait = 250; // In milliseconds
String clearrr = "                 ";
int stringStart, stringStop = 0;  
int scrollCursor = screenWidth;  
int tamanho =0; 
String line1 = ""; 


void handle_msg() {

  server.send(200, "text/html", form);    // Send same page so they can send another msg
  refresh = 1;
  // Display msg on Oled
  String msg = server.arg("msg");
  Serial.println(msg);
  decodedMsg = msg;
  lcd.setCursor(0,0);
  //lcd.print(clearrr);
  line1 = msg;
  //lcd.clear();  
  //lcd.setCursor(0,0);
  //lcd.print(msg);
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
void setup() {
  // put your setup code here, to run once:
  lcd.begin (16,2);
  Serial.begin(9600);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid,password);

  Serial.println();
  Serial.print("Connecting");

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");        
  }

  Serial.println();
  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );
 server.on("/", []() {
    server.send(200, "text/html", form);
  });
  server.on("/msg", handle_msg);                  // And as regular external functions:
  server.begin();                                 // Start the server
  Serial.print("SSID : ");                        // prints SSID in monitor
  Serial.println(ssid);                           // to monitor
  char result[16];
  sprintf(result, "%3d.%3d.%1d.%3d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
  Serial.println();
  Serial.println(result);
  decodedMsg = result;
  Serial.println("WebServer ready!   ");

  Serial.println(WiFi.localIP()); 
 syncTime(getDateNow());
}


void loop() {
  // put your main code here, to run repeatedly:

server.handleClient();  

  lcd.setBacklight(HIGH);
//lcd.print(getDateNow());
showTime();
 lcd.setCursor(scrollCursor, 0);  
  lcd.print(line1.substring(stringStart,stringStop));  
   

  //Quanto menor o valor do delay, mais rapido o scroll  
  delay(250);  
  scroll_sup(); //Chama a rotina que executa o scroll  

  //Verifica o tamanho da string  
  tamanho = line1.length();  
  if (stringStart == tamanho)  
  {  
    stringStart = 0;  
    stringStop = 0;  
  } 
  
}

String getDateNow(){
  HTTPClient http;
  http.begin(rotaDateTime);
  int httpCode = http.GET(); //Retorna o código http, caso não conecte irá retornar -1
  String payload = http.getString();
  http.end();
  if (httpCode != 200) {
    return "0";
  }
  return payload;
};

bool syncTime(String dateTime)
{
  int ano, mes, dia, h, m, s;
  if (dateTime == "0")
  {
    Serial.println("Error data");
    return 0;
  }
  ano = dateTime.substring(1, 5).toInt();
  mes = dateTime.substring(6, 8).toInt();
  Serial.println(mes);
  dia = dateTime.substring(8, 11).toInt();
  h = dateTime.substring(11, 14).toInt();
  m = dateTime.substring(14, 17).toInt();
  s = dateTime.substring(17, 20).toInt();
  setTime(h, m, s, dia, mes, ano);
  return 1;
};

void showTime(){
  sprintf(dateBuffer, "%04u-%02u-%02u %02u:%02u:%02u", year(), month(), day(), hour(), minute(), second());
   lcd.setCursor(0,1);
  lcd.print(dateBuffer);
  
};

void scroll_sup()  
{  
  lcd.clear();  
  if(stringStart == 0 && scrollCursor > 0)
  {  
    scrollCursor--;  
    stringStop++;  
  } else if (stringStart == stringStop){  
    stringStart = stringStop = 0;  
    scrollCursor = screenWidth;  
  } else if (stringStop == line1.length() && scrollCursor == 0) {  
    stringStart++;  
  } else {  
    stringStart++;  
    stringStop++;  
  }  
}  
