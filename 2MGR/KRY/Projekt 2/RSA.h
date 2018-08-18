/*
* Soubor:  RSA.h
* Datum:   2018//4/28
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: Kryptografie, proj 2
* Popis:   Generovani klicu RSA, sifrovani, desifrovani, prolomeni sifry
*/

#include <cstdio>
#include <iostream>
#include <cstdint>
#include <exception>
#include <tuple>
#include <gmpxx.h>

#include "Functions.h"

namespace RSA {

	// print help
   void printHelp();

   // generate key
   std::string generateKeys(std::string input);

   // encrypt/decrypt message
   std::string cryptMessage(std::string input_1, std::string input_2, std::string input_3);

   // break cypher adn return prime
   std::string breakCypher(std::string input);

   // helper functions and structs
   namespace {
	   	// define struct to hold key attributes
		typedef struct {
			mpz_class p;
			mpz_class q;
			mpz_class n;
			mpz_class e;
			mpz_class d;
		}Key;

		// generate key attributes
		int generateKeyAttributes(Key *key, int modulus_length);

   }
}