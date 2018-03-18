#include "NeuroEvolutionEnvironment.h"
#include "utility/random/WeightedRandomGenerator.h"
#include "utility/utilityRandom.h"

NeuroEvolutionEnvironment::NeuroEvolutionEnvironment(int populatioSize, int offspringSize, float crossoverProbability)
	: EvolutionEnvironment(populatioSize, offspringSize, crossoverProbability)
{
}

NeuralNetworkGenotype NeuroEvolutionEnvironment::crossover(const NeuralNetworkGenotype& genotype1, const NeuralNetworkGenotype& genotype2)
{
	const float preferabilityGenotype1 = utility::getRandomFloat(0.2f, 0.8f);
	const float preferabilityGenotype2 = 1.0f - preferabilityGenotype1;
	
	NeuralNetworkGenotype mergedGenotype = genotype1;

	for (int i = 0; i < mergedGenotype.inputToHiddenNodeWeights.getElementCount(); i++)
	{
		mergedGenotype.inputToHiddenNodeWeights[i] =
			genotype1.inputToHiddenNodeWeights[i] * preferabilityGenotype1 +
			genotype2.inputToHiddenNodeWeights[i] * preferabilityGenotype2;
	}

	for (int i = 0; i < mergedGenotype.hiddenToHiddenNodeWeights.getElementCount(); i++)
	{
		mergedGenotype.hiddenToHiddenNodeWeights[i] =
			genotype1.hiddenToHiddenNodeWeights[i] * preferabilityGenotype1 +
			genotype2.hiddenToHiddenNodeWeights[i] * preferabilityGenotype2;
	}

	for (int i = 0; i < mergedGenotype.hiddenToOutputNodeWeights.getElementCount(); i++)
	{
		mergedGenotype.hiddenToOutputNodeWeights[i] =
			genotype1.hiddenToOutputNodeWeights[i] * preferabilityGenotype1 +
			genotype2.hiddenToOutputNodeWeights[i] * preferabilityGenotype2;
	}

	return mergedGenotype;
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
	const int repetitions = 3;
	float score = 0.0f;
	for (int i = 0; i < repetitions; i++)
	{
		score += runPingEvaluation(genotype, 0, false);
		score += runPingEvaluation(genotype, 1, false);
		score += runPingEvaluation(genotype, 2, false);
		score += runPingEvaluation(genotype, 3, false);
		score += runPingEvaluation(genotype, 4, false);
	}
	return score / repetitions / 5.0f;
}
