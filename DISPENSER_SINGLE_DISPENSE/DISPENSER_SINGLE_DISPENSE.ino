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



int enA = D3;
int in1 = D4;
int in2 = D7;



///////// DO NOT ALTER HERE ////////////////////
//Your Domain name with URL path or IP address with path - don't change this URL, unlesss you dev your own server
String host = "https://dvrblacktech.pythonanywhere.com/dispenser_v2";
String host_1 = host + "?passkey=" + passkey + "&nodemcu=1";

String host_val = "https://dvrblacktech.pythonanywhere.com/dispenser_getval/";
String host_val_1 = host_val + passkey;

// REFER THIS FOR PIN 16 TO CONNECT RELAY  
// https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/05/ESP8266-NodeMCU-kit-12-E-pinout-gpio-pin.png
const int RELAY_tea_small = D1;
const int RELAY_tea_medium = D2;

const int RELAY_juice_medium = D8;

const int trigPin = D6;
const int echoPin = D5;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
long duration;
float distanceCm;
float distanceInch;

int amt_tea_small,amt_tea_medium,amt_coffe_small,amt_coffe_medium,amt_juice_small,amt_juice_medium;

void setup() {
  Serial.begin(115200); 

  pinMode(enA, OUTPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  
  
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
  
  pinMode(RELAY_tea_small,OUTPUT);digitalWrite(RELAY_tea_small,0); 
  
}

void loop() {
    if(WiFi.status()== WL_CONNECTED)
    {
      
      WiFiClientSecure client;
      HTTPClient http;

      /////////////////////
      //Get values
      /////////////////////
      client.setInsecure(); //the magic line, use with caution
      client.connect(host_val_1, 443);
      http.begin(client, host_val_1);
      String payload_values;
      if (http.GET() == HTTP_CODE_OK)    
          payload_values = http.getString();
      Serial.println(payload_values);

      String strs[20];
      int StringCount = 0;
      String  str = payload_values;

      // Split the string into substrings
      while (str.length() > 0)
      {
        int index = str.indexOf(':');
        if (index == -1) // No space found
        {
          strs[StringCount++] = str;
          break;
        }
        else
        {
          strs[StringCount++] = str.substring(0, index);
          str = str.substring(index+1);
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
      //Track payment
      /////////////////////
      client.setInsecure(); //the magic line, use with caution
      client.connect(host_1, 443);
        
      http.begin(client, host_1);
      
      String payload;
      while(1){
        main:
        if (http.GET() == HTTP_CODE_OK)    
            payload = http.getString();
        if(payload[8]=='1')
        {
          int amt_f = (payload.substring(17,payload.length()).toInt())/100;


          
          /////////////////////tea duspense control////////////////////////////////
          //small tea
          if(amt_f==amt_tea_small)
          { 
            ultrasonic();
            unsigned long start = millis (); 
            while((distanceCm>20) and (millis () - start <= 20000))
                ultrasonic();
                
            if(millis () - start <= 20000){
            //tea - Small Cup Dispense
            delay(2000);
            Serial.println("Dispensing Paracetamol Tablets -- Please wait...");
            
            digitalWrite(RELAY_tea_small,1);
            delay(5000);                                                                          //------- Change this time for variation
            digitalWrite(RELAY_tea_small,0);




          analogWrite(enA, 255);
          // Turn on motor A & B
 
  // Now change motor directions
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  delay(2000);

   digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  delay(2000);

  
  // Turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);




            
            
            Serial.println("Dispense Done.. take your cup.. Thank You..\n\n");
            }
            else{
              goto main;
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
