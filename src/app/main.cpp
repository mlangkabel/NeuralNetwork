#include <iostream>
#include <vector>
#include <chrono>

#include <CL/cl.hpp>

#include "neural_network/NeuralNodeGroup.h"
#include "neural_network/NeuralEdgeGroup.h"
#include "utility/random/RandomNumberGenerator.h"
#include "utility/logging.h"

int main()
{
	const int NODE_COUNT = 5000;
	const int REPETITIONS = 2000;
	RandomNumberGenerator rng(1);

	std::shared_ptr<NeuralNodeGroup> sourceNodeGroup = std::make_shared<NeuralNodeGroup>(NODE_COUNT);
	{
		std::vector<float> activationLevels(NODE_COUNT);
		for (int i = 0; i < NODE_COUNT; i++)
		{
			activationLevels[i] = rng.getFloat();
		}
		sourceNodeGroup->setActivationLevels(activationLevels);
	}

	for (const float v: sourceNodeGroup->getActivationLevels())
	{
		std::cout << v << " ";
	}
	std::cout << std::endl;

	std::shared_ptr<NeuralNodeGroup> targetNodeGroup = std::make_shared<NeuralNodeGroup>(NODE_COUNT);

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
		edgeGroup.update();
	}
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();


	for (const float v : targetNodeGroup->getActivationLevels())
	{
		std::cout << v << " ";
	}
	std::cout << std::endl;

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	std::cout << "duration: " << (float(duration) / REPETITIONS) << std::endl;
	return 0;
}
