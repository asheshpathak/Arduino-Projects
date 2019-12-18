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

//Color Sensor Declaration Ends

//TMH declarations
 int percent=0;
 int tempType=0;
 int humType=0;

//WiFi Config Declarations
const char *ssid = "AndroidAP";
const char *pass = "wjwp8170";

//Declaring Request Body
char reqBody[100];

ESP8266WebServer server;
int moispin = A0; //PIN for taking in moisture values
int humTempPin = 16; //PIN for taking in Humidity and Temperature values
SimpleDHT11 dht11(humTempPin); //Instantiate Oject


int moisvalue=0;
byte temperature = 0;
byte humidity = 0;

void setup() 
{
  Serial.begin(115200);
  //ColorSensor

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

  //Color Sensor

  
  pinMode(moispin,INPUT);//declaring moisture pin to take inputs
  
  
}

void loop() {

  //POST METHODS
  server.handleClient();
  if(WiFi.status()==WL_CONNECTED) //Check WiFi connection status
  {
     
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

 
     // start working...
     // read without samples.
    int err = SimpleDHTErrSuccess;
    if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) 
    {
      Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
      return;
    }
    //PERFORM TYPECONVERSION ON HUM AND TEMP VALUES
    tempType=(int)temperature;
    humType=(int)humidity;
  
    moisvalue = analogRead(moispin);
    percent=convertToPercent(moisvalue);
   
    HTTPClient http; //declare object of class HTTPClient
    http.begin("http://immense-gorge-30611.herokuapp.com//updateStatus");      //Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //Specify content-type header
   
    sprintf(reqBody, "moisData=%d&tempData=%d&humData=%d&red=%d&blue=%d&green=%d",percent,tempType,humType,fRed,fBlue,fGreen);
    //Serial.print("Resquest URL");
    //Serial.print(reqBody); 
    //Serial.println();
   
    int httpCode = http.POST(reqBody);   //Send the request
    String payload = http.getString();                  //Get the response payload

    Serial.println(httpCode);   //Print HTTP return code
    Serial.print("Response from Server :"+payload);    //Print request response payload
    Serial.println();
    Serial.print("Moisture: ");Serial.print(percent);
    Serial.print(" Temperature: ");Serial.print(tempType);
    Serial.print(" Humidity: ");Serial.print(humType);
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
  
  // DHT11 sampling rate is 1HZ.
  delay(5000);


}




//Other Functions
int convertToPercent(int value)
{
  int percentValue = 0;
  percentValue = map(value, 1024, 330, 0, 100);
  return percentValue;
}
