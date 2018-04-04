#ifndef NEURO_EVOLUTION_ENVIRONMENT_H
#define NEURO_EVOLUTION_ENVIRONMENT_H

#include "evolution/EvolutionEnvironment.h"
#include "NeuralNetworkConfiguration.h"

class NeuroEvolutionEnvironment: public EvolutionEnvironment<NeuralNetworkConfiguration>
{
public:
	NeuroEvolutionEnvironment(int populatioSize, int offspringSize, float crossoverProbability);

private:
	NeuralNetworkConfiguration crossover(const NeuralNetworkConfiguration& genotype1, const NeuralNetworkConfiguration& genotype2) override;
	NeuralNetworkConfiguration mutate(const NeuralNetworkConfiguration& genotype) override;
	float evaluate(const NeuralNetworkConfiguration& genotype) override;
};

#endif // NEURO_EVOLUTION_ENVIRONMENT_H
