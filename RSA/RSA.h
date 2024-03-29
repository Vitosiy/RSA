#pragma once
#include "BigNum.h"
#include <mpirxx.h>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <locale>
#include <Windows.h>
#include <io.h>



class RSA
{
private:
	static BigNum e; // public key
	static BigNum d; // private key
	static BigNum n; // mod

	//--------------------------------------------------------------------------------------------------------//

	static void generatePQ(BigNum&, BigNum&, int);
	static BigNum calculateD(BigNum&, BigNum&, BigNum& p, BigNum& q);
	static void calculateE();
public:
	RSA() = delete;
	static void encode(const std::string&, const std::string&, const unsigned int);
	static void decode(const std::string&, const std::string&);
};
