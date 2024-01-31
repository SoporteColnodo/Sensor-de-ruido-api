#include <WiFi.h>

void ConnectWiFi_STA(bool useStaticIP = false)
{
   Serial.println("");
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   if(useStaticIP) WiFi.config(ip, gateway, subnet);
   while (WiFi.status() != WL_CONNECTED) 
   { 
     delay(100);  
     Serial.print('.'); 
   }
 
   Serial.println("");
   Serial.print("Proceso iniciado de:\t");
   Serial.println(hostname);
   Serial.print("Dirección IP:\t");
   Serial.println(WiFi.localIP());
}