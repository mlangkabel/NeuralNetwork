#include "utility/utilityString.h"
#include "utility/random/RandomNumberGenerator.h"

namespace
{
	RandomNumberGenerator rng(0);
}

namespace utility
{
	int getRandomInt(int min, int max)
	{
		return rng.getInt(min, max);
	}

	float getRandomFloat(float min = 0.0f, float max = 1.0f)
	{
		return rng.getFloat(min, max);
	}

	bool getRandomBool()
	{
		return rng.getBool();
	}
}
