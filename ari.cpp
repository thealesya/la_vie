#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class BigInt
{
private:
	string value;
	bool isNeg;
public:
	BigInt(); 
	BigInt(int x); 
	BigInt(const string& value); 

	const string& getValue() const; 
	const bool getIsNeg() const; 

	const bool operator == (const BigInt& bigInt) const; 
	const bool operator != (const BigInt& bigInt) const; 
	const bool operator < (const BigInt& bigInt) const; 
	const bool operator > (const BigInt& bigInt) const; 

	BigInt& operator = (const BigInt& bigInt);

	BigInt operator + (const BigInt& bigInt) const; 
	BigInt operator - (const BigInt& bigInt) const; 
	BigInt operator * (const BigInt& bigInt) const; 
	BigInt operator / (const BigInt& bigInt) const; 

	BigInt operator+() const; 
	BigInt operator-() const; 

	friend istream& operator >> (istream& stream, BigInt& bigInt); 
	friend ostream& operator << (ostream& stream, const BigInt& bigInt); 
};

BigInt::BigInt()
{
	
	this->value = "0";
	this->isNeg = false; 
}

BigInt::BigInt(int x) 
{

	this->isNeg = x < 0 ? true : false; 
	this->value = to_string(isNeg ? -x : x);
}

BigInt::BigInt(const string& value)
{
	
	if (value.length() == 0) 
	{
		this->value = "0";
		this->isNeg = false;
		return;
	}

	isNeg = value[0] == '-'; 
	this->value = value.substr(isNeg); 
	int counter = 0;					

	while (this->value[counter] == '0' && (this->value.length() - counter) > 1) 
	{
		counter++;
	}

	this->value = this->value.substr(counter); 

	try
	{
		
		for (int i = 0; i < this->value.length(); i++) {
			if (this->value[i] < '0' || this->value[i] > '9')
			{
				throw this->value[i];
			}
		}
	}
	catch (const char err)
	{
		cout << "Incorrect symbol: " << err << "\n";
		exit(0);
	}
}

const string& BigInt::getValue() const
{
	return value;
}


const bool BigInt::getIsNeg() const
{
	return isNeg;
}


const bool BigInt::operator == (const BigInt& bigInt) const
{
	
	return ((value == bigInt.getValue() && isNeg == bigInt.getIsNeg()) ? true : false);
}

const bool BigInt::operator != (const BigInt& bigInt) const
{
	
	return (value != bigInt.getValue() || isNeg != bigInt.getIsNeg()) ? true : false;
}

const bool BigInt::operator < (const BigInt& bigInt) const 
{
	string value2 = bigInt.getValue(); 
	int len1 = value.length();
	int len2 = value2.length(); 

	if (isNeg == bigInt.getIsNeg())
	{ 
		if (len1 != len2) 
		{ 
			return (len1 < len2) ^ isNeg; 
		}
		int i = 0;
		while (i < len1 && value[i] == value2[i])
		{
			i++;
		}

		
		return (i < len1) && ((value[i] < value2[i]) ^ isNeg);
	}
	return isNeg; 
}

const bool BigInt::operator > (const BigInt& bigInt) const
{
	
	return !(*this < bigInt || *this == bigInt);
}

BigInt& BigInt::operator = (const BigInt& bigInt)
{
	
	value = bigInt.getValue();
	isNeg = bigInt.getIsNeg();

	return *this;
}

BigInt BigInt::operator +() const
{
	
	return BigInt(*this);
}

BigInt BigInt::operator -() const 
{
	
	return BigInt(isNeg ? value : string("-") + value);
}

BigInt BigInt::operator + (const BigInt& bigInt) const
{
	if (isNeg == bigInt.getIsNeg()) 
	{ 
		string num2 = bigInt.getValue();

		int len1 = value.length(); 
		int len2 = num2.length(); 
		int length = (len1 < len2) ? len2 + 1 : len1 + 1;

		int* a = new int[length]; 
		int* b = new int[length];
		char* res = new char[length + 1]; 
		res[length] = '\0';

		for (int i = 0; i < length; i++) 
		{
			
			a[i] = (i < len1) ? (value[len1 - 1 - i] - '0') : 0;
			b[i] = (i < len2) ? (num2[len2 - 1 - i] - '0') : 0; 
			res[i] = 0;											
		}

		for (int i = 0; i < length; i++)
		{
			
			res[length - 1 - i] += a[i] + b[i]; 
			res[length - 1 - 1 - i] += res[length - 1 - i] / 10; 
		
			res[length - 1 - i] %= 10; 
		}
	
		for (int i = 0; i < length; i++)
		{
			res[length - 1 - i] += '0';
		}


		string result = isNeg ? string("-") + string(res) : string(res);
		delete[] a;
		delete[] b;
		delete[] res;
		return BigInt(result);
	}
	else
	{
	
		return isNeg ? (bigInt - (-BigInt(*this))) : (*this - (-BigInt(bigInt)));
	}
}

BigInt BigInt::operator - (const BigInt& bigInt) const
{
	if (*this == bigInt) 
	{
		return 0;
		
	}
	if (!isNeg && !bigInt.getIsNeg())
	{ 
		string value2 = bigInt.getValue();
		int len1 = value.length();
		int len2 = value2.length(); 
		int length = (len1 < len2 ? len2 : len1);

		int* a = new int[length]; 
		int* b = new int[length];

		bool isNegRes = bigInt > * this; 

		char* res = new char[length + 1];
		res[length] = '\0';

		for (int i = 0; i < length; i++) 
		{ 
			a[i] = (i < len1) ? (value[len1 - 1 - i] - '0') : 0;
			b[i] = (i < len2) ? (value2[len2 - 1 - i] - '0') : 0;
			res[i] = 0;
		}

		for (int i = 0; i < length; i++) {
			if (!isNegRes)
			{ 
				if (a[i] - b[i] < 0)
				{
					a[i + 1] += -1;
					res[length - 1 - i] += 10 + (a[i] - b[i]);
					res[length - 1 - i - 1] += res[length - 1 - i] / 10;
					res[length - 1 - i] %= 10;
				}
				else
				{
					res[length - 1 - i] += (a[i] - b[i]);
					res[length - 1 - i - 1] += res[length - 1 - i] / 10;
					res[length - 1 - i] %= 10;
				}
			}
			else 
			{
				if (b[i] - a[i] < 0) 
				{ 
					b[i + 1] += -1;
					res[length - 1 - i] += 10 + (b[i] - a[i]);
					res[length - 1 - i - 1] += res[length - 1 - i] / 10; 
					res[length - 1 - i] %= 10; 
				}
				else 
				{ 
					res[length - 1 - i] += (b[i] - a[i]);
					res[length - 1 - i - 1] += res[length - 1 - i] / 10;
					res[length - 1 - i] %= 10;
				}
			}
		}
	
		for (int i = 0; i < length; i++)
		{
			res[length - 1 - i] += '0';
		}

		
		string result = isNegRes ? string("-") + string(res) : string(res);
		delete[] a;
		delete[] b;
		delete[] res;
		return BigInt(result);

	}
	else 
	{
		return isNeg && bigInt.getIsNeg() ? (-BigInt(bigInt) - (-BigInt(*this))) : (*this + -BigInt(bigInt));
	}
}

BigInt BigInt::operator * (const BigInt& bigInt) const 
{
	if (value == "0" || bigInt.getValue() == "0") 
	{
		return 0;
	}
	string value2 = bigInt.getValue(); 

	int len1 = value.length(); 
	int len2 = value2.length(); 
	int length = len1 + len2 + 1; 
	bool isNegRes = isNeg ^ bigInt.getIsNeg(); 

	int* a = new int[length];
	int* b = new int[length];
	char* res = new char[length + 1];
	res[length] = '\0';

	for (int i = 0; i < length; i++) 
	{ 
		a[i] = (i < len1) ? (value[len1 - 1 - i] - '0') : 0;
		b[i] = (i < len2) ? (value2[len2 - 1 - i] - '0') : 0;
		res[i] = 0;
	}
	
	for (int i = 0; i < len1; i++) 
	{
		for (int j = 0; j < len2; j++)
 {
			res[length - 1 - (i + j)] += a[i] * b[j];
			res[length - 1 - 1 - (i + j)] += res[length - 1 - (i + j)] / 10;
			res[length - 1 - (i + j)] %= 10;
		}
	}

	for (int i = 0; i < length; i++) {
		res[length - 1 - i] += '0';
	}

	string result = isNegRes ? string("-") + string(res) : string(res);
	delete[] a;
	delete[] b;
	delete[] res;

	return BigInt(result);
}

BigInt BigInt::operator / (const BigInt& bigInt) const {
	string value1 = value;
	string value2 = bigInt.getValue(); 
	try
	{
		if (value2 == "0") 
		{
			throw; 
		}
	}
	catch (const exception& err)
	{
		cout << err.what() << "\n";
		exit(0);
	}

	if (value1 == "0") 
	{
		return 0; 
	}

	if (value1 == "1") 
	{
		return BigInt(bigInt.getIsNeg() ? -BigInt(*this) : *this); 
	}

	int zeroes = 0;
	while (value2[value2.length() - 1 - zeroes] == '0')
	{
		zeroes++; 
	}

	if (zeroes >= value.length())
	{
		return 0;
	}

	if (zeroes) 
	{
		value1 = value1.substr(0, value.length() - zeroes);
		value2 = value2.substr(0, value2.length() - zeroes);
	}

	bool isNegRes = isNeg ^ bigInt.getIsNeg(); 

	BigInt tmp(value2);
	int divider_length = value2.length(); 
	int length = value1.length();
	int index = 0;
	string div; 
	string v; 

	
	while (BigInt(v) < tmp && index < length) 
	{
		v += value[index++];
	}

	do {
		int count = 0; 

		
		if (BigInt(v) > tmp || BigInt(v) == tmp)
		{
			BigInt mod = v;

			
			while (mod > tmp || mod == tmp) 
			{
				mod = mod - tmp;
				count++;
			}

			v = mod.getValue();
		}

		div += count + '0'; 

		if (index <= length)
		{
			v += value1[index++];
		}
	} while (index <= length);


	return BigInt(isNegRes && div != "0" ? string("-") + div : div);
}

istream& operator >> (istream& stream, BigInt& bigInt)
{
	string value;
	stream >> value;
	bigInt = BigInt(value);

	return stream;
}

ostream& operator << (ostream& stream, const BigInt& bigInt)
{
	if (bigInt.getIsNeg()) 
	{
		stream << "-";
	}
	return stream << bigInt.getValue();
}

int main()
{

	BigInt a;
	BigInt b;
	cin >> a;
	cin >> b;

	cout << "-------------------------------------------------------------------" << "\n";
	cout << "-------------------------------------------------------------------" << "\n";
	cout << "-------------------------------------------------------------------" << "\n";
	cout << "-------------------------------------------------------------------" << "\n";
	cout << "-------------------------------------------------------------------" << "\n";
	cout << a + b << "\n";
	cout << "-------------------------------------------------------------------" << "\n";
	cout << a - b << "\n";
	cout << "-------------------------------------------------------------------" << "\n";
	cout << a * b << "\n";
	cout << "-------------------------------------------------------------------" << "\n";
	cout << a / b << "\n";
	cout << "-------------------------------------------------------------------" << "\n";
}