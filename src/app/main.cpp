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
	std::vector<cl::Platform> all_platforms;
	cl::Platform::get(&all_platforms);
	if (all_platforms.size() == 0) 
	{
		LOG_ERROR("No platforms found. Check OpenCL installation!");
		exit(1);
	}
	cl::Platform default_platform = all_platforms[0];
	LOG_INFO("Using platform: " + default_platform.getInfo<CL_PLATFORM_NAME>() + " Version: " + default_platform.getInfo<CL_PLATFORM_VERSION>());

	//get default device of the default platform
	std::vector<cl::Device> all_devices;
	default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
	if (all_devices.size() == 0) {
		LOG_ERROR("No devices found. Check OpenCL installation!");
		exit(1);
	}
	cl::Device default_device = all_devices[0];
	LOG_INFO("Using device: " + default_device.getInfo<CL_DEVICE_NAME>());

	cl::Context context(default_device);

	//create queue to which we will push commands for the device.
	cl::CommandQueue queue(context, default_device);


	const int NODE_COUNT = 5000;
	const int REPETITIONS = 2000;
	RandomNumberGenerator rng(1);

	std::shared_ptr<NeuralNodeGroup> sourceNodeGroup = std::make_shared<NeuralNodeGroup>(NODE_COUNT, context, queue);
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

	std::shared_ptr<NeuralNodeGroup> targetNodeGroup = std::make_shared<NeuralNodeGroup>(NODE_COUNT, context, queue);

	NeuralEdgeGroup edgeGroup(sourceNodeGroup, targetNodeGroup, default_device, context, queue);
	
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
