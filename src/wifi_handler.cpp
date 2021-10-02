#include <build_defs.h>

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
int timeout = 200;       // seconds to run for

WiFiManager wm;
extern const char *host;

#include <ESPmDNS.h>

bool pwanl_wifiinit()
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









const char* root_ca= \
"-----BEGIN CERTIFICATE-----\n" \
"MIICiTCCAg+gAwIBAgIQH0evqmIAcFBUTAGem2OZKjAKBggqhkjOPQQDAzCBhTEL\n" \
"MAkGA1UEBhMCR0IxGzAZBgNVBAgTEkdyZWF0ZXIgTWFuY2hlc3RlcjEQMA4GA1UE\n" \
"BxMHU2FsZm9yZDEaMBgGA1UEChMRQ09NT0RPIENBIExpbWl0ZWQxKzApBgNVBAMT\n" \
"IkNPTU9ETyBFQ0MgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMDgwMzA2MDAw\n" \
"MDAwWhcNMzgwMTE4MjM1OTU5WjCBhTELMAkGA1UEBhMCR0IxGzAZBgNVBAgTEkdy\n" \
"ZWF0ZXIgTWFuY2hlc3RlcjEQMA4GA1UEBxMHU2FsZm9yZDEaMBgGA1UEChMRQ09N\n" \
"T0RPIENBIExpbWl0ZWQxKzApBgNVBAMTIkNPTU9ETyBFQ0MgQ2VydGlmaWNhdGlv\n" \
"biBBdXRob3JpdHkwdjAQBgcqhkjOPQIBBgUrgQQAIgNiAAQDR3svdcmCFYX7deSR\n" \
"FtSrYpn1PlILBs5BAH+X4QokPB0BBO490o0JlwzgdeT6+3eKKvUDYEs2ixYjFq0J\n" \
"cfRK9ChQtP6IHG4/bC8vCVlbpVsLM5niwz2J+Wos77LTBumjQjBAMB0GA1UdDgQW\n" \
"BBR1cacZSBm8nZ3qQUfflMRId5nTeTAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/\n" \
"BAUwAwEB/zAKBggqhkjOPQQDAwNoADBlAjEA7wNbeqy3eApyt4jf/7VGFAkK+qDm\n" \
"fQjGGoe9GKhzvSbKYAydzpmfz1wPMOG+FDHqAjAU9JM8SaczepBGR7NjfRObTrdv\n" \
"GDeAU/7dIOA1mjbRxwG55tzd8/8dLDoWV9mSOdY=\n" \
"-----END CERTIFICATE-----\n";

#include <HTTPClient.h>
String serverName = "http://192.168.1.106:1880/update-sensor";
void pwanl_sync()
{
  HTTPClient http;

  http.begin(serverName.c_str()); //Specify the URL and certificate
  int httpCode = http.GET();                                                  //Make the request

  if (httpCode > 0)
  { //Check for the returning code

    String payload = http.getString();
    DBG.println(httpCode);
    DBG.println(payload);
  }

  else
  {
    Serial.println("Error on HTTP request");
  }

  DBG.end(); //Free the resources
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