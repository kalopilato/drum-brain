#define DRUM1 0    // Our analog pin

/* Tunable Values */
byte ANALOG_SAMPLES = 5;        // Defines how many consecutive reads to take from one strike
byte MAX_STRIKE_VOLTAGE = 150;  // Defines the maximum voltage limit to be read from one strike (higher voltages are clipped to this value)
byte MIN_VEL = 5;               // Defines the minimum output MIDI velocity
byte DELAY = 30;                // Defines how many ms to wait after detecting a strike before polling for another

void setup(){
  Serial.begin(9600);
}

void loop(){
  if(analogRead(DRUM1) > 0){
    byte maxVal = 0;

    for(int read = 0; read < ANALOG_SAMPLES; read++){
      byte val = analogRead(DRUM1);
      if(val > maxVal) maxVal = val;
    }
    
    if(maxVal > MAX_STRIKE_VOLTAGE) maxVal = MAX_STRIKE_VOLTAGE;   
    byte velocity = (float) maxVal / MAX_STRIKE_VOLTAGE * 127; 
    if(velocity < MIN_VEL) velocity = MIN_VEL;  // Limit minimum velocity value to MIN_VEL (defined in class)*/

    //tidy
    Serial.println("Strike");
    Serial.println(velocity, DEC);
    Serial.println();
    
    delay(DELAY);  
  }
}
