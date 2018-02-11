#ifndef RANDOM_NUMBER_GENERATOR_H
#define RANDOM_NUMBER_GENERATOR_H

class RandomNumberGenerator 
{
public:
	RandomNumberGenerator(unsigned int seed);

	void setSeed(unsigned int seed);
	void reset();

	int getInt(int min, int max);
	float getFloat(float min = 0.0f, float max = 1.0f);
	bool getBool();

private:
	int getNext();
	unsigned int m_seed;

	unsigned int m_value;
	unsigned int m_a;
	unsigned int m_b;
	unsigned int m_m;
};

#endif