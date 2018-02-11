#ifndef MATRIX_H
#define MATRIX_H

template <typename T>
class Matrix
{
public:
	Matrix(const int width, const int height);
	Matrix(const Matrix<T>& rhs);
	~Matrix();

	Matrix<T>& operator=(const Matrix<T>& rhs);

	int getWidth() const;
	int getHeight() const;
	int getElementCount() const;

	T& operator[](size_t pos);

	void setValue(const int x, const int y, const T value);
	T getValue(const int x, const int y) const;

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
	if (this = &rhs)
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
void Matrix<T>::setValue(const int x, const int y, const T value)
{
	m_values[x + y * m_width] = value;
}

template <typename T>
T Matrix<T>::getValue(const int x, const int y) const
{
	return m_values[x + y * m_width];
}

#endif // MATRIX_H
