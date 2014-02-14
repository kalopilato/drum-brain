/*
	DrumPad.cpp - Library for creating and accessing drumpads for the "MIDI Drum Brain" Arduino project.
	Created by Kalo Pilato, 13th Feb, 2014.
	************************ LICENSE TBC	****************************** 
*/

#include "Arduino.h"
#include "DrumPad.h"

/* Constructor */
DrumPad::DrumPad(String name, byte note, byte pin){
	_name = name;
	_note = note;
	_pin = pin;
}

/* Return name of DrumPad */
String DrumPad::getName(){
	return _name;
}

/* Return note of DrumPad */
byte DrumPad::getNote(){
	return _note;
}

/* Return pin of DrumPad */
byte DrumPad::getPin(){
	return _pin;
}

/* Set note of DrumPad */
void DrumPad::setNote(byte newNote){
	_note = newNote;
}