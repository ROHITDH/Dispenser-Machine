// 25/04/2022 | ROHIT D H | +91 9448803740 | DISPENSER CONTROL
// Version 3.0.tiny
// Only supports Tea Dispense of 1 kind
// Supports Relay,Buzzer,LED,auto count, Temp senser, heater


#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <OneWire.h> 
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D3

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);
const int relaypin = D4;
float temp_data;





////////// CONSTANTS TO SET ///////////////////
// PUT YOUR WIFI USERNAME, PASSWORD 
const char* ssid = "YOUR_WIFI_HOTSPOT_USERNAME";
const char* password = "YOUR_WIFI_HOTSPOT_PASSWORD";

// PUT YOUR PASSKEY OBTAINED VIA MAIL
String passkey = "PUT_YOUR_PASSKEY_HERE";
///////////////////////////////////////////////




///////// DO NOT ALTER HERE ////////////////////
//Your Domain name with URL path or IP address with path - don't change this URL, unlesss you dev your own server
String host = "https://dvrblacktech.pythonanywhere.com/dispenser_v2";
String host_1 = host + "?passkey=" + passkey + "&nodemcu=1";

String host_val = "https://dvrblacktech.pythonanywhere.com/dispenser_getval/";
String host_val_1 = host_val + passkey;

String host_count = "https://dvrblacktech.pythonanywhere.com/dispenser_getcount/";
String host_count_1 = host_count + passkey;

String host_counter = "https://dvrblacktech.pythonanywhere.com/dispenser_getcounter/";
String host_counter_1 = host_counter + passkey;

// REFER THIS FOR PIN 16 TO CONNECT RELAY  
// https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/05/ESP8266-NodeMCU-kit-12-E-pinout-gpio-pin.png
const int RELAY_tea_small = D7;
const int led = D2;
const int buzzer = D1;


const int trigPin = D6;
const int echoPin = D5;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
long duration;
float distanceCm;
float distanceInch;

int amt_tea_small;
int count_tea_small;

int  counter_tea_small;

String query,res;
String payload_values,str;
String strs[20];
int StringCount, index_;

void setup() {
  Serial.begin(115200); 
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
  pinMode(RELAY_tea_small,OUTPUT);digitalWrite(RELAY_tea_small,1);
  
  pinMode(led,OUTPUT);digitalWrite(led,0);
  pinMode(buzzer,OUTPUT);digitalWrite(buzzer,0);

   sensors.begin(); 
 pinMode(relaypin, OUTPUT);
digitalWrite(relaypin, HIGH);
  
}

void loop() {
    if(WiFi.status()== WL_CONNECTED)
    {
      
      WiFiClientSecure client;
      HTTPClient http;

      
      /////////////////////
      //Get values of Payment
      /////////////////////
      client.setInsecure(); //the magic line, use with caution
      client.connect(host_val_1, 443);
      http.begin(client, host_val_1);
      if (http.GET() == HTTP_CODE_OK)    
          payload_values = http.getString();
      Serial.println(payload_values);

      
      StringCount = 0;
      str = payload_values;

      // Split the string into substrings
      while (str.length() > 0)
      {
        index_ = str.indexOf(':');
        if (index_ == -1) // No space found
        {
          strs[StringCount++] = str;
          break;
        }
        else
        {
          strs[StringCount++] = str.substring(0, index_);
          str = str.substring(index_+1);
        }
      }
      // Show the resulting substrings
      for (int i = 0; i < StringCount; i++)
      {
        Serial.print(i);
        Serial.print(": \"");
        Serial.print(strs[i]);
        Serial.println("\"");
      }

      amt_tea_small=strs[0].toInt();
      client.stop();


      /////////////////////
      //Get values of Counters Max
      /////////////////////
      client.setInsecure(); //the magic line, use with caution
      client.connect(host_count_1, 443);
      http.begin(client, host_count_1);
      if (http.GET() == HTTP_CODE_OK)    
          payload_values = http.getString();
      Serial.println(payload_values);
      
      StringCount = 0;
      str = payload_values;

      // Split the string into substrings
      while (str.length() > 0)
      {
        index_ = str.indexOf(':');
        if (index_ == -1) // No space found
        {
          strs[StringCount++] = str;
          break;
        }
        else
        {
          strs[StringCount++] = str.substring(0, index_);
          str = str.substring(index_+1);
        }
      }
      // Show the resulting substrings
      for (int i = 0; i < StringCount; i++)
      {
        Serial.print(i);
        Serial.print(": \"");
        Serial.print(strs[i]);
        Serial.println("\"");
      }

      count_tea_small=strs[0].toInt();
      client.stop();
      



      /////////////////////
      //Get values of Counters
      /////////////////////
      client.setInsecure(); //the magic line, use with caution
      client.connect(host_counter_1, 443);
      http.begin(client, host_counter_1);
      String payload_values;
      if (http.GET() == HTTP_CODE_OK)    
          payload_values = http.getString();
      Serial.println(payload_values);
      
      StringCount = 0;
      str = payload_values;

      // Split the string into substrings
      while (str.length() > 0)
      {
        index_ = str.indexOf(':');
        if (index_ == -1) // No space found
        {
          strs[StringCount++] = str;
          break;
        }
        else
        {
          strs[StringCount++] = str.substring(0, index_);
          str = str.substring(index_+1);
        }
      }
      // Show the resulting substrings
      for (int i = 0; i < StringCount; i++)
      {
        Serial.print(i);
        Serial.print(": \"");
        Serial.print(strs[i]);
        Serial.println("\"");
      }

      counter_tea_small=strs[0].toInt();
      client.stop();

      
      /////////////////////
      //Track payment
      /////////////////////
      client.setInsecure(); //the magic line, use with caution
      client.connect(host_1, 443);
      
      String payload;
      while(1){
        http.begin(client, host_1);
        main:
        sensors.requestTemperatures();
        temp_data = sensors.getTempCByIndex(0);
        Serial.println(temp_data);
         if (temp_data < 45)
         {
          digitalWrite(relaypin, LOW); 
         }
         else if (temp_data >50)
         {
         digitalWrite(relaypin, HIGH);
         }
 
        if (http.GET() == HTTP_CODE_OK)    
            payload = http.getString();
        if(payload[8]=='1')
        {
          int amt_f = (payload.substring(17,payload.length()).toInt())/100;

          
          /////////////////////tea dispense control////////////////////////////////
          //small tea
          if(counter_tea_small < count_tea_small){  
            if(amt_f==amt_tea_small)
            { 
              ultrasonic();
              unsigned long start = millis (); 
              while(((distanceCm>8) and (distanceCm<15)) and (millis () - start <= 20000))
                  ultrasonic();
                  
              if(millis () - start <= 20000){
              //tea - Small Cup Dispense
              delay(1000);
              Serial.println("Dispensing  Small Cup tea -- Please wait...");
              digitalWrite(RELAY_tea_small,0);
              digitalWrite(led,1);
              digitalWrite(buzzer,1);
              
              delay(5500);                                                                          //------- Change this time for variation
              digitalWrite(RELAY_tea_small,1);
               digitalWrite(led,0);
              digitalWrite(buzzer,0);
              Serial.println("Dispense Done.. take your cup.. Thank You..\n\n");

              //counter update
              counter_tea_small  +=1;
              //Send Request
              query = "&updater=tea_small_counter";
              http.begin(client, host_1+query);
              if (http.GET() == HTTP_CODE_OK)    
                  res = http.getString();
              Serial.println(res);
              Serial.print("Dispensed Count: ");Serial.println(counter_tea_small);
              }
              else{
                goto main;
              }
            }
          }
        }
      }
    }
}

void ultrasonic(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_VELOCITY/2;
  Serial.println(distanceCm);
  delay(1000);
}
