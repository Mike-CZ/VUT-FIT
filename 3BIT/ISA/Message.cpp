/**
* ISA project
* Name: Klient SIP
* Author: Lukas Pelanek, xpelan03@stud.fit.vutbr.cz, 2015
**/

#include "Message.h"

/**
* Get ip address
* @return string containing ip address
*/
string Message::getIP(){
	return this->ipAddress;
}

/**
* Get message
* @return string containing message
*/
string Message::getMessage(){
	return this->message;
}

/**
* Get message
* @return string containing port
*/
string Message::getPort(){
	return this->port;
}

/**
* Get message
* @return string containing raw ip
*/
string Message::getRawIP(){
	return this->rawIP;
}
