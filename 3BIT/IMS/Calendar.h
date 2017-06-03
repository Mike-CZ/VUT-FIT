/*
* Soubor:  Calendar.h
* Datum:   2015/12/05
* Autori:  Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
*		   Jiri Komarek, xkomar27@stud.fit.vutbr.cz
* Projekt: Simulator cislicovych obvodu, projekt do predmetu IMS
* Popis:   Program nacte zdrojovou topologii v NETlistu a vygeneruje script pro gnuplot pomoci ktereho se vykresli casovy prubeh
*/

#pragma once

#include <iostream>
#include <vector>

#include "Wire.h"

/*
* Abstraktni trida procesu
*/
class Process {
public:
	virtual void Action() = 0;
	inline double getTime() const { return time_; }

	Process(double time);
	virtual ~Process();
private:
	double time_;
};

/*
* Trida pro proces hradla
*/
class GateProcess : public Process {
public:
	GateProcess(Gate* gate, double time);
	void Action();

protected:
	Gate* gate_;

};

/*
* Trida pro vstupu LOG1
*/
class InputProcess : public Process {
public:
	InputProcess(Gate* gate, double time);
	void Action();

protected:
	Gate* gate_;

};

/*
* Trida pro proces CLK
*/
class ClkProcess : public Process {
public:
	ClkProcess(Gate* gate, double time);
	void Action();

protected:
	Gate* gate_;

};

/*
* Trida pro Calendar, implementovan jako singleton
*/
class Calendar {
public:
	static Calendar* getInstance();
	static void PlanAction(Process* proc);
	void runProcess(Process* proc);
	Process* getFirst();
	void Destroy();
	bool isEmpty();

private:
	Calendar() { }
	static Calendar* instance_;
	static std::vector<Process*> processes_;
};

