
#include<ESP8266WiFi.h>
#include<FirebaseArduino.h>
#define FIREBASE_HOST  "pdhh7511.firebaseio.com"
#define FIREBASE_AUTH "XHltqT7OMBjqsJ4DPzMhGgZyD3aAE76piuIgbJ6A"

String ssid="ACT114";
String pass="12345678";
 int count=0;
 int  sens=D7;
 int total=0;  
 int item=0;
 int value;
 int ct=0;
 int at=0;
 int bt=0;
 int clean;
 int alert1=1;
 
 void setup()
 {
  Serial.begin(9600);
  pinMode(sens,INPUT);
  WiFi.begin(ssid,pass);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(50);
  }//ct=millis()
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
   if (Firebase.failed()) {
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
    return;
  Firebase.setInt("alert",0);
  Firebase.setInt("alert1",0);    
  }
   
 }
 void loop()
 {
 clean= Firebase.getInt("clean");
  if(clean==1)//if clean button is on
  {
    
    count=0;
    ct=0;//auto alert after particular time
    at=0;// alert to admin if cleaner wont clean
    alert1=0;
    Firebase.setInt("alert1",0);
    
  }
  else//if clean is off
  {
  value= digitalRead(sens);//working of sensor begin
  if(value==HIGH&&item==1)
  {
    count++;
    total++;
    item=0;
    Serial.println(count);
  }
  else if(value==LOW&&item==0)
  {
   item=1; 
  } 
  Firebase.setInt("pc11",count);
  Firebase.setInt("tc11",total);
 
 if(count==5)
  {
    count=0;
    Serial.println("ready to clean");
    Firebase.setInt("alert",1);
    Serial.println("DONE");
    alert1=1;
    bt=millis();
  }
  if(millis()>(ct+1200000))//auto alert after particular time
  {
 
  ct=millis();
    Firebase.setInt("time-clean",1);
    alert1=1;
    at=millis();
    
  }
  if((alert1==1)&&((millis()>(at+600000))||(millis()>(bt+60000))))// alert to admin if cleaner wont clean

  {
    Firebase.setInt("alert1",1);
  }
}
  }
