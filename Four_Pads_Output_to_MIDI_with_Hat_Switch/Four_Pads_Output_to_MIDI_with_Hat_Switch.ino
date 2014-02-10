
/* Drum Pad Input Pins */
int PIN_COUNT = 4; // Defines number of pins in use

/* Tunable Values */
byte ANALOG_SAMPLES = 8;        // Defines how many consecutive reads to take from one strike
byte MAX_STRIKE_VOLTAGE = 100;  // Defines the maximum voltage limit to be read from one strike (higher voltages are clipped to this value)
byte MIN_VEL = 5;               // Defines the minimum output MIDI velocity
byte DELAY = 10;                // Defines how many ms to wait after detecting a strike before polling for another
byte NOTE_HOLD = 5;            // Defines how long the midi note will hold for
byte PEDAL_VELOCITY = 20;       // Defines velocity of pedal changes

/* MIDI Command Values */
int noteON = 144;               // 10010000 in binary, note on command
int noteOFF = 128;              // 10000000 in binary, note off command

/* MIDI Note Values */
int PEDAL_OFF = 59;
int PEDAL_ON = 48;
int KICK = 36;
int SNARE = 38;
int HAT_OPEN = 54;
int HAT_CLOSED = 49;
int CRASH_1 = 77;

/* Pedal Pins */
int hatPedal = 4;               // Defines which pin hat pedal is connected to
int kickPedal = 5;              // Defines which pin kick pedal is connected to

int pedalState = HIGH;          // Defines initial pedal state

void setup(){
  Serial.begin(31250);
  pinMode(hatPedal, INPUT);
}

void loop(){
  
  // Iterate through analog pads reading voltages and sending MIDI output    
  for(int i = 0; i < PIN_COUNT; i++){
            
    // Check if pedal state has changed
    if(digitalRead(hatPedal) != pedalState){
      if(pedalState == HIGH) MIDIoutput(PEDAL_ON, PEDAL_VELOCITY);
      else MIDIoutput(PEDAL_OFF, PEDAL_VELOCITY);
      pedalState = !pedalState;
    }
            
    if(analogRead(i) > 0){
      
      byte maxVal = 0;
  
      for(int read = 0; read < ANALOG_SAMPLES; read++){
        byte val = analogRead(i);
        if(val > maxVal) maxVal = val;
      }
      
      // Calculate note velocity
      if(maxVal > MAX_STRIKE_VOLTAGE) maxVal = MAX_STRIKE_VOLTAGE;   
      byte velocity = (float) maxVal / MAX_STRIKE_VOLTAGE * 127; 
      if(velocity < MIN_VEL) velocity = MIN_VEL;  // Limit minimum velocity value to MIN_VEL (defined in class)*/

      
      // Output MIDI
      int ccNote = 0;
      if(i == 0) ccNote = KICK;
      if(i == 1) ccNote = SNARE;
      if(i == 2) ccNote = getHat();
      if(i == 3) ccNote = CRASH_1;    
      MIDIoutput(ccNote, velocity); 
    }
  }
  delay(DELAY);  
}

/* Write a single on/off signal to MIDI output with velocity */
void MIDIoutput(int MIDInote, int MIDIvelocity){
  Serial.write(noteON);       
  Serial.write(MIDInote);      
  Serial.write(MIDIvelocity);   
  delay(NOTE_HOLD);
  Serial.write(noteOFF);
  Serial.write(MIDInote);      
  Serial.write(MIDIvelocity);   
}

/* Check if Hat should be open or closed and return the appropriate MIDI note */
int getHat(){
  if(pedalState == LOW) return HAT_CLOSED;
  else return HAT_OPEN;
}
