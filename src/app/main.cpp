#include <vector>

#include <CL/cl.hpp>

#include "neural_network/NeuralNodeGroupLinearExcitation.h"
#include "neural_network/NeuralNodeGroupStepExcitation.h"
#include "neural_network/NeuralEdgeGroup.h"
#include "utility/cl/ClSystem.h"
#include "utility/logging.h"

#include "NeuralNetworkGenotype.h"
#include "NeuroEvolutionEnvironment.h"



int main()
{
	const int populationSize = 10;
	const int offspringSize = 10;
	const int generationCount = 2;

	if (!ClSystem::getInstance())
	{
		return EXIT_FAILURE;
	}

	NeuroEvolutionEnvironment evolutionEnvironment(populationSize, offspringSize, 0.1f);
	for (int i = 0; i < populationSize; i++)
	{
		evolutionEnvironment.addGenotype(createRandomNeuralNetworkGenotype(10));
	}

	for (int i = 0; i < generationCount; i++)
	{
		evolutionEnvironment.processGeneration();
		LOG_INFO("Generations processed: " + std::to_string(evolutionEnvironment.getGenerationCount()));
	}

	LOG_INFO("Done. Highest score: " + std::to_string(evolutionEnvironment.getHighestFitness()));
	NeuralNetworkGenotype genotype = evolutionEnvironment.getGenotypeWithHighestFittness();
	runPingEvaluation(genotype, true);

	return EXIT_SUCCESS;
}
