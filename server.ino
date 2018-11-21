
#include <LiquidCrystal_I2C.h>
#include <Time.h>
#include <TimeLib.h>
#define rotaDateTime "http://api.saiot.ect.ufrn.br/v1/device/history/datetime"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#define n_msg_max 5 // numero de mensagens maxima
int screenWidth = 16;
int screenHeight = 2;
int n_msg = 0; // numero de mensagens
volatile int pos = 0; // posição exibida
int n_msg_exibidas = 0;

// Inicializa o display no endereco 0x3F
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
char dateBuffer[26];
const char* ssid = "LII";
const char* password = "wifiLI2Rn";
String fila[n_msg_max];
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
int tamanho = 0;
String line1 = "";
unsigned long int UltimaVez1 = 0;
unsigned long int UltimaVez2 = 0;
unsigned long int UltimaVez3 = 0;
void handle_msg() {

  server.send(200, "text/html", form);    // Send same page so they can send another msg
  refresh = 1;
  // Display msg on Oled
  String msg = server.arg("msg");
  Serial.println(msg);
  decodedMsg = msg;
  lcd.setCursor(0, 0);
  fila[n_msg] = msg;
  n_msg++;
  if (n_msg >= n_msg_max) {
    n_msg = 0;
  }
  if (n_msg_exibidas < n_msg_max)
      n_msg_exibidas++;

  delay(200);
  
}

void setup() {
  // put your setup code here, to run once:

  pinMode(D5,INPUT_PULLUP);
  pinMode(D6,INPUT_PULLUP);
  pinMode(D7,INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(D7), exclui_msg, FALLING);
 attachInterrupt(digitalPinToInterrupt(D5), incrementopos, FALLING);
  attachInterrupt(digitalPinToInterrupt(D6), decrementopos, FALLING);
  lcd.begin (16, 2);
  Serial.begin(9600);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting");

  while ( WiFi.status() != WL_CONNECTED ) {
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
  server.on("/msg", handle_msg);
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


  server.handleClient();
  lcd.setBacklight(HIGH);
  showTime();
  lcd.setCursor(scrollCursor, 0);
  lcd.print(fila[pos].substring(stringStart, stringStop));
  delay(250);
  scroll_sup(pos); //Chama a rotina que executa o scroll
  //Verifica o tamanho da string
  tamanho = fila[pos].length();
  if (stringStart == tamanho)
  {
   incrementopos();
  }

}


String getDateNow() {
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

void showTime() {
  sprintf(dateBuffer, "%04u-%02u-%02u %02u:%02u:%02u", year(), month(), day(), hour(), minute(), second());
  lcd.setCursor(0, 1);
  lcd.print(dateBuffer);

};

void scroll_sup(int n)
{
  lcd.clear();
  if (stringStart == 0 && scrollCursor > 0)
  {
    scrollCursor--;
    stringStop++;
  } else if (stringStart == stringStop) {
    stringStart = stringStop = 0;
    scrollCursor = screenWidth;
  } else if (stringStop == fila[n].length() && scrollCursor == 0) {
    stringStart++;
  } else {
    stringStart++;
    stringStop++;
  }
}

void espaco_fila(String msg) {
  for (int i = 0; i < 5; i++) {
    if (fila[i] == "") {
      fila[i] = msg;
      break;
    }
  }
}

void exclui_msg() {
  if(millis() - UltimaVez3 > 250){
  fila[pos] = "";
  UltimaVez3 = millis();
  }
}

void incrementopos(){
  if(millis() - UltimaVez1 > 250){
  scrollCursor = 16;
  stringStart = 0;
    stringStop = 0;
    if (pos < n_msg_exibidas-1) {
      pos++;
    }
    else {
      pos = 0;
    }
  }
 UltimaVez1 = millis();
}

 void decrementopos(){
  if(millis() - UltimaVez2 > 250){
  scrollCursor = 16;
  stringStart = 0;
    stringStop = 0;
    if (pos > 0) {
      pos--;
    }
    else {
      pos = (n_msg_exibidas)-1;
    }
    UltimaVez2 = millis();
  }
  }
