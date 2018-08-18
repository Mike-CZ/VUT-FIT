/*
* Soubor:  Functions.h
* Datum:   2018//4/28
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: Kryptografie, proj 2
* Popis:   Generovani klicu RSA, sifrovani, desifrovani, prolomeni sifry
*/

#include <cstdio>
#include <iostream>
#include <cstdint>
#include <tuple>
#include <gmpxx.h>
#include <chrono>

// The miller rabin prime test function is from this source: https://github.com/cslarsen/miller-rabin
// Modular multiplication inverse is form this source: https://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/
// Pollard's factorization method is from this source: https://www.geeksforgeeks.org/pollards-rho-algorithm-prime-factorization/

namespace Functions {

   // seed rng
   void seedRng();

	// test if number n is prime, k is number of loops
   bool isPrime(mpz_class &n, size_t k);

   // generate randum number in given range, max inclusive
   mpz_class generateRandomNumber(mpz_class min, mpz_class max);

   // generate random prime
   mpz_class generateRandomPrime(int primeLength);

   // calculate multiplicative inverse, gcd(num, mod) has to be 1
   mpz_class multiplicativeInverse(mpz_class &num, mpz_class &mod);

   // find gcd of num1 and num2
   mpz_class gcd(mpz_class num1, mpz_class num2);

   // get number divisor
   mpz_class getDivisor(mpz_class &number);

   // hidden helper functions and variables
   namespace {
         // generate "random" class to generate numbers
         gmp_randclass rnd (gmp_randinit_default);
   
        	// miller rabin test
   		bool miillerTest(mpz_class &n, const size_t rounds);

         // simple division for first 1.000.000 numbers
         mpz_class simpleDivision(mpz_class &number);

         // pollard's factorization method
         mpz_class pollardsFactorization(mpz_class &number);

   }

}