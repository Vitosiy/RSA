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
	

	for (unsigned int i = 0; i < Str.size(); i = i + 8) {
		if (i + 8 > Str.size()) {
			SubStr = std::string(Str.begin() + i, Str.end());
			vector<unsigned int> int_str = BigNum::convert(SubStr);
			if (Str.size() - (i + 8) == -1)
			{
				std::reverse(LongNum.begin(), LongNum.end());
				this->LongNum.push_back((int_str[0] << 28) | (int_str[1] << 24) | (int_str[2] << 20) | (int_str[3] << 16) | (int_str[4] << 12) | (int_str[5] << 8) | (int_str[6] << 4));
				std::reverse(LongNum.begin(), LongNum.end());
			}
			if (Str.size() - (i + 8) == -2)
			{
				std::reverse(LongNum.begin(), LongNum.end());
				this->LongNum.push_back((int_str[0] << 28) | (int_str[1] << 24) | (int_str[2] << 20) | (int_str[3] << 16) | (int_str[4] << 12) | (int_str[5] << 8));
				std::reverse(LongNum.begin(), LongNum.end());
			}
			if (Str.size() - (i + 8) == -3)
			{
				std::reverse(LongNum.begin(), LongNum.end());
				this->LongNum.push_back((int_str[0] << 28) | (int_str[1] << 24) | (int_str[2] << 20) | (int_str[3] << 16) | (int_str[4] << 12));
				std::reverse(LongNum.begin(), LongNum.end());
			}
			if (Str.size() - (i + 8) == -4)
			{
				std::reverse(LongNum.begin(), LongNum.end());
				this->LongNum.push_back((int_str[0] << 28) | (int_str[1] << 24) | (int_str[2] << 20) | (int_str[3] << 16));
				std::reverse(LongNum.begin(), LongNum.end());
			}
			if (Str.size() - (i + 8) == -5)
			{
				std::reverse(LongNum.begin(), LongNum.end());
				this->LongNum.push_back((int_str[0] << 28) | (int_str[1] << 24) | (int_str[2] << 20));
				std::reverse(LongNum.begin(), LongNum.end());
			}
			if (Str.size() - (i + 8) == -6)
			{
				std::reverse(LongNum.begin(), LongNum.end());
				this->LongNum.push_back((int_str[0] << 28) | (int_str[1] << 24));
				std::reverse(LongNum.begin(), LongNum.end());
			}
			if (Str.size() - (i + 8) == -7)
			{
				std::reverse(LongNum.begin(), LongNum.end());
				this->LongNum.push_back(int_str[0] << 28);
				std::reverse(LongNum.begin(), LongNum.end());
			}
		}
		else {
			SubStr = std::string(Str.begin() + i, Str.begin() + i + 8);
			vector<unsigned int> int_str = BigNum::convert(SubStr);
			std::reverse(LongNum.begin(), LongNum.end());
			this->LongNum.push_back((int_str[0] << 28) | (int_str[1] << 24) | (int_str[2] << 20) | (int_str[3] << 16) | (int_str[4] << 12) | (int_str[5] << 8) | (int_str[6] << 4) | (int_str[7]));
			std::reverse(LongNum.begin(), LongNum.end());
			std::fill(std::begin(SubStr), std::begin(SubStr) + 8, NULL);			
		}
	}
}


BigNum::BigNum(const unsigned int BlockSize, const unsigned int Offset, ifstream& Filename) {
	Filename.seekg(Offset);
	char Tmp[4];
	for (int i = BlockSize; i >= 0; i = i - 4) {
		Filename.read(Tmp, 4);
		std::reverse(LongNum.begin(), LongNum.end());
		this->LongNum.push_back((int)((unsigned char)Tmp[0] << 24) | (int)((unsigned char)Tmp[1] << 16) | (int)((unsigned char)Tmp[2] << 8) | (int)((unsigned char)Tmp[3]));
		std::reverse(LongNum.begin(), LongNum.end());
		std::fill(std::begin(Tmp), std::begin(Tmp) + 4, NULL);
	}
	
	std::reverse(LongNum.begin(), LongNum.end());
	while ((this->LongNum.size() > 1) and (this->LongNum.back() == 0)) {
		this->LongNum.pop_back();
	}
	std::reverse(LongNum.begin(), LongNum.end());
}


vector<unsigned int> BigNum::convert(std::string& Str) {
	vector<unsigned int> res;
	for (unsigned int i = 0; i < Str.size(); i++) {
		if (Str[i] >= '0' && Str[i] <= '9') res.push_back( Str[i] - '0');
		else if (Str[i] >= 'a' && Str[i] <= 'f') res.push_back(Str[i] - 'a' + 10);
		else break;
	}
	return res;
}

//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//

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

			std::cout << std::hex << *it;
		}
		std::cout << std::endl;
	}
}

void BigNum::PrintF(ofstream& filename, bool flag)
{
	if (flag == 1) {
		for (auto it = LongNum.crbegin(); it != LongNum.crend(); ++it) {

			filename << (char)(*it >> 24) << (char)(*it >> 16) << (char)(*it >> 8) << (char)(*it);
		}
		std::cout << std::endl;
	}
	if (flag == 0) {
		for (auto it = LongNum.crbegin(); it != LongNum.crend(); ++it) {

			filename << std::hex << *it;
		}
		filename << std::endl;
	}

}

unsigned int BigNum::Size()
{
	return this->LongNum.size();
};

//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//

BigNum BigNum::Add(const BigNum& A, const BigNum& B)
{
	BigNum _A = A, _B = B, One(1), result;
	bool flag = false;
	unsigned int size_a = _A.Size(), size_b = _B.Size(), length;
	if ((A.sign == false) & (B.sign == true)) { _B.sign == false; result = _A - _B; }
	else if ((A.sign == true) & (B.sign == false)) { _A.sign = false; result = _B - _A; }
	else {
		if ((A.sign == true) & (B.sign == true)) { _A.sign = false; _B.sign = false; flag = true; };
		if (size_b > size_a) {

			length = size_b;
			size_b = size_a;
			size_a = length;
			_A = B;
			_B = A;
		}
		else
			length = size_a;
		size_b = _B.Size();
		for (int i = 0; i < length; i++)
		{
			if (i >= size_b)
				break;
			unsigned long long int tmp = _A.LongNum[i];
			if (((tmp += _B.LongNum[i]) > UINT_MAX)) {
				if ((i + 1) > (length - 1));
				_A.LongNum.push_back(0);

				_A.LongNum[i + 1] += One.LongNum[0];  // ���� ���� ������ ��� ��������, ��������� ��� � ��������� ������
				_A.LongNum[i] = tmp; // ���� ���� ������ ��� �������� �� ����������
			}
			else
			{
				_A.LongNum[i] += _B.LongNum[i]; // ��������� ��������� ������� �����
			}

		}
		while ((_A.Size() > 1) and (_A.LongNum.back() == 0)) {
			_A.LongNum.pop_back();
		};
		result = _A;
	}
	if (flag == true) result.sign = true;
	return result;
}

BigNum BigNum::Sub(const BigNum& A, const BigNum& B)
{
	BigNum _A = A, _B = B, result;
	bool flag = false;
	unsigned int size_a = _A.Size(), size_b = _B.Size(), length = size_a;
	if ((A.sign == false) & (B.sign == true)) { _B.sign = false; result = _A + _B; }
	else if ((A.sign == true) & (B.sign == false)) { _B.sign = true; result = _A + _B; result.sign = true; }
	else {
		if (_A == _B) return _A = 0;
		if ((A.sign == true) & (B.sign == true)) {
			_A = B;
			_B = A;
			size_a = _A.Size();
			size_b = _B.Size();
			length = size_a;
			_A.sign = false;
			_B.sign = false;
		}
		else if (_A < _B) {
			_A = B;
			_B = A;
			size_a = _A.Size();
			size_b = _B.Size();
			length = size_a;
			flag = true;
		}

		for (int ix = 0; ix < length; ix++) {// ������ �� ���� �������� �����, ������� � ����������, �� ������ �� �������
			if (ix >= size_b) {
				break;
			}

			if (_A.LongNum[ix] < _B.LongNum[ix])
			{
				_A.LongNum[ix + 1]--; // ��������� ������� � ������� ������
				_A.LongNum[ix] -= _B.LongNum[ix];
			}
			else _A.LongNum[ix] -= _B.LongNum[ix];
		}
		while ((_A.Size() > 1) and (_A.LongNum.back() == 0)) {
			_A.LongNum.pop_back();
		};
		result = _A;
	}
	if (flag == true) result.sign = true;
	return result;
}

void BigNum::Mul(const BigNum& A, const BigNum& B, BigNum& Res) {

	if (A == BigNum(0) || B == BigNum(0)) {
		Res.LongNum = vector<unsigned int>(0);
		return;
	}

	BigNum _A = A, _B = B, result;
	vector<unsigned int> res(A.LongNum.size() + B.LongNum.size());
	unsigned long long cur;
	if ((A.sign == true) & (B.sign == true)) { _A.sign = false; _B.sign = false; }
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
	if (((A.sign == false) & (B.sign == true)) | ((A.sign == true) & (B.sign == false))) { Res.sign = true; }

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
	if ((A.sign == true) & (B.sign == true)) { _A.sign = false; _B.sign = false; }
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
	while (res.Size() < size_a)
		res.LongNum.push_back(0);
	BigNum tmp_A = _A.LongNum[size_a - 1];
	unsigned int size = size_a - 1;
	int count = 1;
	for (int i = size; i >= 0; )
	{
		int ix = i;
		while ((tmp_A <= _B) and (i > 0)) {
			reverse(tmp_A.LongNum.begin(), tmp_A.LongNum.end());
			//if (i == 0) tmp_A.LongNum.push_back(_A.LongNum[i]);
			tmp_A.LongNum.push_back(_A.LongNum[i - count]);
			reverse(tmp_A.LongNum.begin(), tmp_A.LongNum.end());
			while ((tmp_A.Size() > 1) and (tmp_A.LongNum.back() == 0)) {
				tmp_A.LongNum.pop_back();
			};
			count++;
			if (ix > 0) ix--;
			else break;
		};
		count = 1;
		BigNum tmp;
		unsigned int size_tmp = tmp_A.Size();
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
			if (ix == 0) i = -1;
			else if ((i == ix) and (i > 0)) {
				reverse(tmp_A.LongNum.begin(), tmp_A.LongNum.end());
				tmp_A.LongNum.push_back(_A.LongNum[i - 1]);
				reverse(tmp_A.LongNum.begin(), tmp_A.LongNum.end());
				i--;
			}
			else i = ix;
		}
		else {
			unsigned int res_t = tmp_A.LongNum[size_tmp - 1] / _B.LongNum[size_b - 1];
			while ((tmp = _B * res_t) > tmp_A) {
				res_t--;
			}
			tmp_A = tmp_A - tmp;
			res.LongNum[i] = res_t;
			if (ix == 0) i = -1;
			else if ((i == ix) and (i > 0)) {
				reverse(tmp_A.LongNum.begin(), tmp_A.LongNum.end());
				tmp_A.LongNum.push_back(_A.LongNum[i - 1]);
				reverse(tmp_A.LongNum.begin(), tmp_A.LongNum.end());
				i--;
			}
			else i = ix;
		};
	}
	// ����������� �� ���������� �����
	if (res.Size() > 1) {
		//reverse(res.LongNum.begin(), res.LongNum.end());
		while (res.LongNum.back() == 0) {
			res.LongNum.pop_back();
		};
		//reverse(res.LongNum.begin(), res.LongNum.end());
	}
	BigNum curValue = tmp_A;

	if (flag == 1) {

		IntegerResultOfDivision = res;
		if (((A.sign == false) & (B.sign == true)) | ((A.sign == true) & (B.sign == false))) { IntegerResultOfDivision.sign = true; }

	}
	else
	{
		if (curValue.Size() > 1) {
			while (curValue.LongNum.back() == 0) {
				curValue.LongNum.pop_back();
			};
		}
		Reminder = curValue;
		if (((A.sign == false) & (B.sign == true)) | ((A.sign == true) & (B.sign == false))) { Reminder.sign = true; }
	}
}

//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//

std::string BigNum::ToBin() {
	std::string res;
	for (int i = this->LongNum.size()-1; i >= 0 ; i--) {
		res = res + (std::bitset<32>(this->LongNum[i])).to_string();
	}

	reverse(res.begin(), res.end());
	while (res.back() == '0') {
		res.pop_back();
	};
	reverse(res.begin(), res.end());

	return res;

};

BigNum& BigNum::Pow(const unsigned int Times) {
	if (Times == 1)
		return *this;

	if (Times == 0)
		return *this = 1;


	for (unsigned int i = 0; i < Times - 1; i++) {
		BigNum::Mul(*this, *this, *this);
	}

	return *this;
}

BigNum BigNum::FastPow(BigNum& Num, BigNum& Deg, BigNum& Mod) {

	BigNum _Num = Num, _Mod = Mod;
	//1:
	std::string deg = Deg.ToBin();

	//2:
	unsigned int n = deg.size();

	//3:
	vector<BigNum> Mass(n);
	BigNum cur;
	Mass[0] = _Num % _Mod;
	for (unsigned int i = 1; i < n; i++) {
		cur = Mass[i - 1] * Mass[i - 1];
		Mass[i] = cur % _Mod;
	}

	//4:
	BigNum ResRes(1);
	for (unsigned int i = 0; i < n; i++) {
		ResRes = ResRes * Mass[i].Pow(deg[(n - 1) - i] - 48);
	}
	BigNum result = ResRes % _Mod;

	//5:
	return result;
}

BigNum BigNum::Evk(const BigNum& a, const BigNum& b, BigNum& x, BigNum& y) {
	BigNum _A = a, _B = b, Zero;
	if (a < b) _A = b, _B = a;
	BigNum x1 = 0, y1 = 1, u = 1, v = 0;
	while (_B != Zero) {
		BigNum q = _A / _B;
		BigNum r = _A % _B;
		BigNum m = x1 - u * q;
		BigNum n = y1 - v * q;
		_A = _B;
		_B = r;
		x1 = u;
		y1 = v;
		u = m;
		v = n;
	}
	x = x1;
	y = y1;
	BigNum gcd = _A;
	return gcd;
}

//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//

bool operator!=(const BigNum& A, const BigNum& B) {
	if (A.LongNum.size() != B.LongNum.size()) {
		return true;
	}
	if (A.sign != B.sign) {
		return true;
	}

	for (int i = A.LongNum.size(); i > 0; i--) {
		if (A.LongNum[i - 1] != B.LongNum[i - 1]) {
			return true;
		}
	}

	return false;
}

bool operator==(const BigNum& A, const BigNum& B) {
	if (A.LongNum.size() != B.LongNum.size()) {
		return false;
	}

	if (A.sign != B.sign) {
		return false;
	}

	for (int i = A.LongNum.size(); i > 0; i--) {
		if (A.LongNum[i - 1] != B.LongNum[i - 1]) {
			return false;
		}
	}

	return true;
}

bool operator<(const BigNum& A, const BigNum& B) {
	if ((A.sign == false) & (B.sign == true)) {
		return false;
	}
	if ((A.sign == true) & (B.sign == false)) {
		return true;
	}
	if ((A.sign == true) & (B.sign == true))
	{
		if (A.LongNum.size() > B.LongNum.size()) {
			return true;
		}
		else if (A.LongNum.size() < B.LongNum.size()) {
			return false;
		}
		else {
			for (int i = A.LongNum.size() - 1; i >= 0; i--) {
				if (A.LongNum[i] > B.LongNum[i]) {
					return true;
				}
				else if (A.LongNum[i] == B.LongNum[i]) continue;
				else return false;

			}
		}
		return false;
	}
	else {

		if (A.LongNum.size() < B.LongNum.size()) {
			return true;
		}
		else if (A.LongNum.size() > B.LongNum.size()) {
			return false;
		}
		else {
			for (int i = A.LongNum.size() - 1; i >= 0; i--) {
				if (A.LongNum[i] < B.LongNum[i]) {
					return true;
				}
				else if (A.LongNum[i] == B.LongNum[i]) continue;
				else return false;

			}
		}

		return false;
	}
}

bool operator>(const BigNum& A, const BigNum& B) {
	if ((A.sign == false) & (B.sign == true)) {
		return true;
	}
	if ((A.sign == true) & (B.sign == false)) {
		return false;
	}
	if ((A.sign == true) & (B.sign == true))
	{
		if (A.LongNum.size() > B.LongNum.size()) {
			return false;
		}
		else if (A.LongNum.size() < B.LongNum.size()) {
			return true;
		}
		else {
			for (int i = A.LongNum.size() - 1; i >= 0; i--) {
				if (A.LongNum[i] < B.LongNum[i]) {
					return true;
				}
				else if (A.LongNum[i] == B.LongNum[i]) continue;
				else return false;

			}
		}
		return false;
	}
	else {

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
				else if (A.LongNum[i - 1] == B.LongNum[i - 1]);
				else return false;
			}
		}

		return false;
	}
}

bool operator<=(const BigNum& A, const BigNum& B) {
	if ((A.sign == false) & (B.sign == true)) {
		return false;
	}
	if ((A.sign == true) & (B.sign == false)) {
		return true;
	}
	if ((A.sign == true) & (B.sign == true))
	{
		if (A.LongNum.size() > B.LongNum.size()) {
			return true;
		}
		else if (A.LongNum.size() < B.LongNum.size()) {
			return false;
		}
		else {
			for (int i = A.LongNum.size() - 1; i >= 0; i--) {
				if (A.LongNum[i - 1] > B.LongNum[i - 1]) {
					return true;
				}
				else if (A.LongNum[i - 1] == B.LongNum[i - 1]) {
					continue;
				}
				else return false;

			}
			return true;
		}
		return false;
	}
	else {
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
				else if (A.LongNum[i - 1] == B.LongNum[i - 1]) {
					continue;
				}
				else {
					return false;
				}
			}
			return true;
		}
		return false;
	}
}

bool operator>=(const BigNum& A, const BigNum& B) {
	if ((A.sign == false) & (B.sign == true)) {
		return true;
	}
	if ((A.sign == true) & (B.sign == false)) {
		return false;
	}
	if ((A.sign == true) & (B.sign == true))
	{
		if (A.LongNum.size() > B.LongNum.size()) {
			return false;
		}
		else if (A.LongNum.size() < B.LongNum.size()) {
			return true;
		}
		else {
			for (int i = A.LongNum.size() - 1; i >= 0; i--) {
				if (A.LongNum[i - 1] < B.LongNum[i - 1]) {
					return true;
				}
				else if (A.LongNum[i - 1] == B.LongNum[i - 1]) {
					continue;
				}
				else return false;

			}
			return true;
		}
		return false;
	}
	else {

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
			return true;
		}
		return false;
	}
}

//--------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------//

BigNum& BigNum::operator=(const BigNum& A) {
	this->LongNum = A.LongNum;
	this->sign = A.sign;
	return *this;
}

BigNum operator+(const BigNum& A, const BigNum& B) {
	return BigNum::Add(A, B);
}

BigNum operator+=(BigNum& A, const BigNum& B)
{
	return A = BigNum::Add(A, B);
}

BigNum operator-(const BigNum& A, const BigNum& B) {
	return 	BigNum::Sub(A, B);
}

BigNum operator-=(BigNum& A, const BigNum& B)
{
	return A = BigNum::Sub(A, B);
}

BigNum operator*(const BigNum& A, const BigNum& B) {
	BigNum Res;
	BigNum::Mul(A, B, Res);
	return Res;
}

BigNum operator%(const BigNum& A, const BigNum& B) {
	BigNum IntDivRes(0), Rem(0);
	bool Mistake = false, flag = 0;
	BigNum::Div(A, B, IntDivRes, Rem, Mistake, flag);

	if (!Mistake) {
		return Rem;
	}
	else {
		return BigNum(0);
	}

}

BigNum operator/(const BigNum& A, const BigNum& B) {
	BigNum IntDivRes(0), Rem(0);
	bool Mistake = false, flag = 1;
	BigNum::Div(A, B, IntDivRes, Rem, Mistake, flag);

	if (!Mistake) {
		return IntDivRes;
	}
	else {
		return BigNum(0);
	}
}