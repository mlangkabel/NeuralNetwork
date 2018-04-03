#include "NeuroEvolutionEnvironment.h"
#include "utility/random/WeightedRandomGenerator.h"
#include "utility/utilityRandom.h"

NeuroEvolutionEnvironment::NeuroEvolutionEnvironment(int populatioSize, int offspringSize, float crossoverProbability)
	: EvolutionEnvironment(populatioSize, offspringSize, crossoverProbability)
{
}

NeuralNetworkSpecification NeuroEvolutionEnvironment::crossover(const NeuralNetworkSpecification& genotype1, const NeuralNetworkSpecification& genotype2)
{
	const float minInfluence = 0.2f;

	NeuralNetworkSpecification mergedGenotype = genotype1;

	for (int i = 0; i < mergedGenotype.inputToHiddenNodeWeights.getElementCount(); i++)
	{
		const float preferabilityGenotype1 = utility::getRandomFloat(minInfluence, 1.0f - minInfluence);
		const float preferabilityGenotype2 = 1.0f - preferabilityGenotype1;

		mergedGenotype.inputToHiddenNodeWeights[i] =
			genotype1.inputToHiddenNodeWeights[i] * preferabilityGenotype1 +
			genotype2.inputToHiddenNodeWeights[i] * preferabilityGenotype2;
	}

	for (int i = 0; i < mergedGenotype.hiddenToHiddenNodeWeights.getElementCount(); i++)
	{
		const float preferabilityGenotype1 = utility::getRandomFloat(minInfluence, 1.0f - minInfluence);
		const float preferabilityGenotype2 = 1.0f - preferabilityGenotype1;

		mergedGenotype.hiddenToHiddenNodeWeights[i] =
			genotype1.hiddenToHiddenNodeWeights[i] * preferabilityGenotype1 +
			genotype2.hiddenToHiddenNodeWeights[i] * preferabilityGenotype2;
	}

	for (int i = 0; i < mergedGenotype.hiddenToOutputNodeWeights.getElementCount(); i++)
	{
		const float preferabilityGenotype1 = utility::getRandomFloat(minInfluence, 1.0f - minInfluence);
		const float preferabilityGenotype2 = 1.0f - preferabilityGenotype1;

		mergedGenotype.hiddenToOutputNodeWeights[i] =
			genotype1.hiddenToOutputNodeWeights[i] * preferabilityGenotype1 +
			genotype2.hiddenToOutputNodeWeights[i] * preferabilityGenotype2;
	}

	return mergedGenotype;
}

NeuralNetworkSpecification NeuroEvolutionEnvironment::mutate(const NeuralNetworkSpecification& genotype)
{
	WeightedRandomGenerator<int> countRng(utility::getRandomInt(0, 10000));
	countRng.addResult(1, 1.0f / 2.0f);
	countRng.addResult(2, 1.0f / 4.0f);
	countRng.addResult(3, 1.0f / 8.0f);
	countRng.addResult(4, 1.0f / 16.0f);
	countRng.addResult(5, 1.0f / 32.0f);

	const int mutationCount = countRng.getResult();
	NeuralNetworkSpecification mutatedGenotype = genotype;

	WeightedRandomGenerator<Matrix<float>*> weightsRng(utility::getRandomInt(0, 10000));
	weightsRng.addResult(&mutatedGenotype.inputToHiddenNodeWeights, mutatedGenotype.inputToHiddenNodeWeights.getElementCount());
	weightsRng.addResult(&mutatedGenotype.hiddenToHiddenNodeWeights, mutatedGenotype.hiddenToHiddenNodeWeights.getElementCount());
	weightsRng.addResult(&mutatedGenotype.hiddenToOutputNodeWeights, mutatedGenotype.hiddenToOutputNodeWeights.getElementCount());

	for (int i = 0; i < mutationCount; i++)
	{
		Matrix<float>* weights = weightsRng.getResult();

		const int indexToMutate = utility::getRandomInt(0, weights->getElementCount() - 1);
		(*weights)[indexToMutate] += utility::getRandomFloat(-0.1f, 0.1f);
	}

	return mutatedGenotype;
}

float NeuroEvolutionEnvironment::evaluate(const NeuralNetworkSpecification& genotype)
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
