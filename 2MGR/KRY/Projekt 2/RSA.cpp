/*
* Soubor:  RSA.cpp
* Datum:   2018//4/28
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: Kryptografie, proj 2
* Popis:   Generovani klicu RSA, sifrovani, desifrovani, prolomeni sifry
*/

#include "RSA.h"

namespace RSA {

	// generate keys
	std::string generateKeys(std::string input) {
		std::string result;
		Key key;
		int modulusLength;

		try {
			modulusLength = std::stoi(input);
		}
		catch(std::exception& e) {
			std::cerr << "Verejny modulus musi byt zadan jako cislo" << std::endl;
			return result;
		}

		// check modulus length
		if (modulusLength < 6) {
			std::cerr << "Minimalni delka verejneho modulu musi byt 6 bitu." << std::endl;
			return result;
		}

		// generate key attributes
		if (generateKeyAttributes(&key, modulusLength) < 0) {
			return result;
		}

		// store result into string
		result = "0x" + key.p.get_str(16) + " 0x" + key.q.get_str(16) + " 0x" + key.n.get_str(16) + " 0x" + key.e.get_str(16) + " 0x" + key.d.get_str(16);

		return result;
	}

	// encrypt/decrypt message
   	std::string cryptMessage(std::string input_1, std::string input_2, std::string input_3) {
   		std::string result;
   		mpz_class exponent = 0, modulus = 0, message = 0, cipher = 0;

   		// check given values
   		if (input_1.substr(0, 2) != "0x" || input_2.substr(0, 2) != "0x" || input_3.substr(0, 2) != "0x") {
   			std::cerr << "Vstupni hodnoty musi byt v hexadecimalnim tvaru a zacinat prefixem 0x" << std::endl;
   			return result;
   		}

   		// read input values
   		exponent.set_str(input_1.substr(2), 16); 
   		modulus.set_str(input_2.substr(2), 16); 
   		message.set_str(input_3.substr(2), 16); 

   		// encrypt/decrypt message message
   		mpz_powm(cipher.get_mpz_t(), message.get_mpz_t(), exponent.get_mpz_t(), modulus.get_mpz_t());

   		// store result into string
   		result = "0x" + cipher.get_str(16);

   		return result;
   	}

   	// break cypher adn return prime
   	std::string breakCypher(std::string input) {
   		std::string result;
   		mpz_class prime = 0, modulus = 0;

   		// check given value
   		if (input.substr(0, 2) != "0x") {
   			std::cerr << "Vstupni hodnota musi byt v hexadecimalnim tvaru a zacinat prefixem 0x" << std::endl;
   			return result;
   		}

   		// read input value
   		modulus.set_str(input.substr(2), 16); 

   		// get prime
   		prime = Functions::getDivisor(modulus);

   		if (prime <= 0) {
   			std::cerr << "Nepodarilo se nalezt prvocislo" << std::endl;
   			return result;
   		}

   		// store result into string
   		result = "0x" + prime.get_str(16);

   		return result;
   	}

	// print help
	void printHelp() {
		std::cout << "Generovani klicu:" << std::endl;
		std::cout << "vstup: ./kry -g B" << std::endl;
		std::cout << "vystup: P Q N E D" << std::endl;
		std::cout << "Sifrovani:" << std::endl;
		std::cout << "vstup: ./kry -e E N M" << std::endl;
		std::cout << "vystup: C" << std::endl;
		std::cout << "Desifrovani:" << std::endl;
		std::cout << "vstup: ./kry -d D N C" << std::endl;
		std::cout << "vystup: M" << std::endl;
		std::cout << "Prolomeni RSA:" << std::endl;
		std::cout << "vstup: ./kry -b N" << std::endl;
		std::cout << "vystup: P" << std::endl;
		std::cout << "B ... pozadovana velikost verejneho modulu v bitech (napr. 1024)" << std::endl;
		std::cout << "P ... prvociclo (pri generovani nahodne)" << std::endl;
		std::cout << "Q ... jine prvocislo (pri generovani nahodne)" << std::endl;
		std::cout << "N ... verejny modulus" << std::endl;
		std::cout << "E ... verejny exponent" << std::endl;
		std::cout << "D ... soukromy exponent" << std::endl;
		std::cout << "M ... otevrena zprava (cislo)" << std::endl;
		std::cout << "C ... zasifrovana zprava (cislo)" << std::endl;
		std::cout << "VSECHNA CISLA NA VSTUPU I VYSTUPU (KROME B) JSOU HEXADECIMALNI A ZACINAJI 0x" << std::endl;
	}

	// helper functions
	namespace {

		// generate key attributes
		int generateKeyAttributes(Key *key, int modulusLength) {
			mpz_class p, q, n, phi, e, d;

			// set e to 65537 if modulus length is greater than 2048, othwerwise set 3
			e = modulusLength > 2048 ? 65537 : 3;

			// seed rng
			Functions::seedRng();

			// generate primes and calculate phi where gcd(e, phi) is 1
			do {
				p = Functions::generateRandomPrime(modulusLength / 2);
				q = Functions::generateRandomPrime(modulusLength / 2);

				// calculate phi
				phi = (p - 1) * (q - 1);

			} while(Functions::gcd(e, phi) != 1);

			// multiply primes
			n = p * q;

			// multiplicative inverse
			d = Functions::multiplicativeInverse(e, phi);

			// set key attributes
			key->p = p;
			key->q = q;
			key->n = n;
			key->e = e;
			key->d = d;

			return 0;
		}
	}
}