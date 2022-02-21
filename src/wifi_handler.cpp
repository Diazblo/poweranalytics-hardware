#include <build_defs.h>
#include <wifi_handler.h>

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
int timeout = 200;       // seconds to run for

WiFiManager wm;
extern const char *host;

// #include <ESPmDNS.h>
#include <powermonitor.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

uint16_t chipid;
void get_api_key(){
  for(int i=0; i<17; i=i+8) {
	  chipid |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
	}
}
bool wifi_init()
{
  get_api_key();
  WiFi.mode(WIFI_STA);
  WiFi.begin();
  delay(1000);
  // MDNS.begin(host);


  uint8_t wait_counter = 0;
  while (WiFi.status() != WL_CONNECTED && (wait_counter++) < 20)
  {
    delay(500);
    DBG.print("Trying to connect : ");
    DBG.println(WiFi.SSID());
  }
  DBG.println();
  // MDNS.begin(host);
  if (WiFi.status() != WL_CONNECTED)
  {
    DBG.println("Failed to connect. Please enter config mode to enter correct credentials");
    return 0;
  }
  else
  {
    DBG.println(WiFi.localIP());
    server_init();
    return 1;
  }
}

void start_config_portal()
{
  DBG.println("Initiating config portal");
  // fsserver_end();
  digitalWrite(red_led, LOW);
  digitalWrite(green_led, LOW);
  delay(3000);

  // set configportal timeout
  wm.setConfigPortalTimeout(timeout);

  //Start config portal
  if (!wm.startConfigPortal("", ""))
  {
    DBG.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }
  digitalWrite(red_led, HIGH);
  digitalWrite(green_led, HIGH);
  //if you get here you have connected to the WiFi
  DBG.println("connected...)");
}


// wifi_handler::wifi_handler(void)
// {
//   // powermonitorno = 0;
// }

void wifi_handler::pwanl_sync()
{	
  
  // powermonitor_sync_data_add(pwanl.instpower);

	WiFiClient client;
	HTTPClient http;

	// Begin
	http.begin(client, serverName);

	http.addHeader("Content-Type", "application/x-www-form-urlencoded");
	
  // Encode data from array
  String httpRequestData = "apikey=" + String(chipid);
  for(uint8_t i=0; i<powermonitorno; i++){
    httpRequestData += "&" + powermonitor_sync_data_name[i] + "=" + String(*powermonitor_sync_ptr[i],3);
  }

	// Send HTTP POST request
	int httpResponseCode = http.POST(httpRequestData);

	// Free resources
	http.end();

  // Debug
  V_PRINT("HTTP Request data: ");
	V_PRINTln(httpRequestData);
	V_PRINT("HTTP Response code: ");
	V_PRINTln(httpResponseCode);

}


//********************Server**************************
#include <WebServer.h>
#include <Update.h>
#include <ESPmDNS.h>
const char* host = "poweranalytics";
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
WebServer server(80);

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void server_init()
{
  server.onNotFound(handleNotFound);

  server.on("/", HTTP_GET, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", serverIndex);
    });
    server.on("/update", HTTP_POST, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    }, []() {
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        DBG.setDebugOutput(true);
        DBG.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin()) { //start with max available size
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
          DBG.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
        DBG.setDebugOutput(false);
      }
    });

  server.begin();
  MDNS.begin(host);
}
void server_loop()
{
  server.handleClient();
}


/*------------------------------------------------------------------------------------------------
// #include <ESPmDNS.h>
#include <SPIFFS.h>
#else 
#error Wrong board
#endif

const char* host = "poweranalytics";
WiFiManager wm;

#include "FS.h"


extern void fsserver_end();

void start_config_portal() {
  DBG.println("Initiating config portal");
  fsserver_end();
  digitalWrite(red_led, LOW);
  digitalWrite(green_led, LOW);
  delay(3000);


  // set configportal timeout
  wm.setConfigPortalTimeout(timeout);
  
  if (!wm.startConfigPortal("", "12345678")) {
    DBG.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }
  digitalWrite(red_led, HIGH);
  digitalWrite(green_led, HIGH);
  //if you get here you have connected to the WiFi
  DBG.println("connected...)");
}

  httpRequestData = "api_key=" + String(32687632) + "&field1=" + String(random(50));   
bool connection_message_not_sent = 0;
unsigned long wifi_reconnect_timer = 0;
#define WIFI_RECONNECT_DELAY 5000
void keep_wifi_alive() {
  MDNS.update();
  if ( WiFi.status() == WL_CONNECTED && connection_message_not_sent) {
    DBG.print("Connected : ");
    DBG.println(WiFi.SSID());
    connection_message_not_sent = 0;
  } else if ( WiFi.status() != WL_CONNECTED && (millis() - wifi_reconnect_timer) > WIFI_RECONNECT_DELAY) {
    digitalWrite(red_led, LOW);
    connection_message_not_sent = 1;
    wifi_reconnect_timer = millis();
    DBG.print("Host WiFi Disconnected.\nTrying to reconnect : ");
    DBG.println(WiFi.SSID());
    WiFi.begin();
    delay(1000);
    digitalWrite(red_led, HIGH);

  }
}


*/