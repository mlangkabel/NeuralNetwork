#ifndef NEURO_EVOLUTION_ENVIRONMENT_H
#define NEURO_EVOLUTION_ENVIRONMENT_H

#include "evolution/EvolutionEnvironment.h"
#include "NeuralNetworkSpecification.h"

class NeuroEvolutionEnvironment: public EvolutionEnvironment<NeuralNetworkSpecification>
{
public:
	NeuroEvolutionEnvironment(int populatioSize, int offspringSize, float crossoverProbability);

private:
	NeuralNetworkSpecification crossover(const NeuralNetworkSpecification& genotype1, const NeuralNetworkSpecification& genotype2) override;
	NeuralNetworkSpecification mutate(const NeuralNetworkSpecification& genotype) override;
	float evaluate(const NeuralNetworkSpecification& genotype) override;
};

#endif // NEURO_EVOLUTION_ENVIRONMENT_H
