

// Sketch for Water Tank Volume Sensor
// Nick Cross 18/11/2018

// include libraries for the appropriate sensor
// -----------------------------------------------------
// DHT22 Temp Humidity Sensor
// #include <DHT.h>
// -----------------------------------------------------
// Sleep Library
// #include <Narcoleptic.h>
// -----------------------------------------------------
// Ping Library
#include <NewPing.h>


// Define application constants and variables
// -----------------------------------------------------
// define serial ports - Arduino Leonardo
#define USB Serial
#define mdot Serial1

// Define your Things Network Application EUI and App Key
#define appEui "70B3D57ED00079C6" 
#define appKey "21A728F1F00B6A84F64DB7A1EB8C5158"


// Signal signal pins and max distance for JSN-SR04T
// -----------------------------------------------------
// Pin to receive echo pulse
#define echoPin 2  
// Pin to send trigger pulse                        
#define trigPin 3 
// Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm. 
#define maxDistance 300
// NewPing setup of pins and maximum distance.
NewPing sonar(trigPin, echoPin, maxDistance); 

// Water Tank Dimensions
// -----------------------------------------------------
// Tank Diameter (cm)
// #define tankDiameter 300
// Distance from tank floor to sensor (cm)
// #define tankHeight 300

                        
// Define BYTES array for data storage
// byte payload[1];

// Use debug fuction to print to serial monitor
void debug() {
  while (mdot.available()) USB.println(mdot.readString());
}

// -----------------------------------------------------
// Start Setup
// -----------------------------------------------------

void setup() {
  
  // wait for serial port to connect. needed for arduino leonardo
  while (!USB);

  // opens serial ports, set data rate to 115200 bps
  USB.begin(115200);
  mdot.begin(115200);

  // setup JSN-SR04T
  // pinMode(echoPin, INPUT);
  // pinMode(trigPin, OUTPUT);

  // join the things network
  mdot.println(F("AT"));
  debug();
  mdot.println(F("AT+PN=1"));
  debug();
  mdot.println(F("AT+FSB=2"));
  debug();
  mdot.println(F("AT+NI=0,"appEui));
  debug();
  mdot.println(F("AT+NK=0,"appKey));
  debug();
  mdot.println(F("AT+JOIN"));
  debug();
  delay(10000);
}

// Complete Setup

// -----------------------------------------------------
// Start Loop
// -----------------------------------------------------

void loop() {

  // rcv disatnce value for ultraSoundPing function
  unsigned int pingDistance = ultraSoundPing();
  // payload[0] = ultraSoundPing();

  // char value1 = ultraSoundPing.distance;

  // USB.println(pingDistance);

  // send the value to the mdot
  mdot.print("AT+SEND=");
  mdot.println(pingDistance);
  // mdot.println(pingDistance);
  // mdot.write(payload,1);
  // echo to the serial monitor
  debug();
 
  // pause
  delay(300000);

}


//Function Library ---------------------------------------------

int ultraSoundPing(){
  // Wait 50ms between pings
  delay(50);   
  // Send ping, get ping time in microseconds (uS).                   
  unsigned int uS = sonar.ping(); 
  // Convert pin time to CM
  unsigned int distance = (uS / US_ROUNDTRIP_CM);
  // return value
  return distance;  
}




