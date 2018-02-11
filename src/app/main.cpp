#include <iostream>
#include <vector>
#include <chrono>

#include <CL/cl.hpp>

#include "neural_network/NeuralNodeGroupLinearExcitation.h"
#include "neural_network/NeuralEdgeGroup.h"
#include "utility/cl/ClSystem.h"
#include "utility/random/RandomNumberGenerator.h"
#include "utility/logging.h"

int main()
{
	if (!ClSystem::getInstance())
	{
		return EXIT_FAILURE;
	}

	const int NODE_COUNT = 1000;
	const int REPETITIONS = 2000;
	RandomNumberGenerator rng(1);

	std::shared_ptr<NeuralNodeGroup> sourceNodeGroup = std::make_shared<NeuralNodeGroupLinearExcitation>(NODE_COUNT);
	{
		std::vector<float> activationLevels(NODE_COUNT);
		for (int i = 0; i < NODE_COUNT; i++)
		{
			activationLevels[i] = rng.getFloat();
		}
		sourceNodeGroup->setExcitationLevels(activationLevels);
	}

	for (const float v: sourceNodeGroup->getExcitationLevels())
	{
		std::cout << v << " ";
	}
	std::cout << std::endl;

	std::shared_ptr<NeuralNodeGroup> targetNodeGroup = std::make_shared<NeuralNodeGroupLinearExcitation>(NODE_COUNT);

	NeuralEdgeGroup edgeGroup(sourceNodeGroup, targetNodeGroup);
	
	{
		Matrix<float> weights(NODE_COUNT, NODE_COUNT);
		for (int j = 0; j < NODE_COUNT; j++)
		{
			for (int i = 0; i < NODE_COUNT; i++)
			{
				weights.setValue(i, j, rng.getFloat());
			}
		}
		edgeGroup.setWeights(weights);
	}

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < REPETITIONS; i++)
	{
		sourceNodeGroup->update();
		edgeGroup.update();
	}
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();


	for (const float v : targetNodeGroup->getExcitationLevels())
	{
		std::cout << v << " ";
	}
	std::cout << std::endl;

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	std::cout << "duration: " << (float(duration) / REPETITIONS) << std::endl;

	return EXIT_SUCCESS;
}
