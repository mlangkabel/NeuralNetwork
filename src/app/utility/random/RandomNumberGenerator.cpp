#include "RandomNumberGenerator.h"

RandomNumberGenerator::RandomNumberGenerator(unsigned int seed):
	m_seed(seed),
	m_value(seed),
	m_a(214013),
	m_b(2531011),
	m_m(2147483647)
{
}

void RandomNumberGenerator::setSeed(unsigned int seed)
{
	m_seed = seed;
	reset();
}

void RandomNumberGenerator::reset()
{
	m_value = m_seed;
}

int RandomNumberGenerator::getInt(int min, int max)
{
	return min + (getNext() % (max - min + 1));
}

float RandomNumberGenerator::getFloat(float min, float max)
{
	return min + float(getNext()) / m_m * (max - min);
}

bool RandomNumberGenerator::getBool()
{
	return (getFloat(0.0f, 1.0f) < 0.5f ? true : false);
}

int RandomNumberGenerator::getNext()
{
	return m_value = (m_a * m_value + m_b) % m_m;
}
