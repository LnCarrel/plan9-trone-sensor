/*
 * Enable relay when object is closed
 *  Use a Sparkfun IR sensor (Sharp GP2Y0A02YK0F)
 */

#include <Arduino.h>

#define IR_SENSOR_PIN 4
#define RELAY_OUTPUT 8

// the setup routine runs once when you press reset:
void setup() {
  // initialize digital pin RELAY_OUTPUT as an output.
  pinMode(RELAY_OUTPUT, OUTPUT);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  //Turn low outputs (LED & RELAY)
  digitalWrite(RELAY_OUTPUT, LOW);
  digitalWrite(LED_BUILTIN, LOW);

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  //Read analog input value
  int val = analogRead(IR_SENSOR_PIN);

  //If object is closed, enble output relay
  if (val > 400) {
    digitalWrite(RELAY_OUTPUT, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print("ON  / ");
  } else {
    digitalWrite(RELAY_OUTPUT, LOW);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("OFF / ");
  }
  Serial.println(val);

  // wait for a second
  delay(1000);
}
