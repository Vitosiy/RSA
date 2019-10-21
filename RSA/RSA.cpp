#include "RSA.h"

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
	e = BigNum(to_string(ferm[rand() % 6]));
}

BigNum RSA::calculateD(BigNum& e, BigNum& phi) {
	BigNum tmp;
	BigNum d;
	while (true) {
		tmp = tmp + phi;
		d = (tmp / e) + BigNum("1");
		if ((d * e) % phi == BigNum("1"))
			return d;
	}
}

void RSA::encode(const std::string& pathToInputText, const std::string& pathToPublicKey, const unsigned int mode) {
	if (mode == 0) {
		BigNum p, q;
		generatePQ(p, q, 512);
		n = p * q;
		BigNum phi = (p - BigNum("1")) * (q - BigNum("1"));
		calculateE();
		d = calculateD(e, phi);
		std::ofstream eFile("publicKey.txt"), dFile("privateKey.txt");
		eFile << e << std::endl;
		eFile << n << std::endl;
		dFile << d << std::endl;
		dFile << n << std::endl;
		eFile.close();
		dFile.close();
	}
	else {
		std::ifstream eFile(pathToPublicKey);
		std::string eStr, nStr;
		eFile >> eStr >> nStr;
		n = nStr;
		e = eStr;
		eFile.close();
	}
	std::ofstream fileOutputText("outputEncode.txt");
	char ch;
	std::ifstream fileText(pathToInputText);
	if (!fileText) {
		std::cout << "Wrong path to text" << std::endl;
		return;
	}
	while (fileText.get(ch)) {
		BigNum encodeCh = BigNum(to_string((int)ch));
		BigNum res = BigNum::FastPow(encodeCh,e,n);
		fileOutputText << encodeCh << std::endl;
	}
	fileText.close();
	fileOutputText.close();
}

void RSA::decode(const std::string& pathToText, const std::string& pathToPrivateKey) {
	std::string nStr, dStr;
	std::ifstream dFile(pathToPrivateKey), fileText(pathToText);
	dFile >> dStr >> nStr;
	dFile.close();
	n = nStr;
	d = dStr;
	std::ofstream fileOutputText("outputDecode.txt", std::ios::app);
	std::string str;
	while (fileText >> str) {
		BigNum decodeCh = (BigNum::FastPow(BigNum(str),d, n));
		char res = (char)stoi(decodeCh.NumAsString());
		fileOutputText << res;
	}
	fileText.close();
	fileOutputText.close();
}