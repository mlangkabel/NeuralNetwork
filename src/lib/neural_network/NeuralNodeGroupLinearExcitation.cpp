#include "neural_network/NeuralNodeGroupLinearExcitation.h"

#include "utility/cl/ClSystem.h"

NeuralNodeGroupLinearExcitation::NeuralNodeGroupLinearExcitation(const int nodeCount, const float excitationFactor)
	: NeuralNodeGroup(nodeCount)
	, m_excitationFactor(excitationFactor)
{
	const std::string kernelName = "excitation_linear";
	const std::string kernelCode = (
		"void kernel " + kernelName + "(const __global float* source_values, const float excitation_factor, __global float* target_values){\n"
		"	const int id = get_global_id(0);\n"
		"	target_values[id] = source_values[id] * excitation_factor;\n"
		"}\n"
	);

	Optional<cl::Kernel> kernel = ClSystem::getInstance()->addKernel(kernelName, kernelCode);
	if (kernel)
	{
		m_kernel = kernel.get();
	}
}

void NeuralNodeGroupLinearExcitation::update()
{
	m_kernel.setArg(0, getExcitationLevelsBuffer());
	m_kernel.setArg(1, m_excitationFactor);
	m_kernel.setArg(2, getExcitationStatesBuffer());
	ClSystem::getInstance()->getQueue().enqueueNDRangeKernel(m_kernel, cl::NullRange, cl::NDRange(getNodeCount()), cl::NullRange);
	ClSystem::getInstance()->getQueue().finish();
}
