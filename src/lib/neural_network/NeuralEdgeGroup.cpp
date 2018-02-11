#include "neural_network/NeuralEdgeGroup.h"

#include "neural_network/NeuralNodeGroup.h"
#include "utility/cl/ClSystem.h"
#include "utility/logging.h"

NeuralEdgeGroup::NeuralEdgeGroup(
	std::shared_ptr<NeuralNodeGroup> sourceNodes, 
	std::shared_ptr<NeuralNodeGroup> targetNodes
)
	: m_sourceNodes(sourceNodes)
	, m_targetNodes(targetNodes)
	, m_weightsBuffer(ClSystem::getInstance()->getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * m_sourceNodes->getNodeCount() * m_targetNodes->getNodeCount())
{
	const std::string kernelName = "test";
	const std::string kernelCode = (
		"void kernel test(const int source_value_count, const __global float* source_values, const __global float* weights, __global float* target_values){\n" 
		"	const int id = get_global_id(0);\n"
		"	float acc = 0.0f;\n"
		"	for (int i = 0; i < source_value_count; i++)\n"
		"	{\n"
		"		acc += weights[source_value_count * id + i] * source_values[i];\n"
		"	}\n"
		"	target_values[id] = acc;\n"
		"}\n"
	);

	Optional<cl::Kernel> kernel = ClSystem::getInstance()->addKernel(kernelName, kernelCode);
	if (kernel)
	{
		m_kernel = kernel.get();
	}
}

void NeuralEdgeGroup::setWeights(Matrix<float> weights)
{
	if (weights.getWidth() == m_sourceNodes->getNodeCount() && weights.getHeight() == m_targetNodes->getNodeCount())
	{
		ClSystem::getInstance()->getQueue().enqueueWriteBuffer(m_weightsBuffer, CL_TRUE, 0, sizeof(cl_float) * weights.getElementCount(), &weights[0]);
	}
	else
	{
		LOG_ERROR("The size of the weights matrix does not match the source and target node count.");
	}
}

Matrix<float> NeuralEdgeGroup::getWeights()
{
	Matrix<float> weights(m_sourceNodes->getNodeCount(), m_targetNodes->getNodeCount());
	ClSystem::getInstance()->getQueue().enqueueReadBuffer(m_weightsBuffer, CL_TRUE, 0, sizeof(cl_float) * weights.getElementCount(), &weights[0]);
	return weights;
}

void NeuralEdgeGroup::update()
{
	m_kernel.setArg(0, m_sourceNodes->getNodeCount());
	m_kernel.setArg(1, m_sourceNodes->getBuffer());
	m_kernel.setArg(2, m_weightsBuffer);
	m_kernel.setArg(3, m_targetNodes->getBuffer());
	ClSystem::getInstance()->getQueue().enqueueNDRangeKernel(m_kernel, cl::NullRange, cl::NDRange(m_sourceNodes->getNodeCount()), cl::NullRange);
	ClSystem::getInstance()->getQueue().finish();
}