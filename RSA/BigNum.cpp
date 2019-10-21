#include "BigNum.h"

/*BigNum::BigNum() {
	this->LongNum.push_back(0);
}

BigNum::BigNum(const string & Num) {
	string LocalNum = Num;
	bool DelLastZero = false;
	while (LocalNum.size() > 1 && LocalNum.back() == 0) {
		DelLastZero = true;
		reverse(LocalNum.begin(), LocalNum.end());
		LocalNum.pop_back();
		reverse(LocalNum.begin(), LocalNum.end());
	}

	for (int i = (int)LocalNum.length(); i > 0; i -= 9) {
		if (i < 9) {
			this->LongNum.push_back(atoi(LocalNum.substr(0, i).c_str()));
		}
		else {
			this->LongNum.push_back(atoi(LocalNum.substr(i - 9, 9).c_str()));
		}
	}
	
	if (DelLastZero) {
		this->LongNum.pop_back();
	}
}

void BigNum::Print() {
	printf("%d", this->LongNum.empty() ? 0 : this->LongNum.back());
	for (int i = (int)this->LongNum.size() - 2; i >= 0; --i) {
		printf("%09d", this->LongNum[i]);
	}
	cout << endl;
}*/

BigNum::BigNum()
{
	this->LongNum.push_back(0);
};

BigNum::BigNum(unsigned int x)
{
	this->LongNum.push_back(x);
};

BigNum::BigNum(const unsigned int BlockSize, const unsigned int Offset, const std::string& Filename) {
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
			SubStr = std::string(Str.begin() + i, Str.end() - 1);

		}
		else {
			SubStr = std::string(Str.begin() + i, Str.begin() + i + 4);
			this->LongNum.push_back((int)((unsigned char)SubStr[0] << 24) | (int)((unsigned char)SubStr[1] << 16) | (int)((unsigned char)SubStr[2] << 8) | (int)((unsigned char)SubStr[3]));
		}
	}

	this->Sing = 0;
}

void BigNum::Print() {
	for (auto it = LongNum.crbegin(); it != LongNum.crend(); ++it) {

		std::cout << " " << std::hex << (char)(*it >> 24) << (char)(*it >> 16) << (char)(*it >> 8) << (char)(*it);
		//std::cout << " " << *it;
	}
	std::cout << std::endl;
}


/*void BigNum::Print() {
	for (unsigned int i = 0; i < LongNum.size(); i++) {
		std::cout << " " << std::hex <<this->LongNum[i];
	}
	std::cout << std::endl;
}*/

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

/*void BigNum::Div(BigNum& A, unsigned int B, BigNum& Reminder) {
	if (B == 0) {
		return;
	}

	unsigned int carry = 0;
	for (int i = (int)A.LongNum.size() - 1; i >= 0; --i) {
		long long cur = A.LongNum[i] + carry * 1ll * Base;
		A.LongNum[i] = int(cur / B);
		carry = int(cur % B);
	}
	BigNum Rem(to_string(carry));
	Reminder.LongNum = Rem.LongNum;

	while (A.LongNum.size() > 1 && A.LongNum.back() == 0) {
		A.LongNum.pop_back();
	}
}*/

/*void BigNum::Div(BigNum & A, unsigned int B) {
	if (B == 0) {
		return;
	}

	unsigned int carry = 0;
	for (int i = (int)A.LongNum.size() - 1; i >= 0; --i) {
		long long cur = A.LongNum[i] + carry * 1ll * Base;
		A.LongNum[i] = int(cur / B);
		carry = int(cur % B);
	}

	while (A.LongNum.size() > 1 && A.LongNum.back() == 0) {
		A.LongNum.pop_back();
	}
}*/

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
		if (_A.LongNum[i] >= _B.LongNum[i])
		{
			ResDiv.LongNum[0]++;
		}
	}

	ResDiv.LongNum[0]--;

	IntegerResultOfDivision = ResDiv;
	Reminder = (_A - (_B * ResDiv));
	
}

BigNum & BigNum::operator=(const BigNum & A) {
	this->LongNum = A.LongNum;
	return *this;
}



/*void BigNum::ToBin(const BigNum & Num, BigNum & Res) {
	BigNum Tmp("0");
	BigNum Zero("0");
	BigNum RemTmp("0");
	string ResStr;
	Tmp.LongNum = Num.LongNum;
	while (Tmp > Zero) {
		Div(Tmp, 2, RemTmp);
		ResStr.push_back(RemTmp.LongNum[0] + 48);
	}

	reverse(ResStr.begin(), ResStr.end());
	BigNum A(ResStr);
	Res.LongNum = A.LongNum;
}

BigNum & BigNum::Pow(const unsigned int Times) {
	for (unsigned int i = 0; i < Times - 1; i++) {
		BigNum::Mul(*this, *this, *this);
	}

	return *this;
}

BigNum & BigNum::FastPow(BigNum & Num, BigNum & Deg, BigNum & Mod) {
	//1:
	BigNum Res("0");
	BigNum BinNum(Res.NumAsString());

	//2:
	BigNum Len(to_string(Res.NumAsString().length()));

	//3:
	vector<BigNum> Mass(Res.NumAsString().length());
	Mass[0] = Num % Mod;
	BigNum IntDivRes("0"), Rem("0");
	bool Flag = false;
	for (unsigned int i = 1; i < Mass.size(); i++) {
		Mass[i] = (Mass[i - 1] * Mass[i - 1]) % Mod;
	}

	//4:
	BigNum ResRes("1");
	for (unsigned int i = 0; i < Res.NumAsString().length(); i++) {
		ResRes = ResRes * Mass[i].Pow(BinNum.NumAsString()[Res.NumAsString().length() - i]);
	}

	//5:
	return ResRes % Mod;
}

string BigNum::NumAsString() {
	string Res, Tmp;
	for (unsigned int i = this->LongNum.size(); i > 0; i--) {
		Tmp = to_string(this->LongNum[i - 1]);
		for (unsigned int j = 0; j < Tmp.size(); j++) {
			Res.push_back(Tmp[j]);
		}
	}

	return Res;
}*/



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

/*BigNum operator/(BigNum& A, unsigned int B) {
	BigNum::Div(A, B);
	return A;
}*/
