// 25/04/2022 | ROHIT D H | +91 9448803740 | DISPENSER CONTROL

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>




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
const int RELAY_tea_small = D1;
const int RELAY_tea_medium = D2;


const int RELAY_coffe_small = D3;
const int RELAY_coffe_medium = D4;

const int RELAY_juice_small = D7;
const int RELAY_juice_medium = D8;

const int trigPin = D6;
const int echoPin = D5;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
long duration;
float distanceCm;
float distanceInch;

int amt_tea_small,amt_tea_medium,amt_coffe_small,amt_coffe_medium,amt_juice_small,amt_juice_medium;
int count_tea_small,count_tea_medium,count_coffe_small,count_coffe_medium,count_juice_small,count_juice_medium;

int  counter_tea_small,counter_tea_medium,counter_coffe_small,counter_coffe_medium,counter_juice_small,counter_juice_medium; 


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
  
  pinMode(RELAY_tea_small,OUTPUT);digitalWrite(RELAY_tea_small,1); pinMode(RELAY_tea_medium,OUTPUT);digitalWrite(RELAY_tea_medium,1);
  pinMode(RELAY_coffe_small,OUTPUT);digitalWrite(RELAY_coffe_small,1); pinMode(RELAY_coffe_medium,OUTPUT);digitalWrite(RELAY_coffe_medium,1);
  pinMode(RELAY_juice_small,OUTPUT);digitalWrite(RELAY_juice_small,1); pinMode(RELAY_juice_medium,OUTPUT);digitalWrite(RELAY_juice_medium,1);
  
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
      payload_values;
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

      amt_tea_small=strs[0].toInt();amt_tea_medium=strs[1].toInt();
      amt_coffe_small=strs[3].toInt();amt_coffe_medium=strs[4].toInt();
      amt_juice_small=strs[6].toInt();amt_juice_medium=strs[7].toInt();
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

      count_tea_small=strs[0].toInt();count_tea_medium=strs[1].toInt();
      count_coffe_small=strs[3].toInt();count_coffe_medium=strs[4].toInt();
      count_juice_small=strs[6].toInt();count_juice_medium=strs[7].toInt();
      client.stop();


      /////////////////////
      //Get values of Counters
      /////////////////////
      client.setInsecure(); //the magic line, use with caution
      client.connect(host_counter_1, 443);
      http.begin(client, host_counter_1);
      if (http.GET() == HTTP_CODE_OK)    
          payload_values = http.getString();
      Serial.println(payload_values);

      StringCount = 0;
      str = payload_values;

      // Split the string into substrings
      while (str.length() > 0)
      {
        int index_ = str.indexOf(':');
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

      counter_tea_small=strs[0].toInt();counter_tea_medium=strs[1].toInt();
      counter_coffe_small=strs[3].toInt();counter_coffe_medium=strs[4].toInt();
      counter_juice_small=strs[6].toInt();counter_juice_medium=strs[7].toInt();
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
              while((distanceCm>20) and (millis () - start <= 20000))
                  ultrasonic();
                  
              if(millis () - start <= 20000){
              //tea - Small Cup Dispense
              delay(2000);
              Serial.println("Dispensing  Small Cup tea -- Please wait...");
              digitalWrite(RELAY_tea_small,0);
              delay(5000);                                                                          //------- Change this time for variation
              digitalWrite(RELAY_tea_small,1);
              Serial.println("Dispense Done.. take your cup.. Thank You..\n\n");

              //counter update
              counter_tea_small  +=1;
              //Send Request
              query = "&updater=tea_small_counter";
              http.begin(client, host_1+query);
              if (http.GET() == HTTP_CODE_OK)    
                  res = http.getString();
              Serial.println(res);
              }
              else{
                goto main;
              }
            }
          }

          
          //medium tea
          if(counter_tea_medium < count_tea_medium){
            if(amt_f==amt_tea_medium)
            { 
              ultrasonic();
              unsigned long start = millis (); 
              while((distanceCm>20) and (millis () - start <= 20000))
                  ultrasonic();
              if(millis () - start <= 20000){
              //tea - Medium Cup Dispense
              delay(2000);
              Serial.println("Dispensing  Medium Cup tea -- Please wait...");
              digitalWrite(RELAY_tea_medium,0);
              delay(10000);                                                                          //------- Change this time for variation
              digitalWrite(RELAY_tea_medium,1);
              Serial.println("Dispense Done.. take your cup.. Thank You..\n\n");
              
              //counter update
              counter_tea_medium  +=1;
              //Send Request
              query = "&updater=tea_medium_counter";
              http.begin(client, host_1+query);
              if (http.GET() == HTTP_CODE_OK)    
                  res = http.getString();
              Serial.println(res);
              }
              else{
                goto main;
              }
            }
          }
          //////////////////////////////////////////////////////////////

          /////////////////////coffe duspense control////////////////////////////////
          //small coffe
          if(counter_coffe_small < count_coffe_small){
            if(amt_f==amt_coffe_small)
            { 
              ultrasonic();
              unsigned long start = millis (); 
              while((distanceCm>20) and (millis () - start <= 20000))
                  ultrasonic();
                  
              if(millis () - start <= 20000){
              //coffe - Small Cup Dispense
              delay(2000);
              Serial.println("Dispensing  Small Cup coffe -- Please wait...");
              digitalWrite(RELAY_coffe_small,0);
              delay(5000);                                                                          //------- Change this time for variation
              digitalWrite(RELAY_coffe_small,1);
              Serial.println("Dispense Done.. take your cup.. Thank You..\n\n");
  
              //counter update
              counter_coffe_small  +=1;
              //Send Request
              query = "&updater=coffe_small_counter";
              http.begin(client, host_1+query);
              if (http.GET() == HTTP_CODE_OK)    
                  res = http.getString();
              Serial.println(res);
              }
              else{
                goto main;
              }
            }
          }
          //medium coffe
          if(counter_coffe_medium < count_coffe_medium){
            if(amt_f==amt_coffe_medium)
            { 
              ultrasonic();
              unsigned long start = millis (); 
              while((distanceCm>20) and (millis () - start <= 20000))
                  ultrasonic();
              if(millis () - start <= 20000){
              //coffe - Medium Cup Dispense
              delay(2000);
              Serial.println("Dispensing  Medium Cup coffe -- Please wait...");
              digitalWrite(RELAY_coffe_medium,0);
              delay(10000);                                                                          //------- Change this time for variation
              digitalWrite(RELAY_coffe_medium,1);
              Serial.println("Dispense Done.. take your cup.. Thank You..\n\n");

              //counter update
              counter_coffe_medium  +=1;
              //Send Request
              query = "&updater=coffe_medium_counter";
              http.begin(client, host_1+query);
              if (http.GET() == HTTP_CODE_OK)    
                  res = http.getString();
              Serial.println(res);
              }
              else{
                goto main;
              }
            }
          }
          //////////////////////////////////////////////////////////////

          /////////////////////juice duspense control////////////////////////////////
          //small juice
          if(counter_juice_small < count_juice_small){
            if(amt_f==amt_juice_small)
            { 
              ultrasonic();
              unsigned long start = millis (); 
              while((distanceCm>20) and (millis () - start <= 20000))
                  ultrasonic();
                  
              if(millis () - start <= 20000){
              //juice - Small Cup Dispense
              delay(2000);
              Serial.println("Dispensing  Small Cup juice -- Please wait...");
              digitalWrite(RELAY_juice_small,0);
              delay(5000);                                                                          //------- Change this time for variation
              digitalWrite(RELAY_juice_small,1);
              Serial.println("Dispense Done.. take your cup.. Thank You..\n\n");
              
              //counter update
              counter_juice_small  +=1;
              //Send Request
              query = "&updater=juice_small_counter";
              http.begin(client, host_1+query);
              if (http.GET() == HTTP_CODE_OK)    
                  res = http.getString();
              Serial.println(res);
              }
              else{
                goto main;
              }
            }
          }
          //medium juice
          if(counter_juice_medium < count_juice_medium){
            if(amt_f==amt_juice_medium)
            { 
              ultrasonic();
              unsigned long start = millis (); 
              while((distanceCm>20) and (millis () - start <= 20000))
                  ultrasonic();
              if(millis () - start <= 20000){
              //juice - Medium Cup Dispense
              delay(2000);
              Serial.println("Dispensing  Medium Cup juice -- Please wait...");
              digitalWrite(RELAY_juice_medium,0);
              delay(10000);                                                                          //------- Change this time for variation
              digitalWrite(RELAY_juice_medium,1);
              Serial.println("Dispense Done.. take your cup.. Thank You..\n\n");

              //counter update
              counter_juice_medium  +=1;
              //Send Request
              query = "&updater=juice_medium_counter";
              http.begin(client, host_1+query);
              if (http.GET() == HTTP_CODE_OK)    
                  res = http.getString();
              Serial.println(res);
              }
              else{
                goto main;
              }
            }
          }
          //////////////////////////////////////////////////////////////


       
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
}
