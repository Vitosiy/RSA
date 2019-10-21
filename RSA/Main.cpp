#include "RSA.h"

BigNum RSA::e = 0;
BigNum RSA::d = 0;
BigNum RSA::n = 0;

int main() {
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	std::locale::global(std::locale("rus"));



	int answer = 0;
	std::string pathToText, pathToPrivateKey, pathToPublicKey;
	while (true) {
		std::cout << "1) Encode\n2) Decode\n";
		std::cin >> answer;
		switch (answer) {
		case 1:
			std::cout << "1) Generate new keys\n2) Enter path to public keys\n";
			std::cin >> answer;
			switch (answer) {
			case 1:
				std::cout << "Enter path to text: ";
				std::cin >> pathToText;
				RSA::encode(pathToText, "", 0);
				exit(0);
			case 2:
				std::cout << "Enter path to text: ";
				std::cin >> pathToText;
				std::cout << "Enter path to pub key: ";
				std::cin >> pathToPublicKey;
				RSA::encode(pathToText, pathToPublicKey, 1);
				exit(0);
			}
		case 2:
			std::cout << "Enter path to text: ";
			std::cin >> pathToText;
			std::cout << "Enter path to priv key: ";
			std::cin >> pathToPrivateKey;
			RSA::decode(pathToText, pathToPrivateKey);
			exit(0);
		}
	}
	return 0;
}