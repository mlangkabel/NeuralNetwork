#ifndef OPTIONAL_H
#define OPTIONAL_H

template <typename T>
class Optional
{
public:
	Optional();
	Optional(const T& data);
	Optional(const Optional<T>& rhs);

	Optional<T>& operator=(const T& data);
	Optional<T>& operator=(const Optional<T>& rhs);

	bool operator==(const T& rhs);
	bool operator==(const Optional<T>& rhs);

	explicit  operator bool() const noexcept { return m_valid; }

	const T& get() const;
	T& get();

private:
	bool m_valid;
	T m_data;
};

template <typename T>
Optional<T>::Optional()
	: m_valid(false)
{
}

template <typename T>
Optional<T>::Optional(const T& data)
	: m_data(data)
	, m_valid(true)
{
}

template <typename T>
Optional<T>::Optional(const Optional<T>& rhs)
{
	if (this != &rhs)
	{
		m_data = rhs.m_data;
		m_valid = rhs.m_valid;
	}
}

template <typename T>
Optional<T>& Optional<T>::operator=(const T& data)
{
	m_data = m_data;
	m_valid = true;
	return *this;
}

template <typename T>
Optional<T>& Optional<T>::operator=(const Optional<T>& rhs)
{
	if (this != &rhs)
	{
		m_data = rhs.m_data;
		m_valid = rhs.m_valid;
	}
	return *this;
}

template <typename T>
bool Optional<T>::operator==(const T& rhs)
{
	return m_valid && m_data == rhs;
}

template <typename T>
bool Optional<T>::operator==(const Optional<T>& rhs)
{
	return (m_valid == rhs.m_valid && (!m_valid || (m_data == rhs.m_data)));
}

template <typename T>
const T& Optional<T>::get() const
{
	return m_data;
}

template <typename T>
T& Optional<T>::get()
{
	return m_data;
}

#endif // OPTIONAL_H
