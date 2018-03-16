#include <vector>

#include <CL/cl.hpp>

#include "neural_network/gpu/NeuralNodeGroupLinearExcitationGpu.h"
#include "neural_network/gpu/NeuralNodeGroupStepExcitationGpu.h"
#include "neural_network/gpu/NeuralEdgeGroupGpu.h"
#include "utility/cl/ClSystem.h"
#include "utility/logging.h"

#include "NeuralNetworkGenotype.h"
#include "NeuroEvolutionEnvironment.h"

int main()
{
	const int populationSize = 50;
	const int offspringSize = 10;

	if (!ClSystem::getInstance())
	{
		return EXIT_FAILURE;
	}

	NeuroEvolutionEnvironment evolutionEnvironment(populationSize, offspringSize, 0.3f);
	for (int i = 0; i < populationSize; i++)
	{
		evolutionEnvironment.addGenotype(createRandomNeuralNetworkGenotype(20));
	}

	bool running = true;

	std::thread thread([&]() {
		while (running)
		{
			evolutionEnvironment.processGeneration();
			LOG_INFO(
				"Generations processed: " + std::to_string(evolutionEnvironment.getGenerationCount()) +
				" Highest Fitness: " + std::to_string(evolutionEnvironment.getHighestFitness())
			);
		}
	});

	system("pause");

	running = false;

	thread.join();

	LOG_INFO("Done. Highest score: " + std::to_string(evolutionEnvironment.getHighestFitness()));
	NeuralNetworkGenotype genotype = evolutionEnvironment.getGenotypeWithHighestFittness();
	runPingEvaluation(genotype, 0, true);
	runPingEvaluation(genotype, 1, true);
	runPingEvaluation(genotype, 2, true);
	runPingEvaluation(genotype, 3, true);
	runPingEvaluation(genotype, 4, true);

	return EXIT_SUCCESS;
}
