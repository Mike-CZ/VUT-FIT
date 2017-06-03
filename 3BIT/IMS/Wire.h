/*
* Soubor:  Wire.h
* Datum:   2015/12/05
* Autori:  Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
*		   Jiri Komarek, xkomar27@stud.fit.vutbr.cz
* Projekt: Simulator cislicovych obvodu, projekt do predmetu IMS
* Popis:   Program nacte zdrojovou topologii v NETlistu a vygeneruje script pro gnuplot pomoci ktereho se vykresli casovy prubeh
*/

#pragma once

#include <vector>

class Gate;

/*
* Vyctovy typ reprezentujici hodnotu na dratu
*/
enum Value {
	V_LOG_0 = 0,
	V_LOG_1 = 1,
	V_UNDEFINED = 2
};

class Wire {
public:
	static void AddWire(std::vector<Wire*> &wires, int index);

	inline Value getWireValue() const { return value_; }
	inline std::vector<Gate*> getOutputDevices() const { return outputDevices_; }
	inline int getIndex() const { return index_; }
	inline bool isConnected() const { return inConnected_ && outConnected_; }
	inline bool isInConnected() const { return inConnected_; }
	inline bool isOutConnected() const { return outConnected_; }
	inline void connectIn() { inConnected_ = true; }
	inline void connectOut() { outConnected_ = true; }
	inline void putValueOnWire(Value value) { value_ = value; }
	inline void pushOutputDevice(Gate *gate) { outputDevices_.push_back(gate); }
	Wire(int index);

protected:
	Value value_;
	int index_;
	bool inConnected_;
	bool outConnected_;
	std::vector<Gate*> outputDevices_;
};
