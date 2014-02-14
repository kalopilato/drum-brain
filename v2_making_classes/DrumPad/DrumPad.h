/*
	DrumPad.h - Library for creating and accessing drumpads for the "MIDI Drum Brain" Arduino project.
	Created by Kalo Pilato, 13th Feb, 2014.
	************************ LICENSE TBC	****************************** 
*/

#ifndef DrumPad_h
#define DrumPad_h

#include "Arduino.h"

class DrumPad{
public:
	DrumPad(String name, byte note, byte pin);
	String getName();
	byte getNote();
	byte getPin();
	void setNote(byte newNote);
private:
	String _name;
	byte _note;
	byte _pin;
};

#endif