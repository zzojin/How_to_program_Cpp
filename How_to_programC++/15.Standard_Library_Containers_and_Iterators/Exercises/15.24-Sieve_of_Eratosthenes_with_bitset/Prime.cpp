/**
 * Class implementation for prime numbers.
 * Provides factorization with algorithm Sieve of Eratosthenes.
 */

#include <iostream>
#include <cmath>
#include <unordered_map>
#include "Prime.h"

Prime::Prime()
    : Prime(0)
{
}

Prime::Prime(size_t a_number)
    : m_number(a_number)
{
}

Prime::~Prime()
{
}

bool Prime::isPrime() const
{
    return isPrime(getNumber());
}


/** 
 * Calculate prime number
 */

bool Prime::isPrime(size_t a_number) const
{
	// Take only half a number
	size_t half = a_number / 2;
	std::vector<bool> num(half);

	for (size_t i = 2; i < half; ) {
		toggleMultipleBits(num, i);

		if (a_number % i == 0) {
			return false;
		}

		// increment counter to appropriate value
		i = findFirstPrime(num, i);
	}

	return true;
}


/** 
 * Calculate prime number and return vector of prime numbers
 */

bool Prime::isPrime(size_t a_number, std::vector<bool>* a_primes) const
{
	// Take only half a number
	size_t half = a_number / 2;
	std::vector<bool>& num = *a_primes;

	for (size_t i = 2; i < half; ) {
		toggleMultipleBits(num, i);

		if (a_number % i == 0) {
			return false;
		}

		// increment counter to appropriate value
		i = findFirstPrime(num, i);
	}

	return true;
}

/**
 * Non-prime number factorization or shorter factorization.
 * Function uses algorithm Sieve of Eratosthenes.
 */

void Prime::factorization() const
{
	/* List of prime factorization */
	std::unordered_map<size_t, size_t> primeNumbers;
	
	size_t number = getNumber();
	
	/* Factorize only half the number values */
	size_t halfNumber = number / 2;
	std::vector<bool> listOfPrimes(halfNumber);

	size_t tmpNumber(number);

	/* Prime factorization (excluding trivial 1) */
	for (size_t primeIndex = 2; tmpNumber > 1 && primeIndex < halfNumber; )
	{
		if ((tmpNumber % primeIndex) != 0)
		{
			/* Check next prime factor */
			primeIndex = findFirstPrime(listOfPrimes, ++primeIndex);
			continue;
		}

		/* Determine prime factor */
		tmpNumber /= primeIndex;

		if (primeNumbers.find(primeIndex) != primeNumbers.end()) {
			++primeNumbers.at( primeIndex );
		}
		else {
			/* Create prime number entry */
			primeNumbers.insert( std::make_pair(primeIndex, 1) );
		}
	}

	/* Trivial prime number factorization */
	if (primeNumbers.size() < 1) {
		primeNumbers.insert( std::make_pair(1, 1) );
		primeNumbers.insert( std::make_pair(number, 1) );
	}

	for (std::unordered_map<size_t,size_t>::const_iterator prime = primeNumbers.cbegin(); 
	     prime != primeNumbers.cend(); ++prime) {
		std::cout << prime->first << ": " << prime->second << std::endl;
	}
	
	/* Test factorization */
	size_t product(1);
	for (auto prime = primeNumbers.cbegin(); prime != primeNumbers.cend(); ++prime) {
		product *= std::pow(prime->first, prime->second);
	}
	std::cout << "Total product: " << product << std::endl;
}


/**
 *  Toggle specified index bits with index multiplication factor
 */

void Prime::toggleMultipleBits(std::vector<bool>& a_vec, size_t index) const
{
	for (size_t i = index; i < a_vec.size(); i += index)
	{
		if (!a_vec.at(i)) {
			a_vec.at(i) = true;
		}
	}
}


/**
 * Search for first zero bit after (including) start bit index
 */

size_t Prime::findFirstPrime(const std::vector<bool>& a_vec, size_t a_startIndex) const
{
	for (size_t i = a_startIndex; i < a_vec.size(); ++i)
	{
		if (!a_vec.at(i)) {
			return i;
		}
	}

	return a_vec.size(); // off by one!
}
