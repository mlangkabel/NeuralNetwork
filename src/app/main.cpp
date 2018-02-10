#include <iostream>
#include <vector>

#include <CL/cl.hpp>

#include "neural_network/NeuralNodeGroup.h"
#include "neural_network/NeuralEdgeGroup.h"
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

	std::shared_ptr<NeuralNodeGroup> nodeGroup = std::make_shared<NeuralNodeGroup>(2, context, queue);
	nodeGroup->setActivationLevels({ 3.6f, -6.0f });

	for (const float v: nodeGroup->getActivationLevels()) 
	{
		std::cout << v << " ";
	}
	std::cout << std::endl;

	NeuralEdgeGroup edgeGroup(nodeGroup, nodeGroup, default_device, context, queue);
	Matrix<float> weights(2, 2);
	weights.setValue(0, 0, 1.0f);
	weights.setValue(1, 0, 1.0f);
	weights.setValue(0, 1, 1.0f);
	weights.setValue(1, 1, 0.0f);
	edgeGroup.setWeights(weights);

	edgeGroup.update();
	

	for (const float v : nodeGroup->getActivationLevels())
	{
		std::cout << v << " ";
	}
	std::cout << std::endl;

	return 0;
}
