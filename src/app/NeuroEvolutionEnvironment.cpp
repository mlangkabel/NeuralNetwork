#include "NeuroEvolutionEnvironment.h"
#include "utility/random/WeightedRandomGenerator.h"
#include "utility/utilityRandom.h"

NeuroEvolutionEnvironment::NeuroEvolutionEnvironment(int populatioSize, int offspringSize, float crossoverProbability)
	: EvolutionEnvironment(populatioSize, offspringSize, crossoverProbability)
{
}

NeuralNetworkGenotype NeuroEvolutionEnvironment::crossover(const NeuralNetworkGenotype& genotype1, const NeuralNetworkGenotype& genotype2)
{
	return genotype1;
}

NeuralNetworkGenotype NeuroEvolutionEnvironment::mutate(const NeuralNetworkGenotype& genotype)
{
	NeuralNetworkGenotype mutatedGenotype = genotype;

	WeightedRandomGenerator<Matrix<float>*> wrng(utility::getRandomInt(0, 10000));
	wrng.addResult(&mutatedGenotype.inputToHiddenNodeWeights, mutatedGenotype.inputToHiddenNodeWeights.getElementCount());
	wrng.addResult(&mutatedGenotype.hiddenToHiddenNodeWeights, mutatedGenotype.hiddenToHiddenNodeWeights.getElementCount());
	wrng.addResult(&mutatedGenotype.hiddenToOutputNodeWeights, mutatedGenotype.hiddenToOutputNodeWeights.getElementCount());

	Matrix<float>* weights = wrng.getResult();

	const int indexToMutate = utility::getRandomInt(0, weights->getElementCount() - 1);
	(*weights)[indexToMutate] += utility::getRandomFloat(-0.1f, 0.1f);

	return mutatedGenotype;
}

float NeuroEvolutionEnvironment::evaluate(const NeuralNetworkGenotype& genotype)
{
	const int repetitions = 10;
	float score = 0.0f;
	for (int i = 0; i < repetitions; i++)
	{
		score += runPingEvaluation(genotype, false);
	}
	return score / repetitions;
}
