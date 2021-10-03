#include <build_defs.h>
#include <wifi_handler.h>

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
int timeout = 200;       // seconds to run for

WiFiManager wm;
extern const char *host;

#include <ESPmDNS.h>
#include <powermonitor.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>


bool wifi_init()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin();
  delay(1000);
  MDNS.begin(host);

  DBG.print("Trying to connect : ");
  DBG.print(WiFi.SSID());
  DBG.print("   ::");
  uint8_t wait_counter = 0;
  while (WiFi.status() != WL_CONNECTED && (wait_counter++) < 20)
  {
    delay(500);
    DBG.print(".");
  }
  DBG.println();
  MDNS.begin(host);
  if (WiFi.status() != WL_CONNECTED)
  {
    DBG.println("Failed to connect. Please enter config mode to enter correct credentials");
    return 0;
  }
  else
  {
    DBG.println(WiFi.localIP());
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

	// Your Domain name with URL path or IP address with path
	http.begin(client, serverName);

	// Specify content-type header
	// http.addHeader("Content-Type", "application/x-www-form-urlencoded");
	http.addHeader("Content-Type", "application/json");
	// Data to send with HTTP POST

	String httpRequestData = "{";
    httpRequestData += "\"api_key\":\"";
    httpRequestData += String(4658464) + "\",";

		
		for(uint8_t i=0; i<powermonitorno; i++){
			httpRequestData += "\"" + String(powermonitor_sync_data_name[i][10]) + "\":\"" + String(*powermonitor_sync_ptr[i]) + "\",";
    	}

    httpRequestData += "\"cpu\":\"";
    httpRequestData += String(random(100)) + "\"";
    httpRequestData += "}";

  Serial.print("HTTP Request data: ");
	Serial.println(httpRequestData);
	// Send HTTP POST request
	int httpResponseCode = http.POST(httpRequestData);

	Serial.print("HTTP Response code: ");
	Serial.println(httpResponseCode);

	// Free resources
	http.end();
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