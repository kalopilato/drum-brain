
/* Drum Pad Input Pins */
int PIN_COUNT = 4; // Defines number of pins in use

/* Tunable Values */
byte ANALOG_SAMPLES = 3;        // Defines how many consecutive reads to take from one strike
byte MAX_STRIKE_VOLTAGE = 160;  // Defines the maximum voltage limit to be read from one strike (higher voltages are clipped to this value)
byte MIN_VEL = 5;               // Defines the minimum output MIDI velocity
byte DELAY = 10;                // Defines how many ms to wait after detecting a strike before polling for another
byte NOTE_HOLD = 20;            // Defines how long the midi note will hold for

/* MIDI Command Values */
int noteON = 144;               // 10010000 in binary, note on command
int noteOFF = 128;              // 10000000 in binary, note off command

void setup(){
  Serial.begin(31250);
}

void loop(){
  for(int i = 0; i < PIN_COUNT; i++){
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
      if(i == 0) ccNote = 36;
      if(i == 1) ccNote = 38;
      if(i == 2) ccNote = 54;
      if(i == 3) ccNote = 77;
      MIDIoutput(ccNote, velocity); 
    }
  }
  delay(DELAY);  
}

/* Write a single on/off signal to MIDI output */
void MIDIoutput(int MIDInote, int MIDIvelocity){
  Serial.write(noteON);       
  Serial.write(MIDInote);      
  Serial.write(MIDIvelocity);   
  delay(NOTE_HOLD);
  Serial.write(noteOFF);
  Serial.write(MIDInote);      
  Serial.write(MIDIvelocity);   
}

