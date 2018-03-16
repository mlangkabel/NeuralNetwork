#ifndef EVOLUTION_ENVIRONMENT_H
#define EVOLUTION_ENVIRONMENT_H

#include "evolution/EvaluatedGenotype.h"
#include "utility/random/WeightedRandomGenerator.h"

#include <vector>
#include <memory>
#include <functional>
#include <limits>
#include <thread>

template <typename GenotypeType>
class EvolutionEnvironment
{
public:
	EvolutionEnvironment(int populationSize, int offspringSize, float crossoverProbability);
	virtual ~EvolutionEnvironment() = default;

	void addGenotype(GenotypeType genotype);

	float getHighestFitness() const;
	GenotypeType getGenotypeWithHighestFittness() const;
	std::vector<GenotypeType> getPopulation() const;
	int getGenerationCount() const;

	void processGeneration();

protected:
	RandomNumberGenerator m_randomNumberGenerator;

private:
	virtual GenotypeType crossover(const GenotypeType& genotype1, const GenotypeType& genotype2) = 0;
	virtual GenotypeType mutate(const GenotypeType& genotype) = 0;
	virtual float evaluate(const GenotypeType& genotype) = 0;
	float getMinFitness() const;

	const int m_populationSize;
	const int m_offspringSize;
	const float m_crossoverProbability;
	int m_generationCount;

	std::vector<EvaluatedGenotype<GenotypeType>> m_population;
};

template <typename GenotypeType>
EvolutionEnvironment<GenotypeType>::EvolutionEnvironment(int populationSize, int offspringSize, float crossoverProbability)
	: m_randomNumberGenerator(0)
	, m_populationSize(populationSize)
	, m_offspringSize(offspringSize)
	, m_crossoverProbability(crossoverProbability)
	, m_generationCount(0)
{
}

template <typename GenotypeType>
void EvolutionEnvironment<GenotypeType>::addGenotype(GenotypeType genotype)
{
	m_population.push_back(EvaluatedGenotype<GenotypeType>(genotype, evaluate(genotype)));
}

template <typename GenotypeType>
float EvolutionEnvironment<GenotypeType>::getHighestFitness() const
{
	float maxFitness = -std::numeric_limits<float>::infinity();

	for(int i = 0; i < m_population.size(); i++)
	{
		maxFitness = std::fmax(maxFitness, m_population[i].fitness);
	}

	return maxFitness;
}

template <typename GenotypeType>
GenotypeType EvolutionEnvironment<GenotypeType>::getGenotypeWithHighestFittness() const
{
	float maxFitness = -std::numeric_limits<float>::infinity();
	GenotypeType genotype;

	for(int i = 0; i < m_population.size(); i++)
	{
		if(m_population[i].fitness > maxFitness)
		{
			maxFitness = m_population[i].fitness;
			genotype = m_population[i].genotype;
		}
	}

	return genotype;
}

template <typename GenotypeType>
std::vector<GenotypeType> EvolutionEnvironment<GenotypeType>::getPopulation() const
{
	std::vector<GenotypeType> population;

	for(int i = 0; i < m_population.size(); i++)
	{
		population.push_back(m_population[i].genotype);
	}

	return population;
}

template <typename GenotypeType>
int EvolutionEnvironment<GenotypeType>::getGenerationCount() const
{
	return m_generationCount;
}

template <typename GenotypeType>
void EvolutionEnvironment<GenotypeType>::processGeneration()
{
	m_generationCount++;

	WeightedRandomGenerator<GenotypeType> potentialParents(m_randomNumberGenerator.getInt(0, 1000000));

	{
		const float minFitness = getMinFitness();
		for (const EvaluatedGenotype<GenotypeType>& candidate : m_population)
		{
			const float fitness = candidate.fitness - minFitness + 0.1f;
			potentialParents.addResult(candidate.genotype, fitness * fitness);
		}
	}

	for(int i = 0; i < m_offspringSize; i++)
	{
		if (m_randomNumberGenerator.getFloat() <= m_crossoverProbability)
		{
			addGenotype(crossover(potentialParents.getResult(), potentialParents.getResult()));
		}
		else
		{
			addGenotype(mutate(potentialParents.getResult()));
		}
	}

	{
		const float minFitness = getMinFitness();

		WeightedRandomGenerator<EvaluatedGenotype<GenotypeType>> potentialNextGeneration(m_randomNumberGenerator.getInt(0, 1000000));
		for (const EvaluatedGenotype<GenotypeType>& candidate : m_population)
		{
			const float fitness = candidate.fitness - minFitness + 0.1f;
			potentialNextGeneration.addResult(candidate, fitness * fitness);
		}

		m_population = potentialNextGeneration.getResults(m_populationSize, false);
	}
}

template <typename GenotypeType>
float EvolutionEnvironment<GenotypeType>::getMinFitness() const
{
	float minFitness = std::numeric_limits<float>::infinity();
	for(int i = 0; i < m_population.size(); i++)
	{
		minFitness = std::fmin(minFitness, m_population[i].fitness);
	}
	return minFitness;
}

#endif // EVOLUTION_ENVIRONMENT_H
