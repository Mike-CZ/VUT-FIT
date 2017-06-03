/*
* Soubor:  Gate.cpp
* Datum:   2015/12/05
* Autori:  Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
*		   Jiri Komarek, xkomar27@stud.fit.vutbr.cz
* Projekt: Simulator cislicovych obvodu, projekt do predmetu IMS
* Popis:   Program nacte zdrojovou topologii v NETlistu a vygeneruje script pro gnuplot pomoci ktereho se vykresli casovy prubeh
*/

#include <string>
#include "Gate.h"

/*
* Konstruktor pro abstraktni tridu hradel
*/
Gate::Gate() {
	outputValue_ = V_UNDEFINED;
	oldValue_ = V_UNDEFINED;
	delay = GATE_DELAY;
}

/*
* Destruktor
*/
Gate::~Gate() { }


/*
* Funkce vrati hodnotu hradla v hodnote char.. hodnoty X, 0, 1
*/
char Gate::getCharValue() {
	if (outputValue_ == V_UNDEFINED) {
		return 'X';
	}
	else if (outputValue_ == V_LOG_0) {
		return '0';
	}
	else if (outputValue_ == V_LOG_1) {
		return '1';
	}
	return 'X';
}


/*
* Funkce vrati int hodnotu stare/aktualni hodnoty na vystupu
*/
int Gate::getIntValue() {
	switch (outputValue_) {
		case V_UNDEFINED:
			return 0;

		case V_LOG_0:
			return 1;

		case V_LOG_1:
			return 2;

		default:
			return 0;
	}
}

/*
* Konstruktor pro abstraktni tridu dvouvstupych hradel
*/
TwoInGate::TwoInGate(int in1, int in2, int out) : Gate() {
	inputIndex1_ = in1;
	inputIndex2_ = in2;
	outputIndex_ = out;
	input1_ = NULL;
	input2_ = NULL;
	output_ = NULL;
}

/*
* Konstruktor pro abstraktni tridu jednovstupych hradel
*/
OneInGate::OneInGate(int in, int out) : Gate() {
	inputIndex_ = in;
	outputIndex_ = out;
	input_ = NULL;
	output_ = NULL;
}

/*
* Konstruktor pro generatory
*/
Generator::Generator(int out) {
	outputValue_ = V_LOG_1;
	outputIndex_ = out;
	output_ = NULL;
}

/*
* Konstruktor pro Input
*/
Input::Input(int out) : Generator(out) {
	// rovnou naplanujeme pozdejsi spusteni
	Calendar::PlanAction(new InputProcess(this, Time));
	delay = 0;
}

/*
* Konstruktor pro hodinovy signal
*/
Clk::Clk(int out) : Generator(out) {
	Calendar::PlanAction(new ClkProcess(this, Time));
	delay = CLK_DELAY;
}

/*
* Konstruktor pro hradla NOT
*/
NotGate::NotGate(int in, int out) : OneInGate(in, out) { }

/*
* Konstruktor pro hradla AND
*/
AndGate::AndGate(int in1, int in2, int out) : TwoInGate(in1, in2, out) { }

/*
* Konstruktor pro hradla OR
*/
OrGate::OrGate(int in1, int in2, int out) : TwoInGate(in1, in2, out) { }

/*
* Konstruktor pro hradla NAND
*/
NandGate::NandGate(int in1, int in2, int out) : TwoInGate(in1, in2, out) { }

/*
* Konstruktor pro hradla NOR
*/
NorGate::NorGate(int in1, int in2, int out) : TwoInGate(in1, in2, out) { }


/*
* Input posle na vystup vzdy log 1
*/
void Input::computeOutput() {
	oldValue_ = outputValue_;
	outputValue_ = V_LOG_1;

	// jestlize je pripojen vystup, posleme na nej hodnotu
	if (output_ != NULL) {
		output_->putValueOnWire(outputValue_);
	}
}

/*
* Clk vzdy invertuje svoji predchozi hodnotu
*/
void Clk::computeOutput() {
	oldValue_ = outputValue_;
	outputValue_ = (outputValue_ == V_LOG_1) ? V_LOG_0 : V_LOG_1;

	// jestlize je pripojen vystup, posleme na nej hodnotu
	if (output_ != NULL) {
		output_->putValueOnWire(outputValue_);
	}
}

/*
* Implementace chovani hradla NOT
*/
void NotGate::computeOutput() {
	oldValue_ = outputValue_;
	Value tmpVal = (input_ != NULL) ? input_->getWireValue() : V_UNDEFINED;

	// invertuji hodnotu na vstupu
	if (tmpVal == V_LOG_0) {
		tmpVal = V_LOG_1;
	}
	else if (tmpVal == V_LOG_1) {
		tmpVal = V_LOG_0;
	}

	// jestlize je pripojen vystup, posleme na nej hodnotu
	if (output_ != NULL) {
		output_->putValueOnWire(tmpVal);
	}

	// ulozime aktualni vystup hradla
	outputValue_ = tmpVal;
}

/*
* Implementace chovani hradla AND
*/
void AndGate::computeOutput() {
	oldValue_ = outputValue_;
	Value tmpVal1 = (input1_ != NULL) ? input1_->getWireValue() : V_UNDEFINED;
	Value tmpVal2 = (input2_ != NULL) ? input2_->getWireValue() : V_UNDEFINED;
	Value tmpOut = V_UNDEFINED;

	// prirazeni vystupni hodnoty podle pravdivostni tabulky logickeho clenu AND
	if (tmpVal1 == V_LOG_0 && tmpVal2 == V_LOG_0) {
		tmpOut = V_LOG_0;
	}
	else if (tmpVal1 == V_LOG_0 && tmpVal2 == V_LOG_1) {
		tmpOut = V_LOG_0;
	}
	else if (tmpVal1 == V_LOG_0 && tmpVal2 == V_UNDEFINED) {
		tmpOut = V_LOG_0;
	}
	else if (tmpVal1 == V_LOG_1 && tmpVal2 == V_LOG_0) {
		tmpOut = V_LOG_0;
	}
	else if (tmpVal1 == V_LOG_1 && tmpVal2 == V_LOG_1) {
		tmpOut = V_LOG_1;
	}
	else if (tmpVal1 == V_LOG_1 && tmpVal2 == V_UNDEFINED) {
		tmpOut = V_UNDEFINED;
	}
	else if (tmpVal1 == V_UNDEFINED && tmpVal2 == V_LOG_0) {
		tmpOut = V_LOG_0;
	}
	else if (tmpVal1 == V_UNDEFINED && tmpVal2 == V_LOG_1) {
		tmpOut = V_UNDEFINED;
	}
	else if (tmpVal1 == V_UNDEFINED && tmpVal2 == V_UNDEFINED) {
		tmpOut = V_UNDEFINED;
	}

	// jestlize je pripojen vystup, posleme na nej hodnotu
	if (output_ != NULL) {
		output_->putValueOnWire(tmpOut);
	}

	// ulozime aktualni vystup hradla
	outputValue_ = tmpOut;
}

/*
* Implementace chovani hradla OR
*/
void OrGate::computeOutput() {
	oldValue_ = outputValue_;
	Value tmpVal1 = (input1_ != NULL) ? input1_->getWireValue() : V_UNDEFINED;
	Value tmpVal2 = (input2_ != NULL) ? input2_->getWireValue() : V_UNDEFINED;
	Value tmpOut = V_UNDEFINED;

	// prirazeni vystupni hodnoty podle pravdivostni tabulky logickeho clenu OR
	if (tmpVal1 == V_LOG_0 && tmpVal2 == V_LOG_0) {
		tmpOut = V_LOG_0;
	}
	else if (tmpVal1 == V_LOG_0 && tmpVal2 == V_LOG_1) {
		tmpOut = V_LOG_1;
	}
	else if (tmpVal1 == V_LOG_0 && tmpVal2 == V_UNDEFINED) {
		tmpOut = V_UNDEFINED;
	}
	else if (tmpVal1 == V_LOG_1 && tmpVal2 == V_LOG_0) {
		tmpOut = V_LOG_1;
	}
	else if (tmpVal1 == V_LOG_1 && tmpVal2 == V_LOG_1) {
		tmpOut = V_LOG_1;
	}
	else if (tmpVal1 == V_LOG_1 && tmpVal2 == V_UNDEFINED) {
		tmpOut = V_LOG_1;
	}
	else if (tmpVal1 == V_UNDEFINED && tmpVal2 == V_LOG_0) {
		tmpOut = V_UNDEFINED;
	}
	else if (tmpVal1 == V_UNDEFINED && tmpVal2 == V_LOG_1) {
		tmpOut = V_LOG_1;
	}
	else if (tmpVal1 == V_UNDEFINED && tmpVal2 == V_UNDEFINED) {
		tmpOut = V_UNDEFINED;
	}

	// jestlize je pripojen vystup, posleme na nej hodnotu
	if (output_ != NULL) {
		output_->putValueOnWire(tmpOut);
	}

	// ulozime aktualni vystup hradla
	outputValue_ = tmpOut;
}

/*
* Implementace chovani hradla NAND
*/
void NandGate::computeOutput() {
	oldValue_ = outputValue_;
	Value tmpVal1 = (input1_ != NULL) ? input1_->getWireValue() : V_UNDEFINED;
	Value tmpVal2 = (input2_ != NULL) ? input2_->getWireValue() : V_UNDEFINED;
	Value tmpOut = V_UNDEFINED;

	// prirazeni vystupni hodnoty podle pravdivostni tabulky logickeho clenu NAND
	if (tmpVal1 == V_LOG_0 && tmpVal2 == V_LOG_0) {
		tmpOut = V_LOG_1;
	}
	else if (tmpVal1 == V_LOG_0 && tmpVal2 == V_LOG_1) {
		tmpOut = V_LOG_1;
	}
	else if (tmpVal1 == V_LOG_0 && tmpVal2 == V_UNDEFINED) {
		tmpOut = V_LOG_1;
	}
	else if (tmpVal1 == V_LOG_1 && tmpVal2 == V_LOG_0) {
		tmpOut = V_LOG_1;
	}
	else if (tmpVal1 == V_LOG_1 && tmpVal2 == V_LOG_1) {
		tmpOut = V_LOG_0;
	}
	else if (tmpVal1 == V_LOG_1 && tmpVal2 == V_UNDEFINED) {
		tmpOut = V_UNDEFINED;
	}
	else if (tmpVal1 == V_UNDEFINED && tmpVal2 == V_LOG_0) {
		tmpOut = V_LOG_1;
	}
	else if (tmpVal1 == V_UNDEFINED && tmpVal2 == V_LOG_1) {
		tmpOut = V_UNDEFINED;
	}
	else if (tmpVal1 == V_UNDEFINED && tmpVal2 == V_UNDEFINED) {
		tmpOut = V_UNDEFINED;
	}

	// jestlize je pripojen vystup, posleme na nej hodnotu
	if (output_ != NULL) {
		output_->putValueOnWire(tmpOut);
	}

	// ulozime aktualni vystup hradla
	outputValue_ = tmpOut;
}

/*
* Implementace chovani hradla NOR
*/
void NorGate::computeOutput() {
	oldValue_ = outputValue_;
	Value tmpVal1 = (input1_ != NULL) ? input1_->getWireValue() : V_UNDEFINED;
	Value tmpVal2 = (input2_ != NULL) ? input2_->getWireValue() : V_UNDEFINED;
	Value tmpOut = V_UNDEFINED;

	// prirazeni vystupni hodnoty podle pravdivostni tabulky logickeho clenu NOR
	if (tmpVal1 == V_LOG_0 && tmpVal2 == V_LOG_0) {
		tmpOut = V_LOG_1;
	}
	else if (tmpVal1 == V_LOG_0 && tmpVal2 == V_LOG_1) {
		tmpOut = V_LOG_0;
	}
	else if (tmpVal1 == V_LOG_0 && tmpVal2 == V_UNDEFINED) {
		tmpOut = V_UNDEFINED;
	}
	else if (tmpVal1 == V_LOG_1 && tmpVal2 == V_LOG_0) {
		tmpOut = V_LOG_0;
	}
	else if (tmpVal1 == V_LOG_1 && tmpVal2 == V_LOG_1) {
		tmpOut = V_LOG_0;
	}
	else if (tmpVal1 == V_LOG_1 && tmpVal2 == V_UNDEFINED) {
		tmpOut = V_LOG_0;
	}
	else if (tmpVal1 == V_UNDEFINED && tmpVal2 == V_LOG_0) {
		tmpOut = V_UNDEFINED;
	}
	else if (tmpVal1 == V_UNDEFINED && tmpVal2 == V_LOG_1) {
		tmpOut = V_LOG_0;
	}
	else if (tmpVal1 == V_UNDEFINED && tmpVal2 == V_UNDEFINED) {
		tmpOut = V_UNDEFINED;
	}

	// jestlize je pripojen vystup, posleme na nej hodnotu
	if (output_ != NULL) {
		output_->putValueOnWire(tmpOut);
	}

	// ulozime aktualni vystup hradla
	outputValue_ = tmpOut;
}

/*
* Funkce pro dvouvstupa hradla, propoji drat na prislusny pin hradla 
*/
void TwoInGate::connectWire_(Wire *wire) {

	// jestli ze index pinu odpovida indexu dratu, propojime komponenty
	if (inputIndex1_ == wire->getIndex()) {
		wire->connectOut();
		wire->pushOutputDevice(this);
		input1_ = wire;
	}

	if (inputIndex2_ == wire->getIndex()) {
		wire->connectOut();
		wire->pushOutputDevice(this);
		input2_ = wire;
	}
	
	if (outputIndex_ == wire->getIndex()) {
		wire->connectIn();
		output_ = wire;
	}
}

/*
* Funkce pro jednovstupa hradla, propoji drat na prislusny pin hradla
*/
void OneInGate::connectWire_(Wire *wire) {

	// jestli ze index pinu odpovida indexu dratu, propojime komponenty
	if (inputIndex_ == wire->getIndex()) {
		wire->connectOut();
		wire->pushOutputDevice(this);
		input_ = wire;
	}
	if (outputIndex_ == wire->getIndex()) {
		wire->connectIn();
		output_ = wire;
	}
}

/*
* Pripoji generator na vstup
*/
void Generator::connectWire_(Wire *wire) {
	if (outputIndex_ == wire->getIndex()) {
		wire->connectIn();
		output_ = wire;
	}
}

/*
* Funkce propoji komponenty a piny na zaklade prirazenych ciselnych pinu
*/
void Gate::ConnectWiresAndGates(std::vector<Gate*> &gates, std::vector<Wire*> &wires) {
	for (std::vector<Gate*>::size_type i = 0; i != gates.size(); ++i) {
		for (std::vector<Wire*>::size_type x = 0; x != wires.size(); ++x) {
			gates[i]->connectWire_(wires[x]);
		}
	}
}


/*
* Funkce vypisi informace o soucastkach
*/
std::string Input::getComponentInto() {
	return "IN(" + std::to_string(outputIndex_) + ")";
}
std::string Clk::getComponentInto() {
	return "CLK(" + std::to_string(outputIndex_) + ")";
}
std::string NotGate::getComponentInto() {
	return "NOT(" + std::to_string(inputIndex_) + ":" + std::to_string(outputIndex_) + ")";
}
std::string AndGate::getComponentInto() {
	return "AND(" + std::to_string(inputIndex1_) + ":" + std::to_string(inputIndex2_) + ":" + std::to_string(outputIndex_) + ")";
}
std::string OrGate::getComponentInto() {
	return "OR(" + std::to_string(inputIndex1_) + ":" + std::to_string(inputIndex2_) + ":" + std::to_string(outputIndex_) + ")";
}
std::string NandGate::getComponentInto() {
	return "NAND(" + std::to_string(inputIndex1_) + ":" + std::to_string(inputIndex2_) + ":" + std::to_string(outputIndex_) + ")";
}
std::string NorGate::getComponentInto() {
	return "NOR(" + std::to_string(inputIndex1_) + ":" + std::to_string(inputIndex2_) + ":" + std::to_string(outputIndex_) + ")";
}