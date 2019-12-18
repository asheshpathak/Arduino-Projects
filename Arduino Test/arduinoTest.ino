#include <SimpleDHT.h>
#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<ESP8266HTTPClient.h>
#include<stdio.h>

//Declarations for Color Sensor
#define S0 4
#define S1 5
#define S2 15
#define S3 13
#define sensorOut 12
int fRed = 0;
int fBlue = 0;
int fGreen = 0;

//Declaring Request Body
char reqBody[100];

const char *ssid = "agrawal99";
const char *pass = "shekhar1";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ESP8266WebServer server;

   //Configuring WiFi
  WiFi.begin(ssid,pass);
  while(WiFi.status()!=WL_CONNECTED)
  {
      Serial.print("."); //If connection is established print this line
      delay(500);
  }
      Serial.println("");
      Serial.print("IP Address:");
      Serial.print(WiFi.localIP());
      Serial.println("");

      pinMode(S0, OUTPUT);
      pinMode(S1, OUTPUT);
      pinMode(S2, OUTPUT);
      pinMode(S3, OUTPUT);
      pinMode(sensorOut, INPUT);

       // Setting frequency-scaling to 20%
      digitalWrite(S0,HIGH);
      digitalWrite(S1,LOW);
}

void loop() {

  //server.handleClient();
  if(WiFi.status()==WL_CONNECTED) //Check WiFi connection status
 {
  // put your main code here, to run repeatedly:
  //Color Sensor Properties
     // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  fRed = pulseIn(sensorOut, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  fRed = map(fRed, 68,800,255,0);

  
  
  
  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  fGreen = pulseIn(sensorOut, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  fGreen = map(fGreen, 65,920,255,0);
 

  
  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  fBlue = pulseIn(sensorOut, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  fBlue = map(fBlue, 45,730,255,0);

  Serial.print("R= ");
  Serial.print(fRed);
  Serial.print("  ");
  Serial.print("G= ");
  Serial.print(fGreen);
  Serial.print("  ");
  Serial.print("B= ");
  Serial.print(fBlue);
  Serial.println("  ");
    
    HTTPClient http; //declare object of class HTTPClient
    
    http.begin("http://polar-lowlands-99644.herokuapp.com/update");      //Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //Specify content-type header
   
    sprintf(reqBody, "red=%d&blue=%d&green=%d",fRed,fBlue,fGreen);
    Serial.print("Resquest URL");
    Serial.print(reqBody); 
    Serial.println();
   
    int httpCode = http.POST(reqBody);   //Send the request
    String payload = http.getString();                  //Get the response payload

    Serial.println(httpCode);   //Print HTTP return code
    Serial.print("Response from Server :"+payload);    //Print request response payload
    Serial.println();
      Serial.println("");
  Serial.print("R= ");
  Serial.print(fRed);
  Serial.print("  ");
  Serial.print("G= ");
  Serial.print(fGreen);
  Serial.print("  ");
  Serial.print("B= ");
  Serial.print(fBlue);
  Serial.println("  ");
     
     http.end();  //Close connection
  }
    else 
     {
      Serial.println("Error in WiFi Connection");
     }
  
 
  delay(5000);
}
