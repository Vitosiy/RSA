#include "BigNum.h"


BigNum::BigNum()
{
	this->LongNum.push_back(0);
};

BigNum::BigNum(unsigned int x)
{
	this->LongNum.push_back(x);
};


BigNum::BigNum(unsigned int BlockSize, const unsigned int Offset, const std::string& Filename) {
	this->Sing = 0;
	std::fstream fstr(Filename, std::ios::binary | std::ios::in);
	if (!fstr.is_open()) {
		this->NumCreated = 0;
		return;
	}
	fstr.seekg(Offset);
	char Tmp[4];
	for (int i = BlockSize; i >= 0; i = i - 4) {
		fstr.read(Tmp, 4);
		std::reverse(LongNum.begin(), LongNum.end());
		this->LongNum.push_back((int)((unsigned char)Tmp[0] << 24) | (int)((unsigned char)Tmp[1] << 16) | (int)((unsigned char)Tmp[2] << 8) | (int)((unsigned char)Tmp[3]));
		std::reverse(LongNum.begin(), LongNum.end());
		std::fill(std::begin(Tmp), std::begin(Tmp) + 4, NULL);
	}
	this->NumCreated = 1;
}

BigNum::BigNum(const std::string& Str) {
	std::string SubStr;

	for (unsigned int i = 0; i < Str.size(); i = i + 4) {
		if (i + 4 > Str.size()) {
			SubStr = std::string(Str.begin() + i, Str.end());
			if (Str.size() - (i + 4) == -3)
			{
				std::reverse(LongNum.begin(), LongNum.end());
				this->LongNum.push_back((int)((unsigned char)SubStr[0] << 24) | (int)((unsigned char)SubStr[1] << 16) | (int)((unsigned char)SubStr[2] << 8));
			}
			if (Str.size() - (i + 4) == -2)
			{
				std::reverse(LongNum.begin(), LongNum.end());
				this->LongNum.push_back((int)((unsigned char)SubStr[0] << 24) | (int)((unsigned char)SubStr[1] << 16));
				std::reverse(LongNum.begin(), LongNum.end());
			}
			if (Str.size() - (i + 4) == -1)
			{
				std::reverse(LongNum.begin(), LongNum.end());
				this->LongNum.push_back((int)((unsigned char)SubStr[0] << 24));
				std::reverse(LongNum.begin(), LongNum.end());
			}
		}
		else {
			SubStr = std::string(Str.begin() + i, Str.begin() + i + 4);
			std::reverse(LongNum.begin(), LongNum.end());
			this->LongNum.push_back((int)((unsigned char)SubStr[0] << 24) | (int)((unsigned char)SubStr[1] << 16) | (int)((unsigned char)SubStr[2] << 8) | (int)((unsigned char)SubStr[3]));
			std::reverse(LongNum.begin(), LongNum.end());
			std::fill(std::begin(SubStr), std::begin(SubStr) + 4, NULL);
		}
	}

	this->Sing = 0;
}


void BigNum::Print(bool flag) {

	this->PrintP(flag);
}

void BigNum::PrintP(bool flag) {
	if (flag == 1) {
		for (auto it = LongNum.crbegin(); it != LongNum.crend(); ++it) {

			std::cout << " " << std::hex << (char)(*it >> 24) << (char)(*it >> 16) << (char)(*it >> 8) << (char)(*it);
		}
		std::cout << std::endl;
	}
	if (flag == 0) {
		for (auto it = LongNum.crbegin(); it != LongNum.crend(); ++it) {

			std::cout << " " << *it;
		}
		std::cout << std::endl;
	}
}

void BigNum::PrintF(const std::string& Filename)
{
	std::fstream filename(Filename, std::ios::binary | std::ios::out);
	if (!filename.is_open()) {
		this->NumCreated = 0;
		return;
	}
	for (auto it = LongNum.crbegin(); it != LongNum.crend(); ++it) {

		filename << std::hex << (char)(*it >> 24) << (char)(*it >> 16) << (char)(*it >> 8) << (char)(*it);
	}
	std::cout << std::endl;

}


unsigned int BigNum::Size()
{
	return this->LongNum.size();
};

BigNum BigNum::Add(const BigNum& A, const BigNum& B) 
{
	BigNum _A = A, _B = B;
	unsigned int size_a = _A.Size(), size_b = _B.Size(), length;
	if (size_a > size_b) {
		length = size_a;
		size_a = size_b;
		size_b = length;
		_B = A;
		_A = B;
	}
	else
		length = size_b;
	size_a = _A.Size();
	for (int i = 0; i < length; i++)
	{
		if (i >= size_a)
			break;
		_B.LongNum[i] += _A.LongNum[i]; // суммируем последние разряды чисел
		if(((_A.LongNum[i] / Base) == true) & ((i+1) > (length-1)) )
			_B.LongNum.push_back(0);
		_B.LongNum[i] += (_A.LongNum[i] / Base);  // если есть разряд для переноса, переносим его в следующий разряд
		_B.LongNum[i] %= Base; // если есть разряд для переноса он отсекается
	}

	if (_B.LongNum[length - 2] == 0)
		length--;
	return _B;
}

BigNum BigNum::Sub(const BigNum& A, const BigNum& B)
{
	BigNum _A = A, _B = B, Res;
	unsigned int size_a = _A.Size(), size_b = _B.Size(), length = size_a;
	if (size_a < size_b) {
		return Res; //TODO: make try {} catch block there
	}

	else if (size_a == size_b) {
		if (_A.LongNum[length - 1] < _B.LongNum[length - 1]) {
			return Res;
		}
		else if (_A.LongNum[length - 1] == _B.LongNum[length - 1]) {
			for (int ix = length-1; ix > 0; ix--) { // поразрядное сравнение весов чисел 
				if (_A.LongNum[ix] > _B.LongNum[ix]) {
					break;
				}
				if (_A.LongNum[ix] < _B.LongNum[ix]) {// если разряд первого числа больше
					return Res; // выход 
				}
			}
		}
	}

	for (int ix = 0; ix < length; ix++) {// проход по всем разрядам числа, начиная с последнего, не доходя до первого
		if (ix >= size_b) {
			break;
		}

		_A.LongNum[ix] -= _B.LongNum[ix]; // вычитаем значение текущего разряда меньшего числа

		if (_A.LongNum[ix] / Base > 0) {// если значение в текущем разряде двухразрядное
			_A.LongNum[ix + 1]++; // переносим единицу в старший разряд
			_A.LongNum[ix] %= Base; // в текущем разряде отсекаем ее
		}
	}

	if (_A.LongNum[length-1] == 0) {

		length--;
	}

	return _A;
}

void BigNum::Mul(const BigNum & A, const BigNum & B, BigNum & Res) {

	if (A == BigNum("0") || B == BigNum("0")) {
		Res.LongNum = vector<unsigned int>(0);
		return;
	}

	BigNum _A = A, _B = B;
	vector<unsigned int> res(A.LongNum.size() + B.LongNum.size());
	unsigned long long cur;
	for (unsigned int i = 0; i < _A.LongNum.size(); i++) {
		for (unsigned int j = 0, carryover = 0; j < _B.LongNum.size() || carryover; j++) {
			if (j < _B.LongNum.size()) {
				cur = res[i + j] + _A.LongNum[i] * 1ull * _B.LongNum[j] + carryover;
			}
			else {
				cur = static_cast <unsigned long long>(res[i + j]) + static_cast <unsigned long long> (carryover);
			}
			res[i + j] = (int)(cur % Base);
			carryover = (int)(cur / Base);
		}
	}

	while (res.back() == 0) {
		res.pop_back();
	}

	Res.LongNum = res;
}

void BigNum::Div(const BigNum& A, const BigNum& B, BigNum& IntegerResultOfDivision, BigNum& Reminder, bool& MistakeWasMade) {
	
	BigNum _A = A, _B = B;
	unsigned int length = _A.Size();
	BigNum One(1);
	BigNum ResDiv(1);
	BigNum Tmp(0);

	if ((_A.Size() == 0) | (_B.Size() == 0)) {
		MistakeWasMade = true;
		return;
	}
	if (_A < _B) {
		IntegerResultOfDivision = Tmp;
		Reminder = A;
		return;
	}
	if (_A == _B) {
		IntegerResultOfDivision = One;
		Reminder = Tmp;
		return;
	}

	for (unsigned int i = 0; i < (length-1); i++)
	{
		if (i >= _B.Size())
			break;
		if (_A.LongNum[i] >= _B.LongNum[i])
		{
			ResDiv.LongNum[0]++;
		}
	}

	if (ResDiv.LongNum[0] > 1) {
		ResDiv.LongNum[0]--;
	}


	IntegerResultOfDivision = ResDiv;
	Reminder = (_A - (_B * ResDiv));
	
}

BigNum & BigNum::operator=(const BigNum & A) {
	this->LongNum = A.LongNum;
	return *this;
}

std::string BigNum::ToBin() {
	std::string res;
	for (unsigned int i = 0; i < this->LongNum.size(); i++) {
		res = res + (std::bitset<32>(this->LongNum[i])).to_string();
	}

	return res;

};

BigNum & BigNum::Pow(const unsigned int Times) {
	if(Times == 1)
		return *this;

	if (Times == 0)
		return *this=1;


	for (unsigned int i = 0; i < Times - 1; i++) {
		BigNum::Mul(*this, *this, *this);
	}

	return *this;
}

BigNum & BigNum::FastPow(BigNum & Num, BigNum & Deg, BigNum & Mod) {
	
	BigNum _Num = Num;
	//1:
	std::string deg = Deg.ToBin();

	//2:
	unsigned int n = deg.size();

	//3:
	vector<BigNum> Mass(n);
	BigNum cur;
	Mass[0] = Num % Mod;
	BigNum IntDivRes("0"), Rem("0");
	bool Flag = false;
	for (unsigned int i = 1; i < n; i++) {
		cur = Mass[i - 1] * Mass[i - 1];
		Mass[i] = cur % Mod;
	}

	//4:
	BigNum ResRes("1");
	for (unsigned int i = 0; i < n; i++) {
		ResRes = ResRes * Mass[i].Pow(deg[n - i]);
	}
	BigNum result = ResRes % Mod;

	//5:
	return result;
}

bool operator==(const BigNum & A, const BigNum & B) {
	if (A.LongNum.size() != B.LongNum.size()) {
		return false;
	}

	for (unsigned int i = A.LongNum.size(); i > 0; i--) {
		if (A.LongNum[i - 1] != B.LongNum[i - 1]) {
			return false;
		}
	}

	return true;
}

bool operator <(const BigNum & A, const BigNum& B) {
	if (A.LongNum.size() < B.LongNum.size()) {
		return true;
	}
	else if (A.LongNum.size() > B.LongNum.size()) {
		return false;
	}
	else {
		for (unsigned int i = A.LongNum.size(); i > 0; i--) {
			if (A.LongNum[i - 1] < B.LongNum[i - 1]) {
				return true;
			}
			else {
				return false;
			}
		}
	}

	return false;
}

bool operator>(const BigNum & A, const BigNum & B) {
	if (A.LongNum.size() > B.LongNum.size()) {
		return true;
	}
	else if (A.LongNum.size() < B.LongNum.size()) {
		return false;
	}
	else {
		for (unsigned int i = A.LongNum.size(); i > 0; i--) {
			if (A.LongNum[i - 1] > B.LongNum[i - 1]) {
				return true;
			}
			else {
				return false;
			}
		}
	}

	return false;
}

bool operator<=(const BigNum & A, const BigNum & B) {
	if (A.LongNum.size() < B.LongNum.size()) {
		return true;
	}
	else if (A.LongNum.size() > B.LongNum.size()) {
		return false;
	}
	else {
		for (unsigned int i = A.LongNum.size(); i > 0; i--) {
			if (A.LongNum[i - 1] < B.LongNum[i - 1]) {
				return true;
			}
			else if (A.LongNum[i - 1] == B.LongNum[i - 1]){
				continue;
			}
			else {
				return false;
			}
		}
	}

	return false;
}

bool operator>=(const BigNum & A, const BigNum & B) {
	if (A.LongNum.size() < B.LongNum.size()) {
		return false;
	}
	else if (A.LongNum.size() > B.LongNum.size()) {
		return true;
	}
	else {
		for (unsigned int i = A.LongNum.size(); i > 0; i--) {
			if (A.LongNum[i - 1] > B.LongNum[i - 1]) {
				return true;
			}
			else if (A.LongNum[i - 1] == B.LongNum[i - 1]) {
				continue;
			}
			else {
				return false;
			}
		}
	}

	return false;
}

BigNum operator+(const BigNum & A, const BigNum & B) {
	return BigNum::Add(A, B);
}

BigNum operator+=(BigNum& A, const BigNum& B)
{
	return A = BigNum::Add(A, B);
}

BigNum operator-(const BigNum & A, const BigNum & B) {
	return 	BigNum::Sub(A, B);
}

BigNum operator-=(BigNum& A, const BigNum& B)
{
	return A = BigNum::Sub(A, B);
}

BigNum operator*(const BigNum & A, const BigNum & B) {
	BigNum Res("0");
	BigNum::Mul(A, B, Res);
	return Res;
}

BigNum operator%(const BigNum & A, const BigNum & B) {
	BigNum IntDivRes("0"), Rem("0");
	bool Flag=false;
	BigNum::Div(A, B, IntDivRes, Rem, Flag);

	if (!Flag) {
		return Rem;
	}
	else {
		return BigNum("0");
	}
	
}

BigNum operator/(const BigNum & A, const BigNum & B) {
	BigNum IntDivRes("0"), Rem("0");
	bool Flag=false;
	BigNum::Div(A, B, IntDivRes, Rem, Flag);

	if (!Flag) {
		return IntDivRes;
	}
	else {
		return BigNum("0");
	}
}

