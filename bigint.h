#pragma once

#include <string>
#include <ostream>

#include "list.h"


class BigInt
{
public:
	BigInt();
	BigInt(const std::string &num);
	BigInt(const BigInt &B);
	void create(const std::string &num);
	void free();
	void update(std::string & num);
	void print(std::ostream &out) const;
	std::ostream &printWithoutLastA(std::ostream &out, unsigned int A) const;

	BigInt operator+(const BigInt &B);
	BigInt operator-(const BigInt &B);
	BigInt operator*(const BigInt &B);
    // Used for multiplying with a single digit
    BigInt operator*(const int8_t b);
    BigInt operator/(const BigInt &B);
	BigInt pow(const unsigned int b);

	BigInt & operator+=(const BigInt &B);
	BigInt & operator-=(const BigInt &B);
	BigInt & operator*=(const BigInt &B);
    // Used for multiplying with a single digit
    BigInt & operator*=(const int8_t b);
	BigInt & operator/=(const BigInt &B);

    bool operator==(const BigInt &B);
    bool operator!=(const BigInt &B);

	void addToHead(int8_t a) { m_nums.addToHead(a); m_size++; }
	void addToTail(int8_t a) { m_nums.addToTail(a); m_size++; }
	int8_t deleteFromHead() { m_size--; return m_nums.deleteFromHead(); }
	int8_t deleteFromTail() { m_size--; return m_nums.deleteFromTail(); }
	List<int8_t>::iterator begin() { return m_nums.begin(); }
	List<int8_t>::iterator rbegin() { return m_nums.rbegin(); }
	void trimZeros();
	int size() const { return m_size; }

protected:
	friend std::ostream &operator<<(std::ostream &out, const BigInt &B)
	{
		if (B.m_nums.begin().isValid())
			for (ListIterator<int8_t> it = B.m_nums.begin(); it != B.m_nums.end(); ++it)
				out << (int)it->info;
		else
			out << "0";
		return out;
	}

private:
    // helper functions that do the real work
	int divisionBySubtraction(const BigInt &B, BigInt &Remainder); ///< Helper function for division
	BigInt substract(const BigInt &B) const; ///< Only substract bigger num from smaller
	BigInt add(const BigInt &B) const;
	bool biggerOrEqual(const BigInt &B) const; ///< compares without checking signs

private:
	List<int8_t> m_nums;
	bool m_isPositive;

    // number of digits we have
	unsigned int m_size;
};
