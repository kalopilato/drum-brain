#define DRUM1 0    // Our analog pin

/* Tunable Values */
byte ANALOG_SAMPLES = 3;        // Defines how many consecutive reads to take from one strike
byte MAX_STRIKE_VOLTAGE = 150;  // Defines the maximum voltage limit to be read from one strike (higher voltages are clipped to this value)
byte MIN_VEL = 5;               // Defines the minimum output MIDI velocity
byte DELAY = 15;                // Defines how many ms to wait after detecting a strike before polling for another
byte NOTE_HOLD = 20;            // Defines how long the midi note will hold for

/* MIDI Command Values */
int noteON = 144;               // 10010000 in binary, note on command
int noteOFF = 128;              // 10000000 in binary, note off command

void setup(){
  Serial.begin(31250);
}

void loop(){
  if(analogRead(DRUM1) > 0){
    byte maxVal = 0;

    for(int read = 0; read < ANALOG_SAMPLES; read++){
      byte val = analogRead(DRUM1);
      if(val > maxVal) maxVal = val;
    }
    
    // Calculate note velocity
    if(maxVal > MAX_STRIKE_VOLTAGE) maxVal = MAX_STRIKE_VOLTAGE;   
    byte velocity = (float) maxVal / MAX_STRIKE_VOLTAGE * 127; 
    if(velocity < MIN_VEL) velocity = MIN_VEL;  // Limit minimum velocity value to MIN_VEL (defined in class)*/
    
    // Output MIDI and LED
    MIDIoutput(38, velocity);
    delay(DELAY);  
  }
}

void MIDIoutput(int MIDInote, int MIDIvelocity){
  Serial.write(noteON);       
  Serial.write(MIDInote);      
  Serial.write(MIDIvelocity);   
  delay(NOTE_HOLD);
  Serial.write(noteOFF);
  Serial.write(MIDInote);      
  Serial.write(MIDIvelocity);   
}

