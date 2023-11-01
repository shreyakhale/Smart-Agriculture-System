#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <ArduinoJson.h>

// Sensor pins
#define sensorPin_rain 8
#define relay_pin A0
const int sensor_pin = A1;     /* Soil moisture sensor O/P pin */
const int sensor_pin_mq2 = A2; /* MQ2 sensor O/P pin */

// Define a JSON buffer
StaticJsonDocument<200> jsonBuffer;

// The time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 5;

// The time when the sensor outputs a low impulse
long unsigned int lowIn;

// The amount of milliseconds the sensor has to be low
// before we assume all motion has stopped
long unsigned int pause = 5000;

boolean lockLow = true;
boolean takeLowTime;

int pirPin = 3;    // The digital pin connected to the PIR sensor's output
int ledPin = 13;

void setup() {
  Serial.begin(9600);
  pinMode(relay_pin, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // Give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
  for (int i = 0; i < calibrationTime; i++) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  delay(50);
}

void loop() {
  // Get the reading from the function below and store it in a JSON object
  jsonBuffer["rain"] = digitalRead(sensorPin_rain);
  int sensor_analog = analogRead(sensor_pin);
  float moisture_percentage = (100 - (sensor_analog / 1023.00) * 100);
  jsonBuffer["moisture"] = moisture_percentage;
  jsonBuffer["relay"] = moisture_percentage < 20 && digitalRead(sensorPin_rain);
  jsonBuffer["pir"] = digitalRead(pirPin);

  unsigned int sensorValue = analogRead(sensor_pin_mq2);
  unsigned int outputValue = map(sensorValue, 0, 1023, 0, 255);
  jsonBuffer["gas"] = outputValue;

  // Serialize the JSON object and send it over the serial port
  serializeJson(jsonBuffer, Serial);
  Serial.println();

  delay(8000);
  if (digitalRead(pirPin) == HIGH) {
    digitalWrite(ledPin, HIGH);
    if (lockLow) {
      lockLow = false;
      Serial.println("---");
      Serial.print("motion detected at ");
      Serial.print(millis() / 1000);
      Serial.println(" sec");
      delay(50);
    }
    takeLowTime = true;
  }

  if (digitalRead(pirPin) == LOW) {
    digitalWrite(ledPin, LOW);
    if (takeLowTime) {
      lowIn = millis();
      takeLowTime = false;
    }
    if (!lockLow && millis() - lowIn > pause) {
      lockLow = true;
      Serial.print("motion ended at ");
      Serial.print((millis() - pause) / 1000);
      Serial.println(" sec");
      delay(50);
    }
  }
}
