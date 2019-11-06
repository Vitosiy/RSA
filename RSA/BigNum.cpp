#include "BigNum.h"


BigNum::BigNum()
{
	this->LongNum.push_back(0);
};

BigNum::BigNum(unsigned int x)
{
	this->LongNum.push_back(x);
};


BigNum::BigNum(const std::string& Str) {
	std::string SubStr;

	for (unsigned int i = 0; i < Str.size(); i = i + 4) {
		if (i + 4 > Str.size()) {
			SubStr = std::string(Str.begin() + i, Str.end());
			if (Str.size() - (i + 4) == -1)
			{
				std::reverse(LongNum.begin(), LongNum.end());
				this->LongNum.push_back((int)((unsigned char)SubStr[0] << 24) | (int)((unsigned char)SubStr[1] << 16) | (int)((unsigned char)SubStr[2] << 8));
				std::reverse(LongNum.begin(), LongNum.end());
			}
			if (Str.size() - (i + 4) == -2)
			{
				std::reverse(LongNum.begin(), LongNum.end());
				this->LongNum.push_back((int)((unsigned char)SubStr[0] << 24) | (int)((unsigned char)SubStr[1] << 16));
				std::reverse(LongNum.begin(), LongNum.end());
			}
			if (Str.size() - (i + 4) == -3)
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

void BigNum::PrintF(ofstream& filename)
{
	for (auto it = LongNum.crbegin(); it != LongNum.crend(); ++it) {

		filename << std::hex << (char)(*it >> 24) << (char)(*it >> 16) << (char)(*it >> 8) << (char)(*it);
	}
	filename << std::endl;

}


unsigned int BigNum::Size()
{
	return this->LongNum.size();
};

BigNum BigNum::Add(const BigNum& A, const BigNum& B)
{
	BigNum _A = A, _B = B, One(1);
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
		unsigned long long int tmp = _B.LongNum[i];
		if (((tmp += _A.LongNum[i]) > UINT_MAX)) {
			if ((i + 1) > (length - 1));
			_B.LongNum.push_back(0);

			_B.LongNum[i + 1] += One.LongNum[0];  // если есть разряд для переноса, переносим его в следующий разряд
			_B.LongNum[i] = tmp; // если есть разряд для переноса он отсекается
		}
		else
		{
			_B.LongNum[i] += _A.LongNum[i]; // суммируем последние разряды чисел
		}

	}
	while ((_B.Size() > 1) and (_B.LongNum.back() == 0)) {
		_B.LongNum.pop_back();
	};

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
			for (int ix = length - 1; ix > 0; ix--) { // поразрядное сравнение весов чисел 
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

		if (_A.LongNum[ix] < _B.LongNum[ix])
		{
			_A.LongNum[ix + 1]--; // переносим единицу в старший разряд
			_A.LongNum[ix] -= _B.LongNum[ix];
		}
		else _A.LongNum[ix] -= _B.LongNum[ix];
	}

	while ((_A.Size() > 1) and (_A.LongNum.back() == 0)) {
		_A.LongNum.pop_back();
	};

	return _A;
}

void BigNum::Mul(const BigNum& A, const BigNum& B, BigNum& Res) {

	if (A == BigNum(0) || B == BigNum(0)) {
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
			res[i + j] = (int)cur;
			carryover = (int)(cur >> 32);
		}
	}

	while ((res.size() > 1) and (res.back() == 0)) {
		res.pop_back();
	}

	Res.LongNum = res;
}

void BigNum::Div(const BigNum& A, const BigNum& B, BigNum& IntegerResultOfDivision, BigNum& Reminder, bool& MistakeWasMade, bool& flag)
{
	BigNum _A = A, _B = B;
	BigNum One(1);
	BigNum Zero;
	unsigned int size_a = _A.Size();
	unsigned int size_b = _B.Size();


	if ((_A.Size() == 0) | (_B.Size() == 0)) {
		MistakeWasMade = true;
		return;
	}
	if (_A < _B) {
		IntegerResultOfDivision = Zero;
		Reminder = A;
		return;
	}
	if (_A == _B) {
		IntegerResultOfDivision = One;
		Reminder = Zero;
		return;
	}

	BigNum res;
	while (res.Size() < (size_a - 1))
		res.LongNum.push_back(0);
	BigNum tmp_A = _A.LongNum[size_a - 1];
	unsigned int size;
	if (size_a == 1) size = (size_a - 1);
	else size = (size_a - 2);
	for (int i = size; i >= 0; i--)
	{
		while (tmp_A <= _B) {
			reverse(tmp_A.LongNum.begin(), tmp_A.LongNum.end());
			tmp_A.LongNum.push_back(_A.LongNum[i]);
			reverse(tmp_A.LongNum.begin(), tmp_A.LongNum.end());
			if(i > 0) i--;
		};
		unsigned int size_tmp = tmp_A.Size();
		BigNum tmp;
		if (tmp_A.Size() > size_b) {
			unsigned long long int d_tmp;
			d_tmp = tmp_A.LongNum[size_tmp - 1];
			d_tmp <<= 32;
			d_tmp |= tmp_A.LongNum[size_tmp - 2];
			d_tmp = d_tmp / _B.LongNum[size_b - 1];
			while ((tmp = _B * d_tmp) > tmp_A) {
				d_tmp--;
			}
			tmp_A = tmp_A - tmp;
			res.LongNum[i] = d_tmp;
		}
		else {
			unsigned int res_t = tmp_A.LongNum[size_tmp - 1] / _B.LongNum[size_b - 1];
			while ((tmp = _B * res_t) > tmp_A) {
				res_t--;
			}
			tmp_A = tmp_A - tmp;
			res.LongNum[i] = res_t;
		};
	}
	BigNum curValue = _A - _B * res;

	if (flag == 1) {
		// избавляемся от лидирующих нулей
		if (res.Size() > 1) {
			reverse(res.LongNum.begin(), res.LongNum.end());
			while (res.LongNum.back() == 0) {
				res.LongNum.pop_back();
			};
			reverse(res.LongNum.begin(), res.LongNum.end());
		}
		IntegerResultOfDivision = res;
	}
	else
	{
		if (curValue.Size() > 1) {
			reverse(curValue.LongNum.begin(), curValue.LongNum.end());
			while (curValue.LongNum.back() == 0) {
				curValue.LongNum.pop_back();
			};
			reverse(curValue.LongNum.begin(), curValue.LongNum.end());
		}
		Reminder = curValue;
	}
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

BigNum BigNum::FastPow(BigNum & Num, BigNum & Deg, BigNum & Mod) {
	
	BigNum _Num = Num;
	//1:
	std::string deg = Deg.ToBin();

	//2:
	unsigned int n = deg.size();

	//3:
	vector<BigNum> Mass(n);
	BigNum cur;
	Mass[0] = Num % Mod;
	BigNum IntDivRes(0), Rem(0);
	bool Flag = false;
	for (unsigned int i = 1; i < n; i++) {
		cur = Mass[i - 1] * Mass[i - 1];
		Mass[i] = cur % Mod;
	}

	//4:
	BigNum ResRes(1);
	for (unsigned int i = 0; i < n; i++) {
		ResRes = ResRes * Mass[i].Pow(deg[n - i]);
	}
	BigNum result = ResRes % Mod;

	//5:
	return result;
}

BigNum BigNum::Evk(const BigNum& A, const BigNum& B)
{
	BigNum _A = A, _B = B;
	BigNum y1(1), x1, x2(1), y2;
	while (_B > 0)
	{
		BigNum q = _A / _B;
		BigNum r = _A % _B;
		BigNum T1 = x2 - q * x1;
		BigNum T2 = y2 - q * y1;
		_A = _B;
		_B = r;
		x2 = x1;
		x1 = T1;
		y2 = y1;
		y1 = T2;
	}
	return _A;
}

bool operator==(const BigNum & A, const BigNum & B) {
	if (A.LongNum.size() != B.LongNum.size()) {
		return false;
	}

	for (int i = A.LongNum.size(); i > 0; i--) {
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
		for (int i = A.LongNum.size(); i > 0; i--) {
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
		for (int i = A.LongNum.size(); i > 0; i--) {
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
		for (int i = A.LongNum.size(); i > 0; i--) {
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
		for (int i = A.LongNum.size(); i > 0; i--) {
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
	BigNum Res;
	BigNum::Mul(A, B, Res);
	return Res;
}

BigNum operator%(const BigNum & A, const BigNum & B) {
	BigNum IntDivRes(0), Rem(0);
	bool Mistake =false, flag = 0;
	BigNum::Div(A, B, IntDivRes, Rem, Mistake, flag);

	if (!Mistake) {
		return Rem;
	}
	else {
		return BigNum(0);
	}
	
}

BigNum operator/(const BigNum & A, const BigNum & B) {
	BigNum IntDivRes(0), Rem(0);
	bool Mistake =false, flag = 1;
	BigNum::Div(A, B, IntDivRes, Rem, Mistake, flag);

	if (!Mistake) {
		return IntDivRes;
	}
	else {
		return BigNum(0);
	}
}