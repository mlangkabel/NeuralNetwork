#ifndef NEURO_EVOLUTION_ENVIRONMENT_H
#define NEURO_EVOLUTION_ENVIRONMENT_H

#include "evolution/EvolutionEnvironment.h"
#include "NeuralNetworkGenotype.h"

class NeuroEvolutionEnvironment: public EvolutionEnvironment<NeuralNetworkGenotype>
{
public:
	NeuroEvolutionEnvironment(int populatioSize, int offspringSize, float crossoverProbability);

private:
	NeuralNetworkGenotype crossover(const NeuralNetworkGenotype& genotype1, const NeuralNetworkGenotype& genotype2) override;
	NeuralNetworkGenotype mutate(const NeuralNetworkGenotype& genotype) override;
	float evaluate(const NeuralNetworkGenotype& genotype) override;
};

#endif // NEURO_EVOLUTION_ENVIRONMENT_H
