
#include <LiquidCrystal.h>
#include "DrumPad.h"
#include "KeyMap.h"



/* LCD Configuration */
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

/* Drum Pad Input Pins */
int PIN_COUNT = 6; // Defines number of pins in use

/* Name and version of the system */
String SYS_NAME = "MIDI Drum Brain";
String SYS_VERSION = "v1.0";

/* Tunable Values */
byte ANALOG_SAMPLES = 8;        // Defines how many consecutive reads to take from one strike
byte MAX_STRIKE_VOLTAGE = 150;  // Defines the maximum voltage limit to be read from one strike (higher voltages are clipped to this value)
byte MIN_VEL = 5;               // Defines the minimum output MIDI velocity
byte DELAY = 10;                // Defines how many ms to wait after detecting a strike before polling for another
byte NOTE_HOLD = 5;             // Defines how long the midi note will hold for
byte PEDAL_VELOCITY = 20;       // Defines velocity of pedal changes

/* MIDI Command Values */
int noteON = 144;               // 10010000 in binary, note on command
int noteOFF = 128;              // 10000000 in binary, note off command

/* MIDI Note Values */
int PEDAL_OFF = 59;
int PEDAL_ON = 48;

/* Create KeyMaps (Addictive Drums)*/
String kickNames[] = {"Kick"};
byte kickNotes[] = {36};
KeyMap kick(1, 0, kickNames, kickNotes);
String snareNames[] = {"Sn Rim", "Sn Open Hit", "Sn Rim (dbl)", "Sn Open Hit (dbl)", "Sn Shallow Rim", "Sn SideStick", "Sn Shallow Hit", "Sn Rim Click"};
byte snareNotes[] = {37, 38, 39, 40, 41, 42, 43, 44};
KeyMap snare(8, 1, snareNames, snareNotes);
String tomNames[] = {"Tom 1 Rim", "Tom 1 Open", "Tom 2 Rim", "Tom 2 Open", "Tom 3 Rim", "Tom 3 Open", "Tom 4 Rim", "Tom 4 Open"};
byte tomNotes[] = {72, 71, 70, 69, 68, 67, 66, 65};
KeyMap tom1(8, 1, tomNames, tomNotes); 
KeyMap tom2(8, 3, tomNames, tomNotes);
String hatClNames[] = {"HH 1 Close Tip", "HH 1 Close Shaft", "HH 2 Close Tip", "HH 2 Close Shaft", "HH Close Bell"};
byte hatClNotes[] = {49, 50, 51, 52, 53};
KeyMap hatClosed(5, 0, hatClNames, hatClNotes);
String hatOpNames[] = {"HH Open A", "HH Open B", "HH Open C", "HH Open D", "HH Open Bell"};
byte hatOpNotes[] = {54, 55, 56, 57, 58};
KeyMap hatOpen(5, 0, hatOpNames, hatOpNotes);
String cymbalNames[] = {"Cymbal 1", "Cymbal 1 (dbl)", "Cymbal 2", "Cymbal 3", "Ride Tip", "Ride Bell", "Ride Shaft", "Xtra"};
byte cymbalNotes[] = {77, 46, 79, 81, 60, 61, 62, 47};
KeyMap cymbal(8, 0, cymbalNames, cymbalNotes);

/* Create Drum Pads */
DrumPad kickPed("Kick Pedal", 36, 0);
DrumPad redPad("Red Pad", snare.getNote(), 1);
DrumPad bluePad("Blue Pad", tom1.getNote(), 2);
DrumPad greenPad("Green Pad", tom2.getNote(), 3);
DrumPad yellowPad("Yellow Pad", hatOpen.getNote(), 4);
DrumPad orangePad("Orange Pad", cymbal.getNote(), 5);
DrumPad drumPads[] = {kickPed, redPad, bluePad, greenPad, yellowPad, orangePad};
byte padCount = 6;

/* Pedal Pins */
int hatPedal = 6;               // Defines which pin hat pedal is connected to
int kickPedal = 5;              // Defines which pin kick pedal is connected to

int pedalState = HIGH;          // Defines initial pedal state

/* Button Pins */
int editButton = 2;
int backButton = 3;
int forwardButton = 4;
int enterButton = 5;

int backState = HIGH;
int forwardState = HIGH;
int editState = HIGH;
int enterState = HIGH;
int buttonState[] = {backState, forwardState, editState, enterState};
byte backPos = 0;
byte forwardPos = 1;
byte editPos = 2;
byte enterPos = 3;



void setup(){
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lineWrite(SYS_NAME, SYS_VERSION);
  Serial.begin(31250);
  pinMode(hatPedal, INPUT);
  pinMode(backButton, INPUT);
  pinMode(forwardButton, INPUT);
}

void loop(){
  
  // Iterate through analog pads reading voltages and sending MIDI output    
  for(int i = 0; i < padCount; i++){
            
    // Check if pedal state has changed
    if(digitalRead(hatPedal) != pedalState){
      if(pedalState == HIGH){ 
        MIDIoutput(PEDAL_ON, PEDAL_VELOCITY);
        drumPads[4].setNote(hatClosed.getNote());
      }
      else{ 
        MIDIoutput(PEDAL_OFF, PEDAL_VELOCITY);
        drumPads[4].setNote(hatOpen.getNote());
      }
      pedalState = !pedalState;
    }
    

    /*
    // Check if edit button has been pressed, if so go to the Edit Menu
    if(buttonPressed(editButton, editPos, false)){ 
      editMenu();
      lcd.clear();
      lcd.setCursor(0, 0);
      lineWrite(SYS_NAME, "");
    }
    
    */
    DrumPad currentPad = drumPads[i];
    byte currentPin = currentPad.getPin();

    if(analogRead(currentPin) > 50){
      
      byte maxVal = 0;
  
      for(int read = 0; read < ANALOG_SAMPLES; read++){
        byte val = analogRead(currentPin);
        if(val > maxVal) maxVal = val;
      }
      
      // Calculate note velocity
      if(maxVal > MAX_STRIKE_VOLTAGE) maxVal = MAX_STRIKE_VOLTAGE;   
      byte velocity = (float) maxVal / MAX_STRIKE_VOLTAGE * 127; 
      if(velocity < MIN_VEL) velocity = MIN_VEL;  // Limit minimum velocity value to MIN_VEL (defined in class)*/
      
      // Output MIDI
      MIDIoutput(currentPad.getNote(), velocity); 
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

/* Clear a line of the LCD then write to it */
void lineWrite(String message1, String message2){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message1);
  lcd.setCursor(0, 1);
  lcd.print(message2);
}

/* Provides editing of MIDI notes sent by each pad and display to LCD 
void editMenu(){
  byte padNumber = 0;
  //lineWrite(padNames[padNumber], "");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(padNames[padNumber]);
  
  while(!buttonPressed(editButton, editPos, true)){
    if(buttonPressed(forwardButton, forwardPos, true)){
      if(padNumber < PIN_COUNT - 1) padNumber++;
      else padNumber = 0;
      //lineWrite(padNames[padNumber], "");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(padNames[padNumber]);    
    }
    if(buttonPressed(backButton, backPos, true)){
      if(padNumber > 0) padNumber--;
      else padNumber = PIN_COUNT - 1;
      //lineWrite(padNames[padNumber], "");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(padNames[padNumber]);
    }   
    
    if(buttonPressed(enterButton, enterPos, true)){    
      int* currentNoteArray = noteArray[padNumber];
      String* currentNameArray = nameArray[padNumber];
      byte notePos = noteArrayPos[padNumber];
      int currentNote = currentNoteArray[notePos];
      String currentNoteName = currentNameArray[notePos];
      String editMsg = "Edit " + padNames[padNumber] + ":";
      //lineWrite(editMsg, currentNoteName);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(editMsg);
      lcd.setCursor(0, 1);
      lcd.print(currentNoteName);      
      boolean edited = false;
      while(!edited){
        
        if(buttonPressed(editButton, editPos, true)) return;
        
        if(buttonPressed(forwardButton, forwardPos, true)){
          if(notePos < noteArraySize[padNumber] - 1) notePos++;
          else notePos = 0;   
          currentNoteName = currentNameArray[notePos]; 
          //lineWrite(editMsg, currentNoteName);  
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(editMsg);
          lcd.setCursor(0, 1);
          lcd.print(currentNoteName);     
        }
        if(buttonPressed(backButton, backPos, true)){
          if(notePos > 0) notePos--;
          else notePos = noteArraySize[padNumber] - 1;
          currentNoteName = currentNameArray[notePos];
          //lineWrite(editMsg, currentNoteName);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(editMsg);
          lcd.setCursor(0, 1);
          lcd.print(currentNoteName); 
        }
        if(buttonPressed(enterButton, enterPos, true)){
          noteArrayPos[padNumber] = notePos;
          //lineWrite(editMsg, "DONE");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(editMsg);
          lcd.setCursor(0, 1);
          lcd.print("change saved"); 
          delay(500);
          edited = true;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(padNames[padNumber]); 
        }       
      }
    }  
  }
}
*/


/* Checks if a button has been pressed */
boolean buttonPressed(int button, byte statePos, boolean debounce){
  int state1 = digitalRead(button);
  if(debounce){
    delay(5);
    int state2 = digitalRead(button);
    if(state1 != state2) return false;
  }
  boolean pressed = false;
  int lastState = buttonState[statePos];
  if(state1 != lastState){
    if(state1 == LOW) pressed = true;
    buttonState[statePos] = !lastState;
  }
  return pressed;
}

