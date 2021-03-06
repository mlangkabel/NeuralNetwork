#ifndef NEURO_EVOLUTION_ENVIRONMENT_H
#define NEURO_EVOLUTION_ENVIRONMENT_H

#include "evolution/EvolutionEnvironment.h"
#include "neural_network/NeuralNetworkConfiguration.h"

class NeuroEvolutionEnvironment: public EvolutionEnvironment<NeuralNetworkConfiguration>
{
public:
	NeuroEvolutionEnvironment(int populatioSize, int offspringSize, float crossoverProbability);

private:
	NeuralNetworkConfiguration crossover(const NeuralNetworkConfiguration& genotype1, const NeuralNetworkConfiguration& genotype2) override;
	NeuralNetworkConfiguration mutate(const NeuralNetworkConfiguration& genotype) override;
	float evaluate(const NeuralNetworkConfiguration& genotype) override;
};

NeuralNetworkConfiguration createRandomNeuralNetworkConfiguration(int hiddenNodeAmount);

float runPingEvaluation(NeuralNetworkConfiguration genotype, bool verbose);
float runPingEvaluation(NeuralNetworkConfiguration genotype, const int nunmberOfPings, bool verbose);
std::vector<float>& addNoise(std::vector<float>& vec);
std::vector<float> addNoise(const std::vector<float>& vec);

#endif // NEURO_EVOLUTION_ENVIRONMENT_H
