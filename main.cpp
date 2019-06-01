#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <math.h>
#include <chrono>

#include "bigint.h"


void testBigInt()
{
	std::string num1 = "700000";
	std::string num2 = "20";

	BigInt two("2"), a, b, c, d, e, f;
	a.create(num1);
	b.create(num2);

	c = a + b;
	c.print(std::cout);
	c += two;
	c.print(std::cout);

	e = a - b;
	e.print(std::cout);
	e -= two;
	e.print(std::cout);

	d = a * b;
	d.print(std::cout);
	d *= two;
	d.print(std::cout);

	f = a / b;
	f.print(std::cout);
	f /= two;
	f.print(std::cout);
}

int getFirstNonDigitIf(std::string &a, int from)
{
    int i;
    for (i = from; a[i] != '\0'; i++)
	{
		if (a[i] < '0' || a[i] > '9')
			return i;
	}
	return i;
}

int operatorPrecedenceComparison(char a, char b)
{
	if ((a == '*' || a == '/') && (b == '+' || b == '-'))
		return 1;
	else if ((a == '+' || a == '-') && (b == '*' || b == '/'))
		return -1;
	else
		return 0;
}

void testStackAgainstOperator(std::string &postfix, std::stack<char> &stack, char op)
{
	if (stack.empty())
	{
		stack.push(op);
		return;
	}

	if (operatorPrecedenceComparison(op, stack.top()) == 1)
	{
		stack.push(op);
	}
	else if (operatorPrecedenceComparison(op, stack.top()) == 0)
	{
		char temp = stack.top();
		stack.pop();
		postfix.push_back(temp);
		stack.push(op);
	}
	else if (operatorPrecedenceComparison(op, stack.top()) == -1)
	{
		char temp = stack.top();
		stack.pop();
		postfix.push_back(temp);
		testStackAgainstOperator(postfix, stack, op);
	}
}

std::string infixToPostfix(std::string infix)
{
	std::string postfix;
	std::stack<char> stack;

	for (unsigned int i = 0; i < infix.size(); i++)
	{
		if (infix[i] >= 'A')
		{
			postfix.push_back(infix[i]);
		}
		else if (stack.empty() || stack.top() == '(')
		{
			stack.push(infix[i]);
		}
		else if (infix[i] == '(')
		{
			stack.push(infix[i]);
		}
		else if (infix[i] == ')')
		{
			char temp = stack.top();
			stack.pop();
			while (temp != '(')
			{
				postfix.push_back(temp);
				temp = stack.top();
				stack.pop();
			}
		}
		else
		{
			testStackAgainstOperator(postfix, stack, infix[i]);
		}
	}

	while (stack.empty() == false)
	{
		postfix.push_back(stack.top());
		stack.pop();
	}

	return postfix;
}

std::ostream &outputPi(std::ostream &out, int numOfDigitsAfterPoint)
{
    std::cout << "Calculation of first " << numOfDigitsAfterPoint << " nums of PI has started..." << std::endl << std::endl;
	auto startTime = std::chrono::system_clock::now();
    constexpr int addedDigits = 3;
    numOfDigitsAfterPoint += addedDigits; ///< To force it to be as accurate as possible
	BigInt fractionOfPi;

	BigInt multiNum("2"), one("1"), two("2"), four("4");
	for (int i = 0; i < numOfDigitsAfterPoint; i++)
		four.addToTail(0);

    BigInt count("0");
	bool isAdd = true;
	while (1)
	{
		count += one;
		BigInt tempMult = multiNum*(multiNum+one)*(multiNum+two);
		BigInt temp = four / tempMult;
		if (temp.size() == 0)
			break;

		if (isAdd)
			fractionOfPi += temp;
		else
			fractionOfPi -= temp;

		isAdd = !isAdd;
		multiNum += two;

        // print current PI calculated
        std::cout << "\rPI: 3.";
        fractionOfPi.printWithoutLastA(std::cout, addedDigits) << " - process: " << count;
        out << "3." << fractionOfPi << std::endl;
	}
	std::cout << std::endl;
	auto endTime = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = endTime-startTime;
	std::cout << "Calculation of PI has finished" << std::endl;
	std::cout << "TotalSecondsPassed: " << elapsed_seconds.count() << std::endl;

	for (int i = 0; i < addedDigits; i++)
		fractionOfPi.deleteFromTail();

	out << "3." << fractionOfPi;
	return out;
}

int main()
{
	// testBigInt();

	BigInt result;

    std::ofstream out("pi.txt", std::ofstream::trunc);
    if (out)
    {
        outputPi(out, 60) << std::endl;
    }

	return 0;
}
