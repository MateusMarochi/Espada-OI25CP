#include <WiFi.h>
#include <esp_wpa2.h>
#include <esp_wifi.h>
#include <time.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <SSD1306Wire.h>

//#include "secrets.h"

#ifndef SECRETS

//Register your device using MAC address in MSetup
SSD1306Wire display(0x3C, 5, 4);
const char *ssid = "UTFPR-ALUNO"; // Eduroam // MWireless seized all 2.4GHz SSID on 2/25/2020
#define EAP_IDENTITY "a1917463" //enter full umich email address
#define EAP_PASSWORD "fxinuj59"     //your umich password

static const char incommon_ca[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIE6TCCA9GgAwIBAgIUY54OOtwBYGdSWcgE84XTruDFatgwDQYJKoZIhvcNAQEL
BQAwgYExCzAJBgNVBAYTAkJSMQ8wDQYDVQQIDAZQYXJhbmExETAPBgNVBAcMCEN1
cml0aWJhMQ4wDAYDVQQKDAVVVEZQUjEiMCAGCSqGSIb3DQEJARYTZGVpbmZyYUB1
dGZyLmVkdS5icjEaMBgGA1UEAwwRd2lmaS51dGZwci5lZHUuYnIwHhcNMjEwNDIz
MTI0NzE5WhcNMjgwMjI2MTI0NzE5WjCBgTELMAkGA1UEBhMCQlIxDzANBgNVBAgM
BlBhcmFuYTERMA8GA1UEBwwIQ3VyaXRpYmExDjAMBgNVBAoMBVVURlBSMSIwIAYJ
KoZIhvcNAQkBFhNkZWluZnJhQHV0ZnIuZWR1LmJyMRowGAYDVQQDDBF3aWZpLnV0
ZnByLmVkdS5icjCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANxw+3HT
9a7z7EqXHazCU+bUvRw9h4jr9LaZFOwa1m+UcHQMi1gIWd9VshWAhAF6XdCEl37f
wcn4+4mYu86V6wexWUbfcao+7HrQcyIiup8xyemYvBL/eIfSLe6SikwsK8bKQk29
15qwtudwcODJDwdMn3Z/sMtyuCWgs4eYVHMfW90gN/vJdaHN4ScCQKlasU9Q5bRI
Lfe/XJzw2bQoLTLTA1VrjonwDYE/Z/D6tAYt03Dz6H8pN9SqH/jJJI/KG2Bn8hgr
41uGfLQHBNHuPnjuY/4lErDi3n8by+a0dy2keuNIhgRbxgPofEwsw/D9QB52Nz3L
nBgKSahnL8VEFAECAwEAAaOCAVUwggFRMB0GA1UdDgQWBBRYrAeXC6zfNuO0d3cD
+AfKWalldzCBwQYDVR0jBIG5MIG2gBRYrAeXC6zfNuO0d3cD+AfKWalld6GBh6SB
hDCBgTELMAkGA1UEBhMCQlIxDzANBgNVBAgMBlBhcmFuYTERMA8GA1UEBwwIQ3Vy
aXRpYmExDjAMBgNVBAoMBVVURlBSMSIwIAYJKoZIhvcNAQkBFhNkZWluZnJhQHV0
ZnIuZWR1LmJyMRowGAYDVQQDDBF3aWZpLnV0ZnByLmVkdS5icoIUY54OOtwBYGdS
WcgE84XTruDFatgwDwYDVR0TAQH/BAUwAwEB/zBbBgNVHR8EVDBSMFCgTqBMhkpo
dHRwczovL3BvcnRhbC51dGZwci5lZHUuYnIvc2Vydmlkb3Jlcy9zZXJ2aWNvcy90
aS9hcnF1aXZvcy9jcmxfcmFkaXVzLmNybDANBgkqhkiG9w0BAQsFAAOCAQEAB1N6
hQLTC/WLZ7++bR5YxA+4P3JjeOhmpA+3Z3mNNx8M1c2AwL68sl1PAeFwyRd5bG2w
PdB2BF6giwFM3HqAM2SLe090uhqxCcwZSpSfzyrMLGxzGWsK8moIcXaXzUp0cEm3
EdbLkWW3XGCnP88k3u8elLHubJ1ru0p8bz0GhVE/++zVxZCWAteZKcAisPegWxRO
XXJYLFc+DCLYKx/ky3sOuGXH1aWj6FrIkMe1B1mj3kvQ2W+py8D3X66/7APMVRV0
RasmFvf+LgJSLPfeNAJ7tkywbO6vSPVs0Q62cRuP1GajVKXoUXg4pKN2XrMxpNe8
f0EIdM6cACITx2cjdA==
-----END CERTIFICATE-----

)EOF";

// Obtain the latest from :
// https://documentation.its.umich.edu/content/wifi-manually-configuring-wpa2-enterprise-other-wifi-enabled-devices-unsupported-devices
// Use the "Intermediate CA" file: http://www.itcom.itd.umich.edu/downloads/wifi/incommon_ras_server_ca.cer ^^^^^^^^^^^^^^^^^^^^^^^^^^^
// NO need for "Root CA: UserTrust RSA Cerification Authority" file

#endif

const char *ntpServer = "pool.ntp.org";
const char *timezoneEST = "EST5EDT,M3.2.0/2,M11.1.0";
const char *timezoneBR = "BRST+3BRDT+2,M10.3.0,M2.3.0";

uint8_t counter = 0;
unsigned long previousMillisWiFi = 0;
char timeStringBuff[50];
//uint8_t masterCustomMac[] = {0x24, 0x0A, 0xC4, 0x9A, 0xA5, 0xA1}; // 24:0a:c4:9a:a5:a1

void printLocalTime(bool printToSerial = false)
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.print(F("NTP sync failed "));
    return;
  }
  strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
  if(printToSerial) Serial.println(timeStringBuff);
}

void setup()
{
  delay(4000);
  Serial.begin(115200);
  delay(10);

  display.init();
  display.clear();
  //display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(63, 12, "...");
  display.display();
  display.drawString(63, 12, "BORA");
  display.display();
  
  Serial.println();
  WiFi.disconnect(true); //disconnect form wifi to set new wifi connection
  WiFi.mode(WIFI_STA);   //init wifi mode
  //esp_wifi_set_mac(ESP_IF_WIFI_STA, &masterCustomMac[0]);
  Serial.print("MAC >> ");
  Serial.println(WiFi.macAddress());
  Serial.printf("Connecting to WiFi: %s ", ssid);
  esp_wifi_sta_wpa2_ent_set_ca_cert((uint8_t *)incommon_ca, strlen(incommon_ca) + 1);
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
  esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();
  esp_wifi_sta_wpa2_ent_enable(&config);
  Serial.println("start");
  WiFi.begin(ssid);
  Serial.println("end");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(F("."));
    counter++;
    if (counter >= 60)
    { //after 30 seconds timeout - reset board
      ESP.restart();
    }
  }
  Serial.println(F(" connected!"));
  Serial.print(F("IP address set: "));
  Serial.println(WiFi.localIP()); //print LAN IP

  //init and get the time
  configTime(0, 0, ntpServer);
  setenv("TZ", timezoneBR, 1);
  time_t now;
  Serial.print("Obtaining NTP time: ");
  printLocalTime();
  while (now < 1510592825)
  {
    Serial.print(F("."));
    delay(500);
    time(&now);
  }
  Serial.print(F(" success!\nGot Time: "));
  printLocalTime(true);
  Serial.print(F("NTP time received!\n"));
  
}

void loop()
{
  unsigned long currentMillis = millis();

  if (WiFi.status() == WL_CONNECTED)
  {              //if we are connected to Eduroam network
    display.clear();
        display.drawString(63, 12, "VISH");
        display.display();
    counter = 0; //reset counter
    if (currentMillis - previousMillisWiFi >= 15 * 1000)
    {
      printLocalTime(true);
      previousMillisWiFi = currentMillis;
      Serial.print(F("Wifi is still connected with IP: "));
      Serial.println(WiFi.localIP()); //inform user about his IP address
    }
  }
  else if (WiFi.status() != WL_CONNECTED)
  { //if we lost connection, retry
    WiFi.begin(ssid);

    Serial.printf("Connecting to WiFi: %s ", ssid);
    while (WiFi.status() != WL_CONNECTED)
    { //during lost connection, print dots
      delay(500);
      Serial.print(F("."));
      counter++;
      if (counter >= 60)
      { //30 seconds timeout - reset board
        ESP.restart();
      }
    }
    Serial.println(F(" connected!"));
  }

  wifiStrength = getStrength(10); // Faz a media de 10 medicoes da forca do sinal
      
     if(wifiStrength < -70){
      display.clear();
      display.drawString(63, 12, "FRIO");
      display.display();
    }
     
    
    if(wifiStrength >= - 70 && wifiStrength < - 63){
      display.clear();
      display.drawString(63, 12, "MORNO");
      display.display();
    }

    if(wifiStrength >= - 63 && wifiStrength < - 43){
      display.clear();
      display.drawString(63, 12, "QUENTE");
      display.display();
    }

    if(wifiStrength >= - 43){
      display.clear();
      display.drawString(63, 12, "ACHOU");
      display.display();
    }
}

int getStrength(int points){
    long rssi = 0;
    long averageRSSI=0;
     
    for (int i=0;i < points;i++){
        rssi += WiFi.RSSI();
        delay(20);
    }

   averageRSSI=rssi/points;
   return averageRSSI;
}
