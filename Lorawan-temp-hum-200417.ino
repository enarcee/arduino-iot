// Sketch to test dh22
// Nick Cross 27/01/17 - based on sketch by AMaggio - 2-Sep-2016

#include <DHT.h>
#define USB Serial
#define mdot Serial1

#define app_id "xxxxxxxxxxxxxxxxxx" 
#define app_key "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

// Assign power and signal pins for sensors being used
// Temperature & Humimidity sensor
#define dht_data 2

DHT dht(dht_data, 22);
float temp = NAN;
float hum = NAN;
String sendstr;
unsigned int ctr = 0;

void setup()
{
  //Configure the IO pins
/*  pinMode(dht_gnd, OUTPUT);
  pinMode(dht_nc, INPUT);
  pinMode(dht_pwr, OUTPUT);
  pinMode(mic_vcc, OUTPUT);
  pinMode(mic_gnd, OUTPUT);
  pinMode(mic_aud, INPUT);
  pinMode(mic_vcc, OUTPUT);
  pinMode(mic_gnd, OUTPUT);
  pinMode(mic_aud, INPUT);
  digitalWrite(dht_gnd, LOW);
  digitalWrite(dht_nc, LOW);
  digitalWrite(dht_pwr, HIGH);
  digitalWrite(mic_vcc, HIGH);
  digitalWrite(mic_gnd, LOW);
  digitalWrite(ldr_vcc, HIGH);
  digitalWrite(ldr_gnd, LOW);
  */

  USB.begin(115200);
  mdot.begin(115200);
  delay(1000);
  while (!USB && (millis() < 5000));
  USB.println("TTN Environment Sensor");

  // Set up mDot module for communication with TTN in Australia
  chat("at");
  chat("at+pn=1"); // Public Network
  chat("at+njm=1"); // Network Join Mode is OTAA
  chat("at+ack=0"); // No Ack's from network
  chat("at+fsb=2"); // Frequency Sub-band = 2
  chat("at+txp=20"); // Max transmit power (for workshop devices, not for production)
  chat("at+ni=0,"+String(app_id));
  chat("at+nk=0,"+String(app_key));
  
  mdot.setTimeout(8000);
  chat("at+join");
  mdot.setTimeout(3000);
}

void chat(String instr)
{
  mdot.println(instr);
  String retstr = mdot.readString();
  USB.println(retstr);
}

void readDHT()
{
  temp = dht.readTemperature(false);
  delay(3000);
  hum = dht.readHumidity();
  if (isnan(temp) || isnan(hum)) // sometimes these sensors fail to get a reading, try again
  {
    USB.println("NaN");
    delay(5000);
    temp = dht.readTemperature(false);
    delay(3000);
    hum = dht.readHumidity();
  }
  if (isnan(temp) || isnan(hum)) // if still no reading, then skip this cycle and try later
  {
    USB.println(F("Still NaN. Skipping."));
  }
  else
  {
    USB.print("Temperature=");
    USB.print(temp);
    USB.print(", Humidity=");
    USB.println(hum);
    sendstr += temp;
    sendstr += ",";
    sendstr += hum;
  }
}

void loop()
{
  sendstr = "at+send=";

readDHT(); // DHT22 Temperature and Humidity Sensor

  
  chat(sendstr);
  delay(300000); // ms
}


