#ifndef UTILITY_RANDOM_H
#define UTILITY_RANDOM_H

namespace utility
{
	int getRandomInt(int min, int max);
	float getRandomFloat(float min = 0.0f, float max = 1.0f);
	bool getRandomBool();
}

#endif // UTILITY_RANDOM_H
