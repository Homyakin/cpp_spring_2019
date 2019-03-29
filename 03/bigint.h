#pragma once
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <iostream>


class BigInt;
BigInt operator-(BigInt a, const BigInt& b);
BigInt operator+(BigInt a, const BigInt& b);
bool operator!=(const BigInt& a, const BigInt& b);
bool operator>=(const BigInt& a, const BigInt& b);
bool operator<=(const BigInt& a, const BigInt& b);
bool operator>(const BigInt& a, const BigInt& b);
bool operator==(const BigInt& a, const BigInt& b);
bool operator<(const BigInt& a, const BigInt& b);


class BigInt
{
public:

	BigInt() : sign(0), number(new int64_t[1]), rank(1)
	{
		number[0] = 0;
	}

	BigInt(int64_t a)
	{
		if (a > 0) sign = 1;
		else if (a == 0) sign = 0;
		else
		{
			a *= -1;
			sign = -1;
		}
		if (a / numeral_system > 0)
		{
			malloc(2);
			number[0] = a % numeral_system;
			number[1] = a / numeral_system;
		}
		else
		{
			malloc(1);
			number[0] = a;
		}
	}

	BigInt(const BigInt& other)
	{
		malloc(other.rank);
		sign = other.sign;
		for (int i = 0; i < rank; ++i)
			number[i] = other.number[i];
	}

	BigInt& operator=(const BigInt& other)
	{
		if (this == &other)
			return *this;
		if (rank != other.rank)
			realloc(other.rank);
		sign = other.sign;
		for (int i = 0; i < rank; ++i)
			number[i] = other.number[i];
		return *this;
	}

	BigInt operator-() const
	{
		BigInt tmp(*this);
		tmp.sign *= -1;
		return tmp;
	}

	BigInt& operator+=(const BigInt& other)
	{
		bool rest = false;
		//складываем числа одинаковых знаков
		if((sign >= 0 && other.sign >= 0) || (sign < 0 && other.sign < 0))
		{
			for (int i = 0; i < std::min(rank, other.rank); ++i)
			{
				number[i] += other.number[i];
				if (number[i] / numeral_system)
				{
					if (i != rank - 1)
						number[i + 1] += 1;
					else rest = true;
					number[i] %= numeral_system;
				}
			}
			
			if (rest)
			{
				realloc(rank + 1);
				number[rank - 1] = 1;
			}
			if(sign == 0 && (rank > 1 || number[0] != 0)) sign = 1;
		}
		else
		{
			if (sign >= 0 && *this < -other)
			{
				//x < |-y|; x + -y = -(y - x)
				*this = -(-other - *this);
			}
			else if (sign >= 0) 
			{
				//x >= |-y|; x + -y = x - y
				*this -= -other;
			}
			else if (other < -*this)
			{
				//y < |-x|; -x + y = -(x - y)
				*this = -(-*this - other);
			}
			else 
			{
				//y >= |-x|; -x + y = y - x
				*this = other - -*this;
			}
		}
		return *this;
	}

	BigInt& operator-=(const BigInt& other)
	{
		//вычитаем из большего положительного меньшее
		if (sign >= 0 && other.sign >= 0 && *this >= other)
		{
			for (int i = 0; i < other.rank; ++i)
			{
				if (other.number[i] + number[i] >= numeral_system)
				{
					number[i] += numeral_system;
					--number[i + 1];
				}
				number[i] -= other.number[i];
			}
			
			size_t clr = 0;
			for (int i = rank - 1; i >= 0 && number[i] == 0; ++i) ++clr;
			if (clr == rank)
			{
				sign = 0;
				realloc(1);
			}
			else if (clr != 0)
				realloc(rank - clr);
		}
		else
		{
			if (sign == -1 && other.sign == -1)
			{
				//-x - -y = -x + y
				*this += -other;
			}
			else if (sign == -1 && other.sign >= 0)
			{
				//-x - y = -(x + y)
				*this = -(-*this + other);
			}
			else if (sign >= 0 && other.sign >= 0)
			{
				//x < y; x - y = -(y - x)
				*this = -(other - *this);
			}
			else
			{
				//x - -y = x + y
				*this += -other;
			}
		}
		return *this;
	}

	~BigInt()
	{
		delete[] number;
	}

	friend bool operator<(const BigInt& a, const BigInt& b);
	friend bool operator==(const BigInt& a, const BigInt& b);
	friend std::ostream& operator<<(std::ostream& out, const BigInt& b);

private:

	int8_t sign;
	int64_t* number;
	size_t rank;
	static const int64_t numeral_system = 1e+18;

	void realloc(size_t new_size)
	{
		if (new_size == rank) return;
		int64_t* tmp = number;
		number = new int64_t[new_size];
		std::copy(tmp, tmp + std::min(rank, new_size), number);
		delete[] tmp;
		rank = new_size;
	}

	void malloc(size_t new_size)
	{
		number = new int64_t[new_size];
		rank = new_size;
	}

};

std::ostream& operator<<(std::ostream& out, const BigInt& b)
{
	if (b.sign == -1) out << "-";
	for (int i = b.rank - 1; i >= 0; --i)
		out << b.number[i];
	return out;
}

bool operator<(const BigInt& a, const BigInt& b)
{
	if (a.sign < b.sign)
		return true;
	else if (b.sign < a.sign)
		return false;
	if (a.rank < b.rank)
		return (a.sign == -1 ? false : true);
	else if (b.rank < a.rank)
		return (a.sign == -1 ? true : false);
	
	for (int i = a.rank - 1; i >= 0; --i)
	{
		if (a.number[i] < b.number[i])
			return (a.sign == -1 ? false : true);
		else if (b.number[i] < a.number[i])
			return (a.sign == -1 ? true : false);
	}
	
	return false;
}

bool operator==(const BigInt& a, const BigInt& b)
{
	if (a.sign != b.sign)
		return false;
	if (a.rank != b.rank)
		return false;
	for (int i = a.rank - 1; i >= 0; --i)
	{
		if (a.number[i] != b.number[i])
			return false;
	}
	return true;
}

bool operator>(const BigInt& a, const BigInt& b)
{
	return b < a;
}

bool operator<=(const BigInt& a, const BigInt& b)
{
	return !(b < a);
}

bool operator>=(const BigInt& a, const BigInt& b)
{
	return !(a < b);
}
bool operator!=(const BigInt& a, const BigInt& b)
{
	return !(a == b);
}

BigInt operator+(BigInt a, const BigInt& b)
{
	return a += b;
}

BigInt operator-(BigInt a, const BigInt& b)
{
	return a -= b;
}
