#include "RSA.h"

#define BlockSize 125 //значения в байтах
#define p_q_size 512 //значение в битах

void RSA::generatePQ(BigNum& _p, BigNum& _q, int bit) {
	mpz_t p,q;
	mpz_init(p);
	mpz_init(q);
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	mpz_urandomb(p, state, bit);
	mpz_urandomb(q, state, bit);
	mpz_nextprime(p, p);
	mpz_nextprime(q, q);
	_p = std::string(mpz_get_str(NULL, 16, p));
	_q = std::string(mpz_get_str(NULL, 16, q));
}

void RSA::calculateE() {
	long long ferm[] = { 3, 5, 17, 257, 65537, 4294967297 };
	srand(time(NULL));
	e = BigNum(ferm[rand() % 5]);
}

BigNum RSA::calculateD(BigNum& e, BigNum& phi, BigNum& p, BigNum& q) {
	BigNum d, y;
	BigNum g = BigNum::Evk(e, phi, d, y);
	d = (d % phi + phi) % phi;
	return d;
}

//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//

void RSA::encode(const std::string& pathToInputText, const std::string& pathToPublicKey, const unsigned int mode) {
	std::ifstream fileText(pathToInputText, std::ios::binary | std::ios::in);
	if (!fileText) {
		std::cout << "Wrong path to text" << std::endl;
		return;
	}
	if (mode == 0) {
		BigNum p, q;
		generatePQ(p, q, p_q_size);
		n = p * q;
		BigNum phi = (p - BigNum(1)) * (q - BigNum(1));
		calculateE();
		d = calculateD(e, phi, p, q);
		std::ofstream eFile("publicKey.txt", std::ios::out), dFile("privateKey.txt", std::ios::out);
		e.PrintF(eFile,1);
		n.PrintF(eFile,1);
		d.PrintF(dFile,1);
		n.PrintF(dFile,1);
		eFile.close();
		dFile.close();
	}
	else {
		std::ifstream eFile(pathToPublicKey, std::ios::in | std::ios::binary);
		BigNum e1(4, 0, eFile);
		BigNum n1(BlockSize, 4, eFile);

		e = e1;
		n = n1;

		eFile.close();
	}

	std::ofstream fileOutputText("outputEncode.txt", std::ios::out);

	fileText.seekg(0, std::ios::end);
	int sizeFile = fileText.tellg();
	for (int i = 0; i < sizeFile; i += BlockSize)
	{
		BigNum tmp(BlockSize, i, fileText);
		BigNum res = BigNum::FastPow(tmp, e, n);
		res.PrintF(fileOutputText,1);
		if (i > sizeFile) break;
	}
	
	fileText.close();
	fileOutputText.close();
}

//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//

void RSA::decode(const std::string& pathToText, const std::string& pathToPrivateKey) {
	ifstream dFile("privateKey.txt", std::ios::binary | std::ios::in), fileText("outputEncode.txt", std::ios::binary | std::ios::in);
	
	BigNum d(BlockSize, 0, dFile);
	BigNum n(BlockSize, BlockSize, dFile);
	dFile.close();
		
	std::ofstream fileOutputText("outputDecode.txt", std::ios::out);
	fileText.seekg(0, std::ios::end);
	int sizeFile = fileText.tellg();
	for (int i = 0; i < sizeFile; i += BlockSize)
	{
		BigNum tmp(BlockSize, i, fileText);
		BigNum res = BigNum::FastPow(tmp, d, n);
		res.PrintF(fileOutputText,1);
		if (i > sizeFile) break;
	}

	fileText.close();
	fileOutputText.close();
}