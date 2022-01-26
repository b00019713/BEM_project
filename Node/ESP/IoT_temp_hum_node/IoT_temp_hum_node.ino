
// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
#include <ESP8266WiFi.h>
#include "DHT.h"
#include <ThingSpeak.h>

// Network parameters
const char* ssid     = "Alex";
const char* password = "elusive89";

// ThingSpeak information
char thingSpeakAddress[] = "api.thingspeak.com";
unsigned long channelID = 1640611;
char* writeAPIKey = "DXEBMW51OT39GTVC";
char* readAPIKey = "S8TZRKGTIMNRHE8O";
const unsigned long postingInterval = 300L * 1000L * 1000L; // Should be time in micro.
const unsigned long loopStart = 0L; 

unsigned int dataFieldOne = 1;                            // Field to write temperature data
unsigned int dataFieldTwo = 2;                       // Field to write temperature data


unsigned long lastConnectionTime = 0;
long lastUpdateTime = 0; 
WiFiClient client;

#define DHTPIN 4     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  unsigned long startTime = millis();
// Init Serial DHT WiFi ThingSpeack   
  //Serial.begin(115200);
  //Serial.println(F("DHTxx test!"));
  dht.begin();  
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
// Hoock up wifi connection
  if(WiFi.status() != WL_CONNECTED){
    //Serial.print("Attempting to connect to SSID: ");
    //Serial.println(ssid);
    WiFi.begin(ssid, password);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
    while(WiFi.status() != WL_CONNECTED){
      
      //Serial.print(".");
      delay(2000);     
    } 
    //Serial.println("\nConnected.");
  }
// Read DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // Check if read was a success
  if (isnan(h) || isnan(t) ) {
    //Serial.println(("Failed to read from DHT sensor!"));
  }else{
    // Send data to thingspeak
    //Serial.println("Hum =  " + String( h )+ " Temp = "+String( t ));
    int x = write2TSData( channelID , dataFieldOne , t , dataFieldTwo , h);      // Write the temperature in C, and humidity.
    //Serial.print("Write Status: " + String(x));
  }
  ESP.deepSleep(postingInterval-(startTime-millis())*1000L);      
}

void loop() {

}

int write2TSData( long TSChannel, unsigned int TSField1, float field1Data, unsigned int TSField2, long field2Data){

  ThingSpeak.setField( TSField1, field1Data );
  ThingSpeak.setField( TSField2, field2Data );

   
  int writeSuccess = ThingSpeak.writeFields( TSChannel, writeAPIKey );
  return writeSuccess;
}


/*
// Use this function if you want to write a single field.
int writeTSData( long TSChannel, unsigned int TSField, float data ){
  int  writeSuccess = ThingSpeak.writeField( TSChannel, TSField, data, writeAPIKey ); // Write the data to the channel
  if ( writeSuccess ){
    
    Serial.println( String(data) + " written to Thingspeak." );
    }
    
    return writeSuccess;
}

// Use this function if you want to write multiple fields simultaneously.
int write2TSData( long TSChannel, unsigned int TSField1, float field1Data, unsigned int TSField2, long field2Data, unsigned int TSField3, long field3Data ){

  ThingSpeak.setField( TSField1, field1Data );
  ThingSpeak.setField( TSField2, field2Data );
  ThingSpeak.setField( TSField3, field3Data );
   
  int writeSuccess = ThingSpeak.writeFields( TSChannel, writeAPIKey );
  return writeSuccess;
}


int connectWiFi(){
      WiFi.mode(WIFI_STA);
      ThingSpeak.begin( client );
      WiFi.begin( ssid, password );
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi");
    }
    
    Serial.println( "Connected" );
    
}*/
