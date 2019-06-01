#include "bigint.h"

#include <iostream>


BigInt::BigInt()
	: m_isPositive(true)
	, m_size(0)
{

}

BigInt::BigInt(const std::string &num)
{
	create(num);
}

BigInt::BigInt(const BigInt &B)
{
	m_nums = B.m_nums;
	m_isPositive = B.m_isPositive;
	m_size = B.m_size;
}

void BigInt::create(const std::string & num)
{
	unsigned int i = 0;
	if (num[i] == '-')
	{
		m_isPositive = false;
		i++;
	}
	else if (num[i] == '+')
	{
		m_isPositive = true;
		i++;
	}
	else
	{
		m_isPositive = true;
	}

	m_size = 0;
	for (; num[i] != '\0'; i++)
	{
		addToTail(num[i] - '0');
		m_size++;
	}

	trimZeros();
}

void BigInt::free()
{
	m_size = 0;
	m_nums.clear();
}

void BigInt::update(std::string & num)
{
	free();
	create(num);
}

void BigInt::print(std::ostream &out) const
{
	out << *this << std::endl;
}

std::ostream & BigInt::printWithoutLastA(std::ostream &out, unsigned int A) const
{
	List<int8_t>::iterator it = this->m_nums.begin();
	for (unsigned int i = 0; i < m_size - A; i++, ++it)
		out << (int)*it;

	return out;
}

BigInt BigInt::operator+(const BigInt &B)
{
	BigInt C;

	if (this->m_isPositive && !B.m_isPositive)
	{
		BigInt C;
		if (this->biggerOrEqual(B))
		{
			C = this->substract(B);
			C.m_isPositive = true;
		}
		else
		{
			C = B.substract(*this);
			C.m_isPositive = false;
		}
		return C;
	}
	else if (!this->m_isPositive && B.m_isPositive)
	{
		BigInt C;
		if (this->biggerOrEqual(B))
		{
			C = this->substract(B);
			C.m_isPositive = false;
		}
		else
		{
			C = B.substract(*this);
			C.m_isPositive = true;
		}
		return C;
	}

	C = this->add(B);

	if (!this->m_isPositive && !B.m_isPositive)
		C.m_isPositive = false;

	return C;
}

BigInt BigInt::operator-(const BigInt &B)
{
	if (this->biggerOrEqual(B))
	{
		if (this->m_isPositive)
			return (B.m_isPositive ? this->substract(B) : this->add(B));
		else
		{
			BigInt C = (B.m_isPositive ? this->add(B) : this->substract(B));
			C.m_isPositive = false;
			return C;
		}
	}
	else
	{
		if (B.m_isPositive)
		{
			BigInt C = (this->m_isPositive ? this->add(B) : this->substract(B));
			C.m_isPositive = false;
			return C;
		}
		else
			return (this->m_isPositive ? B.add(*this) : B.substract(*this));
	}
}

BigInt BigInt::operator*(const BigInt &B)
{
	BigInt C;

	List<int8_t>::iterator itB = B.m_nums.rbegin();

	int8_t count = 0;
	while (itB.isValid())
	{
		BigInt temp = this->operator*(*itB);
		for (int8_t k = 0; k < count; k++)
			temp.addToTail(0);
		C += temp;

		--itB;
		count++;
	}

	C.trimZeros();

	C.m_isPositive = !(this->m_isPositive ^ B.m_isPositive);

	return C;
}

BigInt BigInt::operator/(const BigInt &B)
{
	if (B.m_nums.isEmpty()) ///< Cant divide to 0!!
		return B;

	BigInt C, tempCopyThis = *this;

	int sizeA = 0, sizeB = 0;
	for(List<int8_t>::iterator itA = m_nums.begin(); itA != m_nums.end(); ++itA) sizeA++;
	for(List<int8_t>::iterator itB = B.m_nums.begin(); itB != m_nums.end(); ++itB) sizeB++;

    BigInt current, remainder;
	int addedThisRun;
	while (1)
	{
		current = remainder;
		{
			addedThisRun = 0;
			List<int8_t>::iterator itA = tempCopyThis.m_nums.begin();
			bool breakOuterLoop = false;
			while (1)
			{
				if (addedThisRun >= sizeA)
				{
					breakOuterLoop = true;
					break;
				}

				addedThisRun++;
				current.addToTail(*itA);
				if (current.biggerOrEqual(B) != false)
					break;

				C.addToTail(0);
				++itA;
			}
			if (breakOuterLoop)
				break;
		}

		int8_t result = current.divisionBySubtraction(B, remainder);
		if (result > 10)
			C.addToTail(result / 10);
		C.addToTail(result % 10);

		for (int i = 0; i < addedThisRun; i++)
		{
			tempCopyThis.deleteFromHead();
			sizeA--;
		}
	}

	C.m_isPositive = !(this->m_isPositive ^ B.m_isPositive);

	C.trimZeros();

	return C;
}

BigInt BigInt::pow(const unsigned int b)
{
	BigInt C("1");
	for (unsigned int i = 0; i < b; i++)
		C *= *this;

	return C;
}

BigInt & BigInt::operator+=(const BigInt &B)
{
	*this = *this + B;
	return *this;
}

BigInt & BigInt::operator-=(const BigInt &B)
{
	*this = *this - B;
	return *this;
}

BigInt & BigInt::operator*=(const BigInt &B)
{
	*this = *this * B;
	return *this;
}

BigInt & BigInt::operator/=(const BigInt &B)
{
	*this = *this / B;
	return *this;
}

bool BigInt::operator==(const BigInt &B)
{
    List<int8_t>::iterator itA = m_nums.end();
    List<int8_t>::iterator itB = B.m_nums.end();

    while (itA != m_nums.rend() || itB != B.m_nums.rend())
    {
        if (itA != itB)
        {
            return false;
        }

        ++itA;
        ++itB;
    }

    return itA == m_nums.rend() && itB == B.m_nums.rend();
}

bool BigInt::operator!=(const BigInt &B)
{
    return !(operator==(B));
}

void BigInt::trimZeros()
{
	for (List<int8_t>::iterator it = m_nums.begin(); it != m_nums.rbegin(); )
	{
		if (*it != 0)
			break;

		++it;
		deleteFromHead();
	}

    // if there are no digits left
	if (m_nums.isEmpty())
	{
		m_nums.addToTail(0);
		m_isPositive = true;
	}
}

BigInt BigInt::operator*(const int8_t b)
{
	BigInt C;
	List<int8_t>::iterator itA = m_nums.rbegin();

	int8_t carry = 0;
	while (itA != m_nums.rend())
	{
		int8_t result = (*itA) * b + carry;
		carry = result / 10;
		result = result % 10;

		C.addToHead(result);

		--itA;
	}

	if (carry != 0)
		C.addToHead(carry);

	return C;
}

BigInt & BigInt::operator*=(int8_t b)
{
	*this = *this * b;
	return *this;
}

int BigInt::divisionBySubtraction(const BigInt &B, BigInt &Remainder)
{
	Remainder = *this;
	int i = 0;
	while (Remainder.biggerOrEqual(B))
	{
		Remainder = Remainder.substract(B);
		i++;
	}

	return i;
}

BigInt BigInt::substract(const BigInt &B) const
{    
	BigInt C = *this;

	List<int8_t>::iterator itC = C.m_nums.rbegin();
	List<int8_t>::iterator itB = B.m_nums.rbegin();

    // Start from end (least significant digit) and
    // do substraction as we go left.

    // when itB becomes invalid, itA should still be valid
    while (itB.isValid())
	{
		int a = itC.isValid() ? *itC : 0;
		int b = itB.isValid() ? *itB : 0;

		int result = a - b;
		if (result < 0)
		{
			for (List<int8_t>::iterator itC2 = List<int8_t>::iterator(itC->prev); itC2 != C.m_nums.rend(); --itC2)
			{
				if (*itC2 > 0)
				{
					itC2->info--;
					for (List<int8_t>::iterator itC3 = List<int8_t>::iterator(itC2->next); itC3 != itC; ++itC3)
						itC3->info = 9;
					break;
				}
			}

			result += 10;
		}

		itC->info = result;

		--itC;
		--itB;
	}

	C.trimZeros();

	return C;
}

BigInt BigInt::add(const BigInt &B) const
{
	BigInt C;

	List<int8_t>::iterator itA = m_nums.rbegin();
	List<int8_t>::iterator itB = B.m_nums.rbegin();

    // Start from end (least significant digit) and
    // do addition as we go left.
	int8_t carry = 0;
	while (1)
	{
		if ((itA.isValid() && itB.isValid()) == false && carry == 0)
		{
			if (itA.isValid())
			{
				for (; itA.isValid(); --itA)
					C.addToHead(*itA);
			}
			else if (itB.isValid())
			{
				for (; itB.isValid(); --itB)
					C.addToHead(*itB);
			}

            // if we dont have any digits to add and carry is 0
			break;
		}

		int a = itA.isValid() ? *itA : 0;
		int b = itB.isValid() ? *itB : 0;

		int8_t result = a + b + carry;
		carry = result / 10;
		result = result % 10;

		C.addToHead(result);

		if (itA.isValid()) --itA;
		if (itB.isValid()) --itB;
	}

	if (carry != 0)
		C.addToHead(carry);

	return C;
}

bool BigInt::biggerOrEqual(const BigInt &B) const
{
	int result = 0;
	List<int8_t>::iterator itA = m_nums.begin();
	List<int8_t>::iterator itB = B.m_nums.begin();

    // Start from beginning (most significant digit) and
    // check if one has less digits or is smaller.
	while (1)
	{
		if (itA.isValid() && !itB.isValid())
			return true; ///< bigger
		else if (!itA.isValid() && itB.isValid())
			return false; ///< smaller
		else if (!itA.isValid() && !itB.isValid())
			return result != 2; ///< bigger or equal

		if (result == 0 && *itA > *itB)
		{
			result = 1;
		}
		else if (result == 0 && *itA < *itB)
		{
			result = 2;
		}

		++itA;
		++itB;
	}
}
