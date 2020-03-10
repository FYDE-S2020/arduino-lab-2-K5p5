/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "UmugXZNqB59XqEN0LPTZ6-EJfPADUa8Q";

// Your WiFi credentials.
// Set password to "" for open networks.
// The EE IOT network is hidden. You might not be able to see it.
// But you should be able to connect with these credentials. 
char ssid[32] = "EE-IOT-Platform-02";
char pass[32] = "g!TyA>hR2JTy";
#define LED 2
const int freq = 5000;     // 5KHz frequency is more than enough. Remember we used 100 before.
const int ledChannel = 0;  // You can have up to 16 pwm channels (0 to 15)
const int resolution = 10; // 10 bit resolution for 1023. Freq should be greater than resolution.
int time_count = 0; // timer counter global variable
int duty = 0;
String content = "I can't believe this worked";  // null string constant ( an empty string )
BlynkTimer timer1;
BlynkTimer timer2;


void myTimerEvent()
{
// You can send any value at any time.
// Don't send more than 10 values a second or the Blynk server will block you!
    Blynk.virtualWrite(V2, millis() / 1000); // Write the arduino uptime every second
}

void myDisplay(){
 
   if (time_count % 25 == 0) { // every 250 ms
        // Do thing that needs to happen every 0.25 seconds
    }
    if (time_count == 100){
        // Do thing that needs to happen every 1 second
        time_count = 0; // reset time counter
    }
    else {
        // Send serial data to Blynk terminal
        char character;
        while(Serial.available()) { // Check if serial is available every 10 ms
            character = Serial.read();
            content.concat(character);
        }
        if (content != "") {
            Blynk.virtualWrite(V3, content);
            content = ""; // Clear String
        }  
    }
    time_count += 1; // Increment on every tick
        }  



void setup()
{
  // Serial Monitor
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(LED, OUTPUT);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(LED, ledChannel);
  timer1.setInterval(1000L, myTimerEvent);
  timer2.setInterval(10L, myDisplay);
}


BLYNK_WRITE(V1)
{
    // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    int duty = param.asInt(); // assigning incoming value from pin V1 to a variable
    ledcWrite(ledChannel, duty); // Note that this takes ledChannel as an argument, NOT the pin! Set duty = val.
}
BLYNK_WRITE(V0)
{

    // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

    // Because V1 is a button, pinValue will be a 0 or a 1.
    if (pinValue == 1) {
        ledcWrite(LED , 1023);// turn LED off
    }
    else {
      ledcWrite(LED , 0);
        // turn LED on
    }
}

void loop()
{
  Blynk.run();
  timer1.run(); // Initiates BlynkTimer
  timer2.run();

}
