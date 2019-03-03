#include<ESP8266WiFi.h>
#include<FirebaseArduino.h>
#include<Servo.h>
#define FIREBASE_HOST  "pdhh7511.firebaseio.com"
#define FIREBASE_AUTH "XHltqT7OMBjqsJ4DPzMhGgZyD3aAE76piuIgbJ6A"
Servo servol;// alert to admin if cleaner wont cleaN

String ssid = "ACT114";     // Your WiFi credentials.
String pass = "12345678";
int presetdelay = 5000;
int gas = 13;
int i, pos1;
int r1 = 14;
int r2 = 12;
int count = 0;
int temp = 0;
int total = 0;
long int ontime = 0;
int cust = D2;
int cleanon = D1;
//int cleanoff = D3;
int cleanc = 0;
int led = D8;
int noc = 0;
int cleanf = 0;
int t;
int custc;
// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
int buttonPushCounter1 = 0;   // counter for the number of button presses
int buttonState1= 0;         // current state of the button
int lastButtonState1= 0;     // previous state of the button

void setup() {
  servol.attach(16);
  pinMode(gas, INPUT);
  pinMode(cleanon, INPUT);
  pinMode(cust, INPUT);
  pinMode(led, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
 Serial.begin(9600);
  WiFi.begin(ssid, pass);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  if (Firebase.failed()) {
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
    return;
  }
  Firebase.setInt("alert", 0);
}

void loop()
{

  buttonState = digitalRead(cleanon);
  if (buttonState!= lastButtonState)
  {
    
    if (buttonState== HIGH)
    {
      buttonPushCounter++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(buttonPushCounter);
    } 
    else 
    {
     
      Serial.println("off");
    }
    
    delay(75);
  }
  lastButtonState = buttonState;
  
  if (buttonPushCounter == 2) //I am adding this if loop so that the button counter value toggles only between 1 & 0
  {
    buttonPushCounter = 0;
    
   
  }
  cleanc = buttonPushCounter;  //this increments everytime the pushbutton is pressed

if(cleanc==0)
{
  Firebase.setInt("clean",0);
  noc++;
  Firebase.setInt("noc11",noc);
}


  if (cleanc == 1)
  {
    Firebase.setInt("clean", 0);
    
   
  }
  
  // put your main code here, ,to run repeatedly:
  int value = analogRead(gas);
   buttonState1 = digitalRead(cust); //i am assuming u have connected the pushbutton to cleanon digital pin.

  // compare the buttonState to its previous state
  if (buttonState1 != lastButtonState1) {
    // if the state has changed, increment the counter
    if (buttonState1== HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter1++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(buttonPushCounter1);
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState1 = buttonState1;

  if (buttonPushCounter1 == 2) //I am adding this if loop so that the button counter value toggles only between 1 & 0
  {
   Firebase.setInt("cc",0);
    buttonPushCounter1 = 0;
  }
  custc= buttonPushCounter1;  //this increments everytime the pushbutton is pressed

  
  int alert = Firebase.getInt("alert");
  if (value == HIGH && cleanc == 0)
  {
    Serial.print("washroom is ready to clean harmful gas detected");
    digitalWrite(r1, HIGH); //BUZZER
    digitalWrite(r2, HIGH); //SOLENOID
   
    
      for (pos1 = 0; pos1 < 180; pos1 += 1)
      {
        servol.write(pos1);
        delay(10);
      }
      for (pos1 = 180; pos1 >= 1; pos1 -= 1)
      {
        servol.write(pos1);
        delay(10);
      }
    
    ontime = millis();
    temp = 1;
    value = LOW;
  }
  if (custc == HIGH)
  {
    Firebase.setInt("cc",1);
  }
  if (alert == 1)
  {
    Serial.print("washroom is ready to clean (met the maximum usage)");
    digitalWrite(r1, HIGH); //BUZZER
    digitalWrite(r2, HIGH); //SOLENOID
    
      for (pos1 = 0; pos1 < 180; pos1 += 1)
      {
        servol.write(pos1);
        delay(10);
      }
      for (pos1 = 180; pos1 >= 1; pos1 -= 1)
      {
        servol.write(pos1);
        delay(10);
      }
    
    temp = 1;
    ontime = millis();
    Firebase.setInt("alert", 0);
  }
  presetdelay = Firebase.getInt("presetdelay") * 1000;
  if ((temp == 1) && (millis() > (ontime + presetdelay)))
  {
    Serial.println("inside");
    digitalWrite(r1, LOW);
    temp = 0;
  }

  if ((temp == 1) && (millis() > (ontime + presetdelay)))//can reset or fix the timing the relay should run
  {
    Serial.println("inside");
    digitalWrite(r2, LOW);
    temp = 0;

  }
}
