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
	BigNum(const std::string& Str);
	
	static BigNum Add(const BigNum& A, const BigNum& B);
	static BigNum Sub(const BigNum& A, const BigNum& B);
	static void Mul(const BigNum& A, const BigNum& B, BigNum& Res);
	static void Div(const BigNum& A, const BigNum& B, BigNum& IntegerResultOfDivision, BigNum& Reminder, bool& MistakeWasMade, bool& flag);
	
	friend bool operator == (const BigNum&, const BigNum&);
	friend bool operator < (const BigNum&, const BigNum&);
	friend bool operator > (const BigNum& lhs, const BigNum& rhs);
	friend bool operator <= (const BigNum& lhs, const BigNum& rhs);
	friend bool operator >= (const BigNum& lhs, const BigNum& rhs);
	friend bool operator!=(const BigNum&, const BigNum&);

	BigNum& operator = (const BigNum&);
	friend BigNum operator + (const BigNum&, const BigNum&);
	friend BigNum operator+=(BigNum& A, const BigNum& B);
	friend BigNum operator - (const BigNum&, const BigNum&);
	friend BigNum operator * (const BigNum&, const BigNum&);
	friend BigNum operator % (const BigNum&, const BigNum&); 
	friend BigNum operator / (const BigNum&, const BigNum&); 

	string ToBin();
	BigNum& Pow(const unsigned int Times); 
	static BigNum FastPow(BigNum& Num, BigNum& Deg, BigNum& Mod);
	static BigNum Evk(const BigNum& a, const BigNum& b, BigNum& x, BigNum& y);

	unsigned int Size();
	void Print(bool flag);
	void PrintF(std::ofstream& Filename);
	   	 
private:
	void PrintP(bool flag);
	std::vector<unsigned int> LongNum;
	bool Sing : 1;
	bool NumCreated : 1;
	bool sign = false;
};