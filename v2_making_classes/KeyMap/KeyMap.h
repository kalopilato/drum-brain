/*
	KeyMap.h - Library for creating and accessing MIDI Key Mappings for the "MIDI Drum Brain" Arduino project.
	Created by Kalo Pilato, 13th Feb, 2014.
	************************ LICENSE TBC	****************************** 
*/

#ifndef KeyMap_h
#define KeyMap_h

#include "Arduino.h"

class KeyMap{
public:
	KeyMap(byte size, byte startPos, String names[], byte notes[]);
	String getName();
	byte getNote();
	void next();
	void prev();
private:
	String _name;
	byte _note;
	String *_names;
	byte *_notes;
	byte _position;
	byte _size;
};

#endif