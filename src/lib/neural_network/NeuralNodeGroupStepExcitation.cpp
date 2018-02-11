#include "neural_network/NeuralNodeGroupStepExcitation.h"

#include "utility/cl/ClSystem.h"

NeuralNodeGroupStepExcitation::NeuralNodeGroupStepExcitation(const Id id, const int nodeCount, const float threshold)
	: NeuralNodeGroup(id, nodeCount)
	, m_threshold(threshold)
{
	const std::string kernelName = "excitation_step";
	const std::string kernelCode = (
		"void kernel " + kernelName + "(const __global float* source_values, const float threshold, __global float* target_values){\n"
		"	const int id = get_global_id(0);\n"
		"	target_values[id] = source_values[id] < threshold ? 0.0 : 1.0;\n"
		"}\n"
	);

	Optional<cl::Kernel> kernel = ClSystem::getInstance()->addKernel(kernelName, kernelCode);
	if (kernel)
	{
		m_kernel = kernel.get();
	}
}

void NeuralNodeGroupStepExcitation::update()
{
	m_kernel.setArg(0, getExcitationLevelsBuffer());
	m_kernel.setArg(1, m_threshold);
	m_kernel.setArg(2, getExcitationStatesBuffer());
	ClSystem::getInstance()->getQueue().enqueueNDRangeKernel(m_kernel, cl::NullRange, cl::NDRange(getNodeCount()), cl::NullRange);
	ClSystem::getInstance()->getQueue().finish();
}
