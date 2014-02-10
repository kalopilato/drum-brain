int pinCount = 4; // Number of drum pins in use
int controlPin = 4; // Pin for unfiltered "cloned" hit

/* Tunable Values */
byte DELAY_TIME = 15;           // Defines how many ms to wait after detecting a strike before polling for another

void setup(){
  Serial.begin(9600);           // Initializing the serial port at 9600 baud
}

void loop(){
  for(int i = 0; i < pinCount; i++){
    byte val = analogRead(i); // Read the voltage into a variable for the voltage value
    byte controlVal = analogRead(controlPin);
    byte maxVal = 0;              // Initializing the variable for the maximum value from one strike
    if(val != 0){
      Serial.print("Strike: Pin ");
      Serial.print(i);
      Serial.print("\t");
      Serial.print("\t");
      Serial.print("\t");
      Serial.print("Pin ");
      Serial.println(controlPin);
      while(val != 0){
        Serial.print(val);
        Serial.print("\t");
        Serial.print("\t");
        Serial.print("\t");
        Serial.print("\t");
        Serial.println(controlVal);
        val = analogRead(i);
        controlVal = analogRead(controlPin);
      }
      Serial.println();           // Print blank line between hits 
    }
  }
  delay(DELAY_TIME);
}
