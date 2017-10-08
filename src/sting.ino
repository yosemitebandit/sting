/* Controlling my laser tag gun with an accelerometer.
*/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>


// Setup the accelerometers and output pins.
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
const int triggerPin = 2;
const int shieldPin = 3;
const int reloadPin = 4;


void setup(void) {
  Serial.begin(9600);
  pinMode(triggerPin, OUTPUT);
  pinMode(shieldPin, OUTPUT);
  pinMode(reloadPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  digitalWrite(shieldPin, LOW);
  digitalWrite(reloadPin, LOW);
  // Init the sensor.
  if(!accel.begin()) {
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
  // Set the range: 16, 8, 4 or 2g.
  accel.setRange(ADXL345_RANGE_16_G);
}


void loop(void) {
  // Get new accelerometer data.
  sensors_event_t event;
  accel.getEvent(&event);
  // Print measurements occasionally -- m/s^2
  /*Serial.print("X: "); Serial.println(event.acceleration.x);*/
  /*Serial.print("Y: "); Serial.println(event.acceleration.y);*/
  /*Serial.print("Z: "); Serial.println(event.acceleration.z);*/
  double magnitude = abs(event.acceleration.x * event.acceleration.y * event.acceleration.z);
  // Slash to fire.
  if (magnitude > 50000.0) {
    digitalWrite(triggerPin, HIGH);
    delay(300);
    digitalWrite(triggerPin, LOW);
  }
  // Thrust up to reload.
  else if (event.acceleration.y > 35) {
    digitalWrite(reloadPin, HIGH);
    delay(300);
    digitalWrite(reloadPin, LOW);
  }
  // Hold tip to the left for shielding.
  // Kinda depends on how you hold the sword I suppose.
  else if (event.acceleration.x < -8 && abs(event.acceleration.y) < 2 && abs(event.acceleration.z) < 2) {
    digitalWrite(shieldPin, HIGH);
    delay(2000);
    digitalWrite(shieldPin, LOW);
  }
  // Wait a bit.
  delay(20);
}
