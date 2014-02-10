#define DRUM1 0    // Our analog pin

/* Tunable Values */
byte ANALOG_SAMPLES = 5;        // Defines how many consecutive reads to take from one strike
byte MAX_STRIKE_VOLTAGE = 150;  // Defines the maximum voltage limit to be read from one strike (higher voltages are clipped to this value)
byte MIN_VEL = 5;               // Defines the minimum output MIDI velocity
byte DELAY_TIME = 15;           // Defines how many ms to wait after detecting a strike before polling for another

void setup(){
  Serial.begin(9600);           // Initializing the serial port at 9600 baud
}

void loop(){
  byte val = analogRead(DRUM1); // Read the voltage into a variable for the voltage value
  byte maxVal = 0;              // Initializing the variable for the maximum value from one strike
  if(val != 0){
    float currentMax = maxVal;

    for(int read = 0; read < ANALOG_SAMPLES; read++){
      byte currentVal = analogRead(DRUM1);  // Read the current voltage
      if (currentVal > maxVal) maxVal = currentVal;
    }
    
    if(currentMax > MAX_STRIKE_VOLTAGE) currentMax = MAX_STRIKE_VOLTAGE;  // Limit strike maximum voltage value to MAX_STRIKE_VOLTAGE(defined in class)
    byte midiVel = (float) currentMax / MAX_STRIKE_VOLTAGE * 127; 
    if(midiVel < MIN_VEL) midiVel = MIN_VEL;  // Limit minimum velocity value to MIN_VEL (defined in class)
    //tidy
    Serial.println("Strike");
    Serial.println(midiVel, DEC);  // Output MIDI velocity to serial monitor
    Serial.println();           // Print blank line between hits
    
    delay(DELAY_TIME);  
  }
}
