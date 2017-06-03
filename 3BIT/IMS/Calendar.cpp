/*
* Soubor:  Calendar.cpp
* Datum:   2015/12/05
* Autori:  Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
*		   Jiri Komarek, xkomar27@stud.fit.vutbr.cz
* Projekt: Simulator cislicovych obvodu, projekt do predmetu IMS
* Popis:   Program nacte zdrojovou topologii v NETlistu a vygeneruje script pro gnuplot pomoci ktereho se vykresli casovy prubeh
*/

#include "Calendar.h"
#include "Gate.h"

// inicializace 
Calendar* Calendar::instance_ = NULL;
std::vector<Process*> Calendar::processes_;

/*
* Konstruktory
*/
Process::Process(double time) {
	time_ = time;
}

GateProcess::GateProcess(Gate* gate, double time) : Process(time) {
	gate_ = gate;
}

InputProcess::InputProcess(Gate* gate, double time) : Process(time) {
	gate_ = gate;
}

ClkProcess::ClkProcess(Gate* gate, double time) : Process(time) {
	gate_ = gate;
}

/*
* Destruktor
*/
Process::~Process() { }

/*
* Test prazdnosti kalendare
*/
bool Calendar::isEmpty() {
	return processes_.empty();
}

/*
* Funkce vrati instanci
*/
Calendar* Calendar::getInstance() {
	if (instance_ == NULL) {
		instance_ = new Calendar;
	}
	return instance_;
}

/*
* Znici kalendar a procesy ulozene v nem
*/
void Calendar::Destroy() {
	for (std::vector<Process*>::size_type i = 0; i != processes_.size(); ++i) {
		delete processes_[i];
	}
	processes_.clear();

	if (Calendar::instance_ != NULL) {
		delete Calendar::instance_;
	}
	Calendar::instance_ = NULL;
}

/*
* Vykona proces a smaze instanci
*/
void Calendar::runProcess(Process* proc) {
	proc->Action();
	delete proc;
}

/*
* Vlozi udalost do kalendare
*/
void Calendar::PlanAction(Process* proc) {
	processes_.push_back(proc);
}


/*
* Vrati ukazatel na proces s nejnizsim aktivacnim casem a vyjme jej z kalendare
*/
Process* Calendar::getFirst() {
	Process * tmp = NULL;
	std::vector<Process*>::size_type deteleIndex = 0;

	if (processes_.empty()) {
		return NULL;
	}
	else {
		tmp = processes_.front();
		for (std::vector<Process*>::size_type i = 0; i != processes_.size(); ++i) {
			if (processes_[i]->getTime() < tmp->getTime()) {
				tmp = processes_[i];
				deteleIndex = i;
			}
		}
	}
	processes_.erase(processes_.begin() + deteleIndex);
	return tmp;
}

/*
* Zahajeni procesu
*/
void GateProcess::Action() {
	Value oldVal = gate_->getOutputValue();

	// spocitame novou hodnotu hradla
	gate_->computeOutput();

	// pokud je hodnota jina, obeznamime propojena hradla a naplanujeme udalost za cas odpovidajici zpozdeni hradla
	if (oldVal != gate_->getOutputValue()) {
		std::vector<Gate*>devices = gate_->getOutputWire()->getOutputDevices();
		for (std::vector<Gate*>::size_type i = 0; i != devices.size(); ++i) {
			Calendar::PlanAction(new GateProcess(devices[i], Time + devices[i]->getDelay()));
		}
	}
}

void InputProcess::Action() {
	gate_->computeOutput();
	std::vector<Gate*>devices = gate_->getOutputWire()->getOutputDevices();
	for (std::vector<Gate*>::size_type i = 0; i != devices.size(); ++i) {
		Calendar::PlanAction(new GateProcess(devices[i], Time + devices[i]->getDelay()));
	}
}

void ClkProcess::Action() {
	gate_->computeOutput();
	std::vector<Gate*>devices = gate_->getOutputWire()->getOutputDevices();
	for (std::vector<Gate*>::size_type i = 0; i != devices.size(); ++i) {
		Calendar::PlanAction(new GateProcess(devices[i], Time + devices[i]->getDelay()));
	}
	// naplanuj sam sebe
	Calendar::PlanAction(new ClkProcess(gate_, Time + gate_->getDelay()));
}
