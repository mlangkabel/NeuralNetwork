#ifndef MATRIX_H
#define MATRIX_H

#include "utility/logging.h"
#include "utility/utilityString.h"

template <typename T>
class Matrix
{
public:
	Matrix(const int width, const int height);
	Matrix(const Matrix<T>& rhs);
	~Matrix();

	Matrix<T>& operator=(const Matrix<T>& rhs);
	bool operator==(const Matrix<T>& rhs);

	int getWidth() const;
	int getHeight() const;
	int getElementCount() const;

	T& operator[](size_t pos);

	void setValues(const T value);

	void setValue(const int x, const int y, const T value);
	T getValue(const int x, const int y) const;

	bool setValuesFromString(const std::string& valuesString);
	std::string getValuesAsString() const;

private:
	const int m_width;
	const int m_height;
	T* m_values;
};

template <typename T>
Matrix<T>::Matrix(const int width, const int height)
	: m_width(width)
	, m_height(height)
{
	m_values = new T[m_width * m_height];
	std::fill(m_values, m_values + sizeof(m_values), 0);
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& rhs)
	: m_width(rhs.m_width)
	, m_height(rhs.m_height)
{
	m_values = new T[m_width * m_height];
	for (int i = 0; i < m_width * m_height; i++)
	{
		m_values[i] = rhs.m_values[i];
	}
}

template <typename T>
Matrix<T>::~Matrix()
{
	delete[] m_values;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& rhs)
{
	if (this == &rhs)
	{
		return *this;
	}
	delete[] m_values;

	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_values = new T[m_width * m_height];
	for (int i = 0; i < m_width * m_height; i++)
	{
		m_values[i] = rhs.m_values[i];
	}
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T>& rhs)
{
	if (this == &rhs)
	{
		return true;
	}

	if (getWidth() != rhs.getWidth() || getHeight() != rhs.getHeight())
	{
		return false;
	}

	for (int i = 0; i < getWidth() * getHeight(); i++)
	{
		if (m_values[i] != rhs.m_values[i])
		{
			return false;
		}
	}
	return true;
}

template <typename T>
int Matrix<T>::getWidth() const
{
	return m_width;
}

template <typename T>
int Matrix<T>::getHeight() const
{
	return m_height;
}

template <typename T>
int Matrix<T>::getElementCount() const
{
	return m_width * m_height;
}

template <typename T>
T& Matrix<T>::operator[](size_t pos)
{
	return m_values[pos];
}

template <typename T>
void Matrix<T>::setValues(const T value)
{
	for (int i = 0; i < m_width * m_height; i++)
	{
		m_values[i] = value;
	}
}

template <typename T>
void Matrix<T>::setValue(const int x, const int y, const T value)
{
	m_values[x + y * m_width] = value;
}

template <typename T>
T Matrix<T>::getValue(const int x, const int y) const
{
	return m_values[x + y * m_width];
}

template <typename T>
bool Matrix<T>::setValuesFromString(const std::string& valuesString)
{
	const std::vector<std::string> rowStrings = utility::split(valuesString, ";");
	if (rowStrings.size() != getHeight())
	{
		LOG_ERROR(
			"Loading Matrix from string failed. Matrix has specified a height of " + 
			std::to_string(getHeight()) + " but string specifies " + 
			std::to_string(rowStrings.size()) + " rows."
		);
		setValues(T());
		return false;
	}

	for (int y = 0; y < rowStrings.size(); y++)
	{
		const std::vector<std::string> colStrings = utility::split(rowStrings[y], ",");
		if (colStrings.size() != getWidth())
		{
			LOG_ERROR(
				"Loading Matrix from string failed. Matrix has specified a width of " +
				std::to_string(getWidth()) + " but string specifies " +
				std::to_string(colStrings.size()) + " columns in row " + std::to_string(y) + " (0-based)."
			);
			setValues(T());
			return false;
		}
		for (int x = 0; x < colStrings.size(); x++)
		{
			setValue(x, y, utility::fromString<typename T>(colStrings[x]));
		}
	}
	return true;
}

template <typename T>
std::string Matrix<T>::getValuesAsString() const
{
	std::string ret;
	for (int y = 0; y < getHeight(); y++)
	{
		if (y != 0)
		{
			ret += ";";
		}
		for (int x = 0; x < getWidth(); x++)
		{
			if (x != 0)
			{
				ret += ",";
			}
			ret += utility::toString<typename T>(getValue(x, y));
		}
	}
	return ret;
}

#endif // MATRIX_H
