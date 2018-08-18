/*
* Soubor:  Functions.cpp
* Datum:   2018//4/28
* Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
* Projekt: Kryptografie, proj 2
* Popis:   Generovani klicu RSA, sifrovani, desifrovani, prolomeni sifry
*/

#include "Functions.h"

namespace Functions {

	// seed rng
   	void seedRng() {
	   	// seed (this is not secure but for testing purposes it is enough)
		auto duration = std::chrono::system_clock::now().time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	   	rnd.seed(millis);
   	}

	// generate prime
	mpz_class generateRandomPrime(int primeLength) {
		mpz_class prime;

		// get prime with msb on 1
		do {
			prime = rnd.get_z_bits(primeLength) | (mpz_class(1) << (primeLength - 1));
		} while(!isPrime(prime, 20));

		return prime;
	}

	// generate randum number in given range, max inclusive
   	mpz_class generateRandomNumber(mpz_class min, mpz_class max) {
   		return min + rnd.get_z_range((max - min) + 1);
   	}

	// find gcd of num1 and num2
   	mpz_class gcd(mpz_class num1, mpz_class num2) {
   		mpz_class x = num1, y = num2;
   		mpz_class gcd;

   		while (y != 0) {
   			gcd = y;
   			y = x % y;
   			x = gcd;
   		}

   		return gcd;
   	}

   	// calculate multiplicative inverse, gcd(num, mod) has to be 1
	mpz_class multiplicativeInverse(mpz_class &num, mpz_class &mod) {
		mpz_class a = num, m = mod;
		mpz_class y = 0, x = 1;

	    if (m == 1) {
	    	return 0;
	    }

	    while (a > 1) {
	    	mpz_class q = a / m;
	    	mpz_class t = m;

	    	m = a % m;
	    	a = t;
	    	t = y;

	    	y = x - q * y;
	    	x = t;
	    }

	    if (x < 0) {
	    	x += mod;
	    }

	    return x;
	}

	// get number divisor
   	mpz_class getDivisor(mpz_class &number) {
   		mpz_class divisor = 0;

   		if (number > 0) {
   			// try simple division
   			divisor = simpleDivision(number);

   			// use pollard's factorization method
   			if (divisor <= 0) {
   				divisor = pollardsFactorization(number);
   			}
   		}

   		return divisor;
   	}

	// test if number n is prime, k is number of loops
   	bool isPrime(mpz_class &n, size_t k) {
		return miillerTest(n, k);
	}

   // hidden helper functions and variables
   namespace {

   		// simple division for first 1.000.000 numbers
        mpz_class simpleDivision(mpz_class &number) {

        	// try first 1.000.000 numbers
        	for (mpz_class divisor = 2; divisor <= 1000000; divisor++) {
        		if (number % divisor == 0 && divisor != number) {
        			return divisor;
        		}
        	}

        	// otherwise return 0
        	return mpz_class(0);
        }

        // pollard's factorization method
        mpz_class pollardsFactorization(mpz_class &number) {
        	mpz_class divisor = 1, target = number, x = 0, y = 0, c = 0;

        	// seed rng
        	seedRng();

        	// no prime divisor for 1
		    if (target == 1) 
		    	return target;
		 
		    // even number means one of the divisors is 2
		    if (target % 2 == 0) 
		    	return mpz_class(2);

		    // we will pick from the range [2, target]
		    x = generateRandomNumber(2, target);
		    y = x;

		    // the constant in f(x)
		    c = generateRandomNumber(1, target);

		    // until the prime factor isn't obtained.
       		// If n is prime, return n 
		    while (divisor == 1) {

		        // Tortoise Move: x(i+1) = f(x(i))
		        mpz_powm_ui (x.get_mpz_t(), x.get_mpz_t(), 2, target.get_mpz_t()); 
		        x = (x + c + target) % target;
		 
		        // Hare Move: y(i+1) = f(f(y(i)))
		        mpz_powm_ui (y.get_mpz_t(), y.get_mpz_t(), 2, target.get_mpz_t()); 
		        y = (y + c + target) % target;
		        mpz_powm_ui (y.get_mpz_t(), y.get_mpz_t(), 2, target.get_mpz_t()); 
		        y = (y + c + target) % target;
		 
		        // check gcd of |x-y| and n
		        divisor = gcd(abs(x-y), target);
		 
		        // retry if the algorithm fails to find prime factor
		        // with chosen x and c
		        if (divisor == target) 
		        	return pollardsFactorization(target);
		    }
		        	
		    return divisor;
		}

   		// miller rabin test
   		bool miillerTest(mpz_class &n, const size_t rounds) {
		  // Treat n==1, 2, 3 as a primes
		  if (n == 1 || n == 2 || n == 3)
		    return true;

		  // Treat negative numbers in the frontend
		  if (n <= 0)
		    return false;

		  // Even numbers larger than two cannot be prime
		  if ((n & 1) == 0)
		    return false;

		  // Write n-1 as d*2^s by factoring powers of 2 from n-1
		  size_t s = 0;
		  {
		    mpz_class m = n - 1;
		    while ((m & 1) == 0) {
		      ++s;
		      m >>= 1;
		    }
		  }
		  const mpz_class d = (n - 1) / (mpz_class(1) << s);

		  mpz_class x;
		  mpz_class a;
		  for (size_t i = 0; i < rounds; ++i) {
		    a = generateRandomNumber(2, n-2);
		    mpz_powm(x.get_mpz_t(), a.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());

		    if (x ==1 || x == (n - 1))
		      continue;

		    for (size_t r = 0; r < (s-1); ++r) {

		      mpz_powm(x.get_mpz_t(), x.get_mpz_t(), mpz_class(2).get_mpz_t(), n.get_mpz_t());

		      if (x == 1) {
		        // Definitely not a prime
		        return false;
		      }
		      if (x == (n - 1))
		        break;
		    }

		    if (x != (n - 1)) {
		      // Definitely not a prime
		      return false;
		    }
		  }

		  // Might be prime
		  return true;
		}
   }
}