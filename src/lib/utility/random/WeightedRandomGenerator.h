#ifndef WEIGHTED_RANDOM_GENERATOR_H
#define WEIGHTED_RANDOM_GENERATOR_H

#include <iostream>
#include <vector>

#include "utility/random/RandomNumberGenerator.h"

template <typename T>
class WeightedRandomGenerator 
{
public:
	WeightedRandomGenerator(unsigned int seed);

	void setSeed(unsigned int seed);
	void reset();

	void addResult(T value, float weight);
	T getResult();
	std::vector<T> getResults(int count, bool allowDuplicates);

private:
	RandomNumberGenerator m_randomGenerator;
	std::vector<std::pair<T, float>> m_results;
};

template <typename T>
WeightedRandomGenerator<T>::WeightedRandomGenerator(unsigned int seed):
	m_randomGenerator(seed)
{
}
		
template <typename T>
void WeightedRandomGenerator<T>::setSeed(unsigned int seed)
{
	m_randomGenerator.setSeed(seed);
}

template <typename T>
void WeightedRandomGenerator<T>::reset()
{
	m_randomGenerator.reset();
}
		
template <typename T>
void WeightedRandomGenerator<T>::addResult(T value, float weight)
{
	m_results.push_back(std::make_pair(value, weight));
}

template <typename T>
T WeightedRandomGenerator<T>::getResult()
{
	float summedWeights = 0;
	for (int i = 0; i < m_results.size(); i++)
		summedWeights += m_results[i].second;
		
	float random = m_randomGenerator.getFloat(0.0f, summedWeights);

	int index = 0;
	float weights = 0.0f;
	for (index = 0; index < m_results.size(); index++)
	{
		weights += m_results[index].second;
		if (weights > random)
			break;
	}

	return m_results[index].first;
}

template <typename T>
std::vector<T> WeightedRandomGenerator<T>::getResults(int count, bool allowDuplicates)
{
	if (!allowDuplicates && count > m_results.size())
	{
		std::cout << "WeightedRandomGenerator does not have enough results to not allow duplicates." << std::endl;
		allowDuplicates = true;
	}

	std::vector<std::pair<T, float>> potentialResults = m_results;
	std::vector<T> results;

	float summedWeights = 0;
	for (int i = 0; i < potentialResults.size(); i++)
	{
		summedWeights += potentialResults[i].second;
	}

	for (int i = 0; i < count; i++)
	{
		float random = m_randomGenerator.getFloat(0.0f, summedWeights);

		int index = 0;
		float weights = 0.0f;
		for (index = 0; index < potentialResults.size() - 1; index++)
		{
			weights += potentialResults[index].second;
			if (weights > random)
				break;
		}

		results.push_back(potentialResults[index].first);
		summedWeights -= potentialResults[index].second;
		potentialResults.erase(potentialResults.begin() + index);
	}

	return results;
}
	
#endif // WEIGHTED_RANDOM_GENERATOR_H
