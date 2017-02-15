/*
 *  I dati di un sensore DHT11 vengono inviati al servizio thingspeak con una GET 
 *
 *  E' necessario creare un file "secret.h" contentente i dat riservati: il
 *  SSID dell'AP, la password di accesso, e l'API_KEY del canale thingspeak.
 *  Il file contiene righe come le seguenti (sostituento le informazioni rilevanti":
 *  
 *    const char* ssid     = "MyAP";
 *    const char* password = "password";
 *    const char* privateKey = "SECRETSECRET";
 *  TESTED 15/02/2017
 */

#include <ESP8266WiFi.h>
#include "dht11.h"
#include "secret.h"

dht11 sensore;
const char* host = "api.thingspeak.com";

const int dht_pin=2;

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  
  Serial.print("WiFi connesso a ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  // Lettore del sensore
  if (sensore.read(dht_pin) != DHTLIB_OK) {
     Serial.println("Errore del sensore (riprovo)");
     delay(500);
  } else {
    // Creazione della URL con i dati
    String url = "http://api.thingspeak.com/update";
    url += "?api_key="; url += privateKey;
    url += "&field1=";  url += sensore.temperature;
    url += "&field2=";  url += sensore.humidity;
    
    // Costruzione di un client TCP sulla porta 80 (HTTP)
    Serial.println("Invio la GET REQUEST ");
    Serial.println(url);
    WiFiClient client;
    if (!client.connect(host, 80)) {
      Serial.println("connection failed");
      return;
    }
    // Invio della GET REQUEST con la URI preparata sopra
    // e due HTTP headers
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
    // Attesa della risposta             
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Timeout!");
        client.stop();
        return;
      }
    }
    
    delay(30000);
  }
}

