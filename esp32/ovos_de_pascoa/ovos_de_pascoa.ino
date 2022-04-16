#include <WiFiUdp.h>
#include <Wire.h>
#include <SSD1306Wire.h>
#include <WiFi.h>
 
// informacoes da rede
const char *ssid = "Mateus";
const char *senha = "????????";
 
long wifiStrength;
 
//configuracoes do display
SSD1306Wire display(0x3C, 5, 4);
 
void setup() {
  // put your setup code here, to run once:
 
    Serial.begin(115200);
 
    display.init();
    display.clear();
    //display.flipScreenVertically();
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(63, 12, "...");
    display.display();
    connectWiFi();
    display.drawString(63, 12, "BORA");
    display.display();
}
 
 
void loop() {
   // Sempre verifica se esta conectado
    if (WiFi.status() != WL_CONNECTED) { 
        connectWiFi();
        display.clear();
        display.drawString(63, 12, "VISH");
        display.display();
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
 
void connectWiFi(){
 
    while (WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, senha);
        delay(1000);
    }
 
    // Apresenta na serial que foi possivel se conectar
    Serial.println("Conectado");
 
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
