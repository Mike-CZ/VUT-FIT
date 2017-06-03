/*
* Soubor:  Gate.h
* Datum:   2015/12/05
* Autori:  Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
*		   Jiri Komarek, xkomar27@stud.fit.vutbr.cz
* Projekt: Simulator cislicovych obvodu, projekt do predmetu IMS
* Popis:   Program nacte zdrojovou topologii v NETlistu a vygeneruje script pro gnuplot pomoci ktereho se vykresli casovy prubeh
*/

#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "Wire.h"
#include "Calendar.h"

// frekvence CLK
#define CLK_DELAY 50.0

// zpozdeni hradel
#define GATE_DELAY 10.0

extern const double &Time;

/////////////////////////////////////////
/////////// ABSTRAKTNI TRIDY ////////////
/////////////////////////////////////////

/*
* Abstraktni trida pro spolecny zaklad vsech hradel
*/
class Gate {
public:
	static void ConnectWiresAndGates(std::vector<Gate*> &gates, std::vector<Wire*> &wires);

	virtual void computeOutput() = 0;
	virtual Wire *getOutputWire() = 0;
	virtual std::string getComponentInto() = 0;
	inline Value getOutputValue() const { return outputValue_; }
	inline double getDelay() const { return delay; }
	int getIntValue();
	char getCharValue();
	Gate();
	virtual ~Gate();

protected:
	virtual void connectWire_(Wire *wire) = 0;
	double delay;
	Value outputValue_;
	Value oldValue_;
};

class Generator : public Gate {
public:
	Generator(int out);
	Wire *getOutputWire() { return output_; };

protected:
	void connectWire_(Wire *wire);

	Wire *output_;
	int outputIndex_;
};

/*
* Abstraktni trida, ze ktere dedi jednotliva jednovstupa hradla
*/
class OneInGate : public Gate {
public:
	OneInGate(int in, int out);
	Wire *getOutputWire() { return output_; }

protected:
	void connectWire_(Wire *wire);

	int inputIndex_;
	int outputIndex_;
	Wire *input_;
	Wire *output_;
};

/*
* Abstraktni trida, ze ktere dedi jednotliva dvouvstupa hradla
*/
class TwoInGate : public Gate {
public:
	TwoInGate(int in1, int in2, int out);
	Wire *getOutputWire() { return output_; }

protected:
	void connectWire_(Wire *wire);

	int inputIndex1_;
	int inputIndex2_;
	int outputIndex_;
	Wire *input1_;
	Wire *input2_;
	Wire *output_;
};

////////////////////////////////////////////
//////// TRIDY PRO JEDNOTLIVA HRADLA ///////
////////////////////////////////////////////

class Input : public Generator {
public:
	Input(int out);
	void computeOutput();
	std::string getComponentInto();
};

class Clk : public Generator {
public:
	Clk(int out);
	void computeOutput();
	std::string getComponentInto();
};

/*
* Trida pro vytvareni instanci hradel NOT
*/
class NotGate : public OneInGate {
public:
	NotGate(int in, int out);
	void computeOutput();
	std::string getComponentInto();
};

/*
* Trida pro vytvareni instanci hradel AND
*/
class AndGate : public TwoInGate {
public:
	AndGate(int in1, int in2, int out);
	void computeOutput();
	std::string getComponentInto();
};

/*
* Trida pro vytvareni instanci hradel OR
*/
class OrGate : public TwoInGate {
public:
	OrGate(int in1, int in2, int out);
	void computeOutput();
	std::string getComponentInto();
};

/*
* Trida pro vytvareni instanci hradel NAND
*/
class NandGate : public TwoInGate {
public:
	NandGate(int in1, int in2, int out);
	void computeOutput();
	std::string getComponentInto();
};

/*
* Trida pro vytvareni instanci hradel NOR
*/
class NorGate : public TwoInGate {
public:
	NorGate(int in1, int in2, int out);
	void computeOutput();
	std::string getComponentInto();
};