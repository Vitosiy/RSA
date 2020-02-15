#include "RSA.h"

#define BlockSize 76
#define p_q_size 128

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
	_p = std::string(mpz_get_str(NULL, 10, p));
	_q = std::string(mpz_get_str(NULL, 10, q));
}

void RSA::calculateE() {
	long long ferm[] = { 3, 5, 17, 257, 65537, 4294967297 };
	srand(time(NULL));
	e = BigNum(ferm[rand() % 5]);
}

BigNum RSA::calculateD(BigNum& e, BigNum& phi, BigNum& p, BigNum& q) {
	/*BigNum tmp = phi;
	BigNum d, One(1);
	d = BigNum::Evk(e,phi);
	if (((d * e) % phi) == 1) return d;
	else return 0;*/
	BigNum d, y;
	BigNum g = BigNum::Evk(e, phi, d, y);
	while (!(g == 1)) {
		generatePQ(p, q, p_q_size);
		n = p * q;
		BigNum phi = (p - BigNum(1)) * (q - BigNum(1));
		calculateE();
		BigNum g = BigNum::Evk(e, phi, d, y);
	};

	d = (d % phi + phi) % phi;
	//BigNum tmp = (d * e)%phi;
	//if(tmp == 1)
	return d;
}

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
		std::ofstream eFile("publicKey.txt", std::ios::app), dFile("privateKey.txt", std::ios::app);
		e.PrintF(eFile);
		n.PrintF(eFile);
		d.PrintF(dFile);
		n.PrintF(dFile);
		eFile.close();
		dFile.close();
	}
	else {
		std::ifstream eFile(pathToPublicKey, std::ios::in);
		std::string eStr, nStr;
		eFile >> eStr >> nStr;
		n = nStr;
		e = eStr;
		eFile.close();
	}
	std::ofstream fileOutputText("outputEncode.txt", std::ios::app);
	char* tmp = new char[BlockSize];
	while (!fileText.eof()) {
		fileText.read(tmp, BlockSize);
		BigNum Block(tmp);
		BigNum res = BigNum::FastPow(Block,e,n);
		res.PrintF(fileOutputText);
	}
	fileText.close();
	fileOutputText.close();
	delete[] tmp;
}

void RSA::decode(const std::string& pathToText, const std::string& pathToPrivateKey) {
	std::string nStr, dStr;
	std::ifstream dFile(pathToPrivateKey, std::ios::in), fileText(pathToText, std::ios::binary | std::ios::in);
	char* tmp = new char[BlockSize];
	dFile >> dStr >> nStr;
	dFile.close();
	n = nStr;
	d = dStr;
	std::ofstream fileOutputText("outputDecode.txt", std::ios::app);
	while (!fileText.eof()) {
		fileText.read(tmp, BlockSize);
		BigNum Block(tmp);
		BigNum res = (BigNum::FastPow(Block, d, n));
		res.PrintF(fileOutputText);
	}
	fileText.close();
	fileOutputText.close();
	delete[] tmp;
}