char phone_no[]="xxxxxxxxxxxx";#phone number

#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define btn 4

int RXPin = 3;
int TXPin = 2;

int GPSBaud = 9600;
SoftwareSerial mySerial(8,9); 
TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);




String data;
char controller;
float Latitude;
float Longitude;


void setup(){
  Serial.begin(115200);
  mySerial.begin(115200);
  ss.begin(GPSBaud);
  
  pinMode(btn,INPUT);
  pinMode(13,OUTPUT);

  mySerial.println("AT");
  updateSerial();

  // configure sms mode
  mySerial.println("AT+CMGF=1");
  updateSerial();
  
}

void loop(){
  
  data = "";

  
  while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
      
      Latitude = gps.location.lat(); 
      Longitude = gps.location.lng(); 
      
      
      
      if (gps.location.isValid())
      {
        digitalWrite(13, HIGH);
      }
      else
      {
        digitalWrite(13, LOW);
      }
    }
  }
  

  while(mySerial.available())// forward mySerial data to serial port
      {
        char incoming = mySerial.read();
        data += incoming;
        Serial.write(incoming);
      }
      
      if(data != "")
      {
        if(data.indexOf("RING")>0)
        {

          Serial.println("yeah yeah");
          
           mySerial.print("AT+CMGS=\"");
           mySerial.print(phone_no);
           mySerial.write(0x22);
           mySerial.write(0x0D); // hex equivalent of Carriage return
           mySerial.write(0x0A); // hex equivalent of newline
           updateSerial();
           delay(2000);
        
          // to send the message
           mySerial.println("Mr. John's current location is");
           mySerial.println("");
           mySerial.print("http://maps.google.com/maps?q=loc:");
           mySerial.print(Latitude,7);
           mySerial.print(",");
           mySerial.print(Longitude,7);
           updateSerial();
           delay(500);
           mySerial.println((char)26); // decimal equivalent of ctrl+z
           mySerial.println();
           delay(120000);

           mySerial.println("AT");
           updateSerial();
        
          // configure sms mode
           mySerial.println("AT+CMGF=1");
           updateSerial();
        }
  
       
     }



  
  
  if(digitalRead(btn) == HIGH){
    
    Serial.println("yeah yeah");          
    mySerial.print("AT+CMGS=\"");
    mySerial.print(phone_no);
    mySerial.write(0x22);
    mySerial.write(0x0D); // hex equivalent of Carriage return
    mySerial.write(0x0A); // hex equivalent of newline
    updateSerial();
    delay(2000);
    
    // to send the message
    mySerial.println("Kidnap Alert!!!! Mr. John just triggered his device. This is his current location:");
    mySerial.println("");
    mySerial.print("http://maps.google.com/maps?q=loc:");
    mySerial.print(Latitude,7);
    mySerial.print(",");
    mySerial.print(Longitude,7);
    updateSerial();
    delay(500);
    mySerial.println((char)26); // decimal equivalent of ctrl+z
    mySerial.println();
    delay(2000);
    
    mySerial.println("AT");
    updateSerial();
    
    // configure sms mode
    mySerial.println("AT+CMGF=1");
    updateSerial();

    Serial.println("done");
    
    }
   
 } 
               



void updateSerial(){
  delay(500);
  while (Serial.available()){
    Serial.write(mySerial.read()); // forward serial data to mySerial port
  }
  while(mySerial.available()){
    Serial.write(mySerial.read()); // forward mySerial data to serial port
  }
}
