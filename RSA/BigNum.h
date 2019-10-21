#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

class BigNum {
public:
	BigNum();
	BigNum(unsigned int x);

	//BigNum(const string& Num);
	BigNum(const unsigned int BlockSize, const unsigned int Offset, const std::string& Filename);
	BigNum(const std::string& Str);
	void Print();
	static BigNum Add(const BigNum& A, const BigNum& B);
	static BigNum Sub(const BigNum& A, const BigNum& B);
	static void Mul(const BigNum& A, const BigNum& B, BigNum& Res);
	static void Div(BigNum& A, unsigned int B, BigNum& Reminder);
	static void Div(BigNum& A, unsigned int B);
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
	friend BigNum operator / (BigNum&, unsigned int);



	BigNum& Pow(const unsigned int Times); 
	BigNum& FastPow(BigNum& Num, BigNum& Deg, BigNum& Mod);
	unsigned int Size();
	string NumAsString();



private:
	std::vector<unsigned int> LongNum;
	bool Sing : 1;
	bool NumCreated : 1;
	static const unsigned int Base = 0xFFFFFFFF;
	//vector<unsigned int> LongNum;
};