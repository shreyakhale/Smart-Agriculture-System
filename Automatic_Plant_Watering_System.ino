// Sensor pins
#define sensorPin_rain 8
#define relay_pin A0
const int sensor_pin = A1;	/* Soil moisture sensor O/P pin */
const int sensor_pin_mq2 = A2;	/* MQ2 sensor O/P pin */

//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;        

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 3;    //the digital pin connected to the PIR sensor's output
int ledPin = 13;


void setup() {
  
	Serial.begin(9600);
  pinMode(relay_pin, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  //digitalWrite(pirPin, LOW);

  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    //digitalWrite(relay_pin, LOW);
    delay(50);
   
}

void loop() {
	//get the reading from the function below and print it
	int val_rain = digitalRead(sensorPin_rain);  

  //Rain
	Serial.println("Rain: ");
	Serial.println(val_rain);
  if (val_rain) {
		Serial.println("Status: Clear");
	} else {
		Serial.println("Status: It's raining");
	}
  

  //Moisture
  float moisture_percentage;
  int sensor_analog;
  sensor_analog = analogRead(sensor_pin);
  moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
  Serial.print("Moisture Percentage = ");
  Serial.print(moisture_percentage);
  Serial.print("%\n\n");

  //Logic for relay
  if (moisture_percentage < 30){
    if (val_rain == 1) {
      digitalWrite(relay_pin, HIGH);
      Serial.println("Relay turned ON - Motor ON");
    }
    else{
      digitalWrite(relay_pin, LOW);
    }       
  }
  else{
  digitalWrite(relay_pin, LOW);
  }
    
  //Gas
  Serial.print("Analog output: ");
  Serial.println(readSensor());
    
	delay(8000);	// Take a reading every second
	Serial.println();
  if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
       if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;            
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec"); 
         delay(50);
         }         
         takeLowTime = true;
       }

  if(digitalRead(pirPin) == LOW){       
    digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state

    if(takeLowTime){
    lowIn = millis();          //save the time of the transition from high to LOW
    takeLowTime = false;       //make sure this is only done at the start of a LOW phase
    }
    //if the sensor is low for more than the given pause, 
    //we assume that no more motion is going to happen
    if(!lockLow && millis() - lowIn > pause){  
        //makes sure this block of code is only executed again after 
        //a new motion sequence has been detected
        lockLow = true;                        
        Serial.print("motion ended at ");      //output
        Serial.print((millis() - pause)/1000);
        Serial.println(" sec");
        delay(50);
        }
    }
}

//  This function returns the analog data to calling function
int readSensor() {
  unsigned int sensorValue = analogRead(sensor_pin_mq2);  // Read the analog value from sensor
  unsigned int outputValue = map(sensorValue, 0, 1023, 0, 255); // map the 10-bit data to 8-bit data
  if (outputValue > 650) //make this 200 for gas
    Serial.println("Gas detected");
  else
    Serial.println("No gas");
  return outputValue;             // Return analog moisture value
}

