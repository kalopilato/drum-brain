/*
	KeyMap.cpp - Library for creating and accessing MIDI Key Mappings for the "MIDI Drum Brain" Arduino project.
	Created by Kalo Pilato, 13th Feb, 2014.
	************************ LICENSE TBC	****************************** 
*/

#include "Arduino.h"
#include "KeyMap.h"

/* Constructor */
KeyMap::KeyMap(byte size, byte startPos, String names[], byte notes[]){
	_size = size;
	_position = startPos;
	_names = names;
	_notes = notes;
}

/* Return name of current Note */
String KeyMap::getName(){
	return _names[_position];
}

/* Return value of current Note */
byte KeyMap::getNote(){
	return _notes[_position];
}

/* Increment position of current Note */
void KeyMap::next(){
	if(_position < _size - 1) _position++;
	else _position = 0;
}

/* Decrement position of current Note */
void KeyMap::prev(){
	if(_position > 0) _position--;
	else _position = _size - 1;
}