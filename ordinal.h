#pragma once
#include <utility>
#include <iostream>

class ordinal
{
public:
	using natural = unsigned long long;
	struct term
	{
		ordinal* exponent = nullptr;
		natural coeff = 0;

		ordinal getExponent() const { return *exponent; }
		bool isNatural() const { return *exponent == 0; }

		term() {
			exponent = new ordinal();
		}
		term(const term& other)
		{
			exponent = new ordinal(*(other.exponent));
			coeff = other.coeff;
		}

		term operator=(natural n)
		{
			*exponent = 0;
			coeff = n;
			return *this;
		}
		term operator=(const term& other)
		{
			*exponent = *(other.exponent);
			coeff = other.coeff;
			return *this;
		}
		term operator=(term&& other)
		{
			exponent = other.exponent;
			other.exponent = nullptr;
			coeff = other.coeff;
			return *this;
		}
		
		~term()
		{
			delete exponent;
		}

		bool operator==(natural n)
		{
			if (*exponent == 0 && coeff == n)
				return true;
			return false;
		}
		bool operator<(natural n)
		{
			if (*exponent == 0 && coeff < n)
				return true;
			return false;
		}
		bool operator>(natural n)
		{
			if (*exponent > 0 || (*exponent == 0 && coeff > n))
				return true;
			return false;
		}

		bool operator==(const term& other)
		{
			if (*exponent == *(other.exponent) && coeff == other.coeff)
				return true;
			return false;
		}
		bool operator<(const term& other)
		{
			if (*exponent < *(other.exponent) || (*exponent == *(other.exponent) && coeff < other.coeff))
				return true;
			return false;
		}
		bool operator>(const term& other)
		{
			if (*exponent > *(other.exponent) || (*exponent == *(other.exponent) && coeff > other.coeff))
				return true;
			return false;
		}
		bool operator!=(const term& other)
		{
			return !(*this == other);
		}
	};
private:
	term* m_data = nullptr;
	size_t termcount = 0;
private:
	void _setToZero()
	{
		delete[] m_data;
		m_data = nullptr;
		termcount = 0;
	}
	bool _isNatural() const
	{
		return !m_data || (m_data[0].getExponent() == 0);
	}
	bool _hasNaturalTerm() const
	{
		return m_data[termcount - 1].getExponent() == 0;
	}
	natural _getNaturalTerm() const
	{
		if (_hasNaturalTerm())
			return m_data[termcount - 1].coeff;
		return 0;
	}
	void _addnatural(natural n)
	{
		if (termcount == 0) //LHS == 0
		{
			*this = n;
			return;
		}
		if (_hasNaturalTerm()) //LHS has natural term
		{
			m_data[termcount - 1].coeff += n;
			return;
		}
		term* tmp = new term[termcount+1];
		for (size_t i = 0; i < termcount; i++)
		{
			tmp[i] = m_data[i];
		}
		tmp[termcount++] = n;
			
		delete[] m_data;
		m_data = tmp;
	}
	void _add(const ordinal& other)
	{
		if (termcount == 0) //LHS == 0
		{
			*this = other;
			return;
		}
		if (other.m_data) //LHS, RHS != 0
		{
			size_t precTerms = 0;
			while (precTerms < termcount && m_data[precTerms].getExponent() > other.m_data[0].getExponent())
			{
				precTerms++;
			}
			term* tmp = new term[precTerms + other.termcount];
			for (size_t i = 0; i < precTerms; i++)
			{
				tmp[i] = m_data[i];
			}
			for (size_t i = 0; i < other.termcount; i++)
			{
				tmp[i + precTerms] = other.m_data[i];
			}
			if (precTerms < termcount && m_data[precTerms].getExponent() == other.m_data[0].getExponent())
				tmp[precTerms].coeff += m_data[precTerms].coeff;

			delete[] m_data;
			m_data = tmp;
			termcount = precTerms + other.termcount;
		}
	}
	void _mulnatural(natural n)
	{
		if (termcount != 0) //LHS != 0
		{
			if (n == 0) //RHS == 0
			{
				_setToZero();
				return;
			}

			//LHS, RHS != 0
			m_data[0].coeff *= n;
		}
	}
	void _mul(const ordinal& other)
	{
		if (termcount != 0) //LHS != 0
		{
			if (other.termcount == 0) //RHS == 0
			{
				_setToZero();
				return;
			}

			//LHS, RHS != 0
			term* tmp = new term[other.termcount + termcount - 1];
			for (size_t i = 0; i < other.termcount; i++)
			{
				tmp[i].coeff = other.m_data[i].coeff;
				*(tmp[i].exponent) = m_data[0].getExponent();
				*(tmp[i].exponent) += other.m_data[i].getExponent();
			}
			for (size_t i = 1; i < termcount; i++)
			{
				tmp[other.termcount - 1 + i] = m_data[i];
			}

			if (other._hasNaturalTerm()) //RHS has natural term
				tmp[other.termcount - 1].coeff *= m_data[0].coeff;

			delete[] m_data;
			m_data = tmp;
			termcount = other.termcount + termcount - 1;
		}
	}
	void _expnatural(natural n)
	{
		if (*this == 0 || *this == 1)
			return;
		switch (n) {
		case 0:
			*this = 1;
			return;
		case 1:
			return;
		default:
		{
			if (_isNatural()) 
			{
				m_data[0].coeff = pow(m_data[0].coeff, n);
				return;
			}
			else
			{
				term* tmp = new term[n*(termcount-1)+1];
				*(tmp[0].exponent) = m_data[0].getExponent();
				*(tmp[0].exponent) *= n;
				tmp[0].coeff = m_data[0].coeff;
				for (natural i = 0; i < n; i++) {
					for (size_t j = 1; j < termcount; j++)
					{
						*(tmp[i * (termcount - 1) + j].exponent) = m_data[0].getExponent();
						*(tmp[i * (termcount - 1) + j].exponent) *= (n - i - 1);
						*(tmp[i * (termcount - 1) + j].exponent) += m_data[j].getExponent();


						tmp[i * (termcount - 1) + j].coeff = m_data[j].coeff;
					}
				}
				if (_hasNaturalTerm())
				{
					for (natural i = 0; i < n; i++) {
						tmp[(i + 1) * (termcount - 1)].coeff *= m_data[0].coeff;
					}
				}
				
				delete[] m_data;
				m_data = tmp;
				termcount = n * (termcount - 1) + 1;
			}
		}
		}
	}
	void _exp(const ordinal& other)
	{
		if (*this == 0 || *this == 1) //LHS == 0 or 1
			return;
		if (ordinal(0) == other) //RHS == 0
		{
			*this = 1;
			return;
		}
		if (ordinal(1) == other) //RHS == 1
			return;
		if (other._isNatural()) //RHS natural
		{
			_expnatural(other.m_data[0].coeff);
			return;
		}
		if (_isNatural()) //LHS natural, RHS transfinite
		{
			ordinal exp;
			if (other._hasNaturalTerm())
			{
				m_data[0].coeff = pow(_getNaturalTerm(), other._getNaturalTerm());
				exp.m_data = new term[other.termcount - 1];
				for (size_t i = 0; i < other.termcount - 1; i++)
				{
					exp.m_data[i] = other.m_data[i];
				}
				exp.termcount = other.termcount - 1;
			}
			else
			{
				m_data[0].coeff = 1;
				exp = other;
			}
			for (size_t i = 0; i < exp.termcount; i++)
			{
				if (exp.m_data[i].getExponent()._isNatural())
				{
					(exp.m_data[i].exponent->m_data[0].coeff)--;
				}
			}
			*(m_data[0].exponent) = exp;
			return;
		}
		//LHS, RHS transfinite
		ordinal exp;
		if (other._hasNaturalTerm())
		{
			exp.m_data = new term[other.termcount - 1];
			for (size_t i = 0; i < other.termcount - 1; i++)
			{
				exp.m_data[i] = other.m_data[i];
			}
			exp.termcount = other.termcount - 1;
		}
		else
		{
			exp = other;
		}

		ordinal term1 = m_data[0].coeff;
		*(term1.m_data[0].exponent) = *(m_data[0].exponent);
		*(term1.m_data[0].exponent) *= exp;

		ordinal term2 = *this;
		term2._expnatural(other._getNaturalTerm());

		term1._mul(term2);

		*this = term1;
	}
public:
	//Constructors
	ordinal(natural n = 0) 
	{
		if (n != 0)
		{
			m_data = new term[1];
			m_data[0] = n;
			termcount = 1;
		}
	}
	ordinal(const ordinal& other)
	{
		if (other.m_data)
		{
			termcount = other.termcount;
			m_data = new term[termcount];
			for (size_t i = 0; i < termcount; i++)
			{
				m_data[i] = other.m_data[i];
			}
		}
	}
	ordinal(ordinal&& other)
	{
		m_data = other.m_data;
		other.m_data = nullptr;
		termcount = other.termcount;
	}

	//Destructor
	~ordinal()
	{
		delete[] m_data;
	}

	//Assignment
	ordinal operator=(natural n)
	{
		if (n == 0)
		{
			_setToZero();
			return *this;
		}
		switch (termcount) {
		case 0:
			m_data = new term[1];
			m_data[0] = n;
			break;
		case 1:
			m_data[0] = n;
			break;
		default:
			delete[] m_data;
			m_data = new term[1];
			m_data[0] = n;
		}
		termcount = 1;
		return *this;
	}
	ordinal operator=(const ordinal& other)
	{
		if (m_data != other.m_data)
		{
			termcount = other.termcount;
			delete[] m_data;
			if (other.m_data) //RHS != 0
			{
				m_data = new term[termcount];
				for (size_t i = 0; i < termcount; i++)
				{
					m_data[i] = other.m_data[i];
				}
			}
			else
			{
				m_data = nullptr;
			}
		}
		return *this;
	}
	ordinal operator=(ordinal&& other)
	{
		if (m_data != other.m_data)
		{
			termcount = other.termcount;
			delete[] m_data;
			m_data = other.m_data;
			other.m_data = nullptr;
		}
		return *this;
	}

	//Order
	bool operator==(natural n)
	{
		switch(termcount)
		{
		case 0:
			if (n == 0)
				return true;
			return false;
			break;
		case 1:
			if (m_data[0] == n)
				return true;
			return false;
			break;
		default:
			return false;
		}
	}
	bool operator!=(natural n) { return !(*this == n); }
	bool operator<(natural n)
	{
		switch (termcount)
		{
		case 0:
			if (n == 0)
				return false;
			return true;
			break;
		case 1:
			if (m_data[0] < n)
				return true;
			return false;
			break;
		default:
			return false;
		}
	}
	bool operator>(natural n)
	{
		switch (termcount)
		{
		case 0:
			return false;
			break;
		default:
			return m_data[0] > n;
		}
	}

	bool operator==(const ordinal& other)
	{
		if (termcount != other.termcount)
			return false;
		for (size_t i = 0; i < termcount; i++)
		{
			if (m_data[i] != other.m_data[i])
				return false;
		}
		return true;
	}
	bool operator!=(const ordinal& other) { return !(*this == other); }
	bool operator<(const ordinal& other)
	{
		size_t minterms = std::min(termcount, other.termcount);
		for (size_t i = 0; i < minterms; i++)
		{
			if (m_data[i] < other.m_data[i])
				return true;
		}
		if (termcount < other.termcount)
			return true;
		return false;
	}
	bool operator>(const ordinal& other)
	{
		size_t minterms = std::min(termcount, other.termcount);
		for (size_t i = 0; i < minterms; i++)
		{
			if (m_data[i] > other.m_data[i])
				return true;
		}
		if (termcount > other.termcount)
			return true;
		return false;
	}

	//Arithmetic
	ordinal operator+=(natural n)
	{
		_addnatural(n);
		return *this;
	}
	ordinal operator+=(const ordinal& other)
	{
		_add(other);
		return *this;
	}
	ordinal operator*=(natural n)
	{
		_mulnatural(n);
		return *this;
	}
	ordinal operator*=(const ordinal& other)
	{
		_mul(other);
		return *this;
	}
	ordinal operator^=(natural n)
	{
		_expnatural(n);
		return *this;
	}
	ordinal operator^=(const ordinal& other)
	{
		_exp(other);
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const ordinal& ord);

	friend ordinal omegaExp(natural n);
	friend ordinal omegaExp(const ordinal& other);
	friend ordinal omegaExp(ordinal&& other);

	friend bool isNatural(const ordinal& ord);
};

//Equality
bool operator==(ordinal::natural left, const ordinal& right)
{
	ordinal lhs = left;
	return lhs == right;
}
bool operator==(const ordinal& left, ordinal::natural right)
{
	ordinal lhs = left;
	return lhs == right;
}
bool operator==(const ordinal& left, const ordinal& right)
{
	ordinal lhs = left;
	return lhs == right;
}
bool operator!=(ordinal::natural left, const ordinal& right)
{
	return !(left == right);
}
bool operator!=(const ordinal& left, ordinal::natural right)
{
	return !(left == right);
}
bool operator!=(const ordinal& left, const ordinal& right)
{
	return !(left == right);
}

//Order
bool operator<(ordinal::natural left, const ordinal& right)
{
	ordinal lhs = left;
	return lhs < right;
}
bool operator<(const ordinal& left, ordinal::natural right)
{
	ordinal lhs = left;
	return lhs < right;
}
bool operator<(const ordinal& left, const ordinal& right)
{
	ordinal lhs = left;
	return lhs < right;
}
bool operator>(ordinal::natural left, const ordinal& right)
{
	ordinal lhs = left;
	return lhs > right;
}
bool operator>(const ordinal& left, ordinal::natural right)
{
	ordinal lhs = left;
	return lhs > right;
}
bool operator>(const ordinal& left, const ordinal& right)
{
	ordinal lhs = left;
	return lhs > right;
}

//Addition
ordinal operator+(const ordinal& left, const ordinal& right)
{
	ordinal res = left;
	res += right;
	return res;
}
ordinal operator+(ordinal::natural left, const ordinal& right)
{
	ordinal res = left;
	res += right;
	return res;
}
ordinal operator+(const ordinal& left, ordinal::natural right)
{
	ordinal res = left;
	res += right;
	return res;
}

//Multiplication
ordinal operator*(const ordinal& left, const ordinal& right)
{
	ordinal res = left;
	res *= right;
	return res;
}
ordinal operator*(ordinal::natural left, const ordinal& right)
{
	ordinal res = left;
	res *= right;
	return res;
}
ordinal operator*(const ordinal& left, ordinal::natural right)
{
	ordinal res = left;
	res *= right;
	return res;
}

//Exponentiation
ordinal operator^(const ordinal& left, ordinal::natural right)
{
	ordinal res = left;
	res ^= right;
	return res;
}
ordinal operator^(ordinal::natural left, const ordinal& right)
{
	ordinal res = left;
	res ^= right;
	return res;
}
ordinal operator^(const ordinal& left, const ordinal& right)
{
	ordinal res = left;
	res ^= right;
	return res;
}

std::ostream& operator<<(std::ostream& os, const ordinal& ord)
{
	//If zero
	if (ord.termcount == 0)
	{
		os << 0;
		return os;
	}

	//Leading term
	if (isNatural(ord))
		os << ord.m_data[0].coeff;
	else if (ord.m_data[0].getExponent() == 1)
	{
		if (ord.m_data[0].coeff != 1)
			os << ord.m_data[0].coeff;
		os << 'w';
	}
	else
	{
		if (ord.m_data[0].coeff != 1)
			os << ord.m_data[0].coeff;

		if(isNatural(ord.m_data[0].getExponent()))
			os << "w^" << ord.m_data[0].getExponent();
		else
			os << "w^(" << ord.m_data[0].getExponent() << ')';
	}

	for (size_t i = 1; i < ord.termcount; i++)
	{
		os << " + ";

		if (ord.m_data[i].getExponent() == 0)
			os << ord.m_data[i].coeff;
		else if (ord.m_data[i].getExponent() == 1)
		{
			if (ord.m_data[i].coeff != 1)
				os << ord.m_data[i].coeff;
			os << 'w';
		}
		else
		{
			if (ord.m_data[i].coeff != 1)
				os << ord.m_data[i].coeff;

			if (isNatural(ord.m_data[i].getExponent()))
				os << "w^" << ord.m_data[i].getExponent();
			else
				os << "w^(" << ord.m_data[i].getExponent() << ')';
		}
	}
	return os;
}

ordinal omegaExp(ordinal::natural n)
{
	ordinal res = 1;
	*(res.m_data[0].exponent) = n;
	return res;
}

ordinal omegaExp(const ordinal& other)
{
	ordinal res = 1;
	*(res.m_data[0].exponent) = other;
	return res;
}

ordinal omegaExp(ordinal&& other)
{
	ordinal res = 1;
	*(res.m_data[0].exponent) = other;
	return res;
}

bool isNatural(const ordinal& ord)
{
	return ord._isNatural();
}
