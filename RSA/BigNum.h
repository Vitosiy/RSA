#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <bitset>

using namespace std;

class BigNum {
public:
	BigNum();
	BigNum(unsigned int x);

	//BigNum(const string& Num);
	BigNum(const unsigned int BlockSize, const unsigned int Offset, const std::string& Filename);
	BigNum(const std::string& Str);
	static BigNum Add(const BigNum& A, const BigNum& B);
	static BigNum Sub(const BigNum& A, const BigNum& B);
	static void Mul(const BigNum& A, const BigNum& B, BigNum& Res);
	static void Div(const BigNum& A, const BigNum& B, BigNum& IntegerResultOfDivision, BigNum& Reminder, bool& MistakeWasMade); //TODO
	
	friend bool operator == (const BigNum&, const BigNum&);
	friend bool operator < (const BigNum&, const BigNum&);
	friend bool operator > (const BigNum& lhs, const BigNum& rhs);
	friend bool operator <= (const BigNum& lhs, const BigNum& rhs);
	friend bool operator >= (const BigNum& lhs, const BigNum& rhs);

	BigNum& operator = (const BigNum&);
	friend BigNum operator + (const BigNum&, const BigNum&);
	friend BigNum operator+=(BigNum& A, const BigNum& B);
	friend BigNum operator - (const BigNum&, const BigNum&);
	friend BigNum operator * (const BigNum&, const BigNum&);
	friend BigNum operator % (const BigNum&, const BigNum&); 
	friend BigNum operator / (const BigNum&, const BigNum&); 

	std::string ToBin();
	BigNum& Pow(const unsigned int Times); 
	static BigNum& FastPow(BigNum& Num, BigNum& Deg, BigNum& Mod);
	unsigned int Size();
	void Print(bool flag);



private:
	void PrintP(bool flag);
	std::vector<unsigned int> LongNum;
	bool Sing : 1;
	bool NumCreated : 1;
	static const unsigned int Base = 0xFFFFFFFF;
	//vector<unsigned int> LongNum;
};