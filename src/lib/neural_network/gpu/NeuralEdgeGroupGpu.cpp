#include "neural_network/gpu/NeuralEdgeGroupGpu.h"

#include "tinyxml/tinyxml.h"

#include "neural_network/gpu/NeuralNodeGroupGpu.h"
#include "utility/cl/ClSystem.h"
#include "utility/logging.h"
#include "utility/utilityString.h"

std::shared_ptr<NeuralEdgeGroupGpu> NeuralEdgeGroupGpu::loadFromXmlElement(const TiXmlElement* element, const std::vector<std::shared_ptr<NeuralNodeGroupGpu>>& nodeGroups)
{
	std::shared_ptr<NeuralEdgeGroupGpu> nodeGroup;
	if (element)
	{
		if (std::string(element->Value()) != "edge_group")
		{
			return nodeGroup;
		}

		int sourceGroupId;
		if (!element->QueryIntAttribute("source_group_id", &sourceGroupId) == TIXML_SUCCESS || sourceGroupId < 0)
		{
			return nodeGroup;
		}

		int targetGroupId;
		if (!element->QueryIntAttribute("target_group_id", &targetGroupId) == TIXML_SUCCESS || targetGroupId < 0)
		{
			return nodeGroup;
		}

		const std::string weightsString = element->Attribute("weights");
		if (weightsString.empty())
		{
			return nodeGroup;
		}

		std::shared_ptr<NeuralNodeGroupGpu> sourceNodes;
		std::shared_ptr<NeuralNodeGroupGpu> targetNodes;
		for (const std::shared_ptr<NeuralNodeGroupGpu>& nodeGroup : nodeGroups)
		{
			if (nodeGroup->getId() == sourceGroupId)
			{
				sourceNodes = nodeGroup;
			}
			if (nodeGroup->getId() == targetGroupId)
			{
				targetNodes = nodeGroup;
			}
		}
		
		if (sourceNodes && targetNodes)
		{
			Matrix<float> weights(sourceNodes->getNodeCount(), targetNodes->getNodeCount());
			if (!weights.setValuesFromString(weightsString))
			{
				return nodeGroup;
			}
			nodeGroup = std::make_shared<NeuralEdgeGroupGpu>(sourceNodes, targetNodes);
			nodeGroup->setWeights(weights);
		}
	}
	return nodeGroup;
}

NeuralEdgeGroupGpu::NeuralEdgeGroupGpu(
	std::shared_ptr<NeuralNodeGroupGpu> sourceNodes, 
	std::shared_ptr<NeuralNodeGroupGpu> targetNodes
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

void NeuralEdgeGroupGpu::setWeights(Matrix<float> weights)
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

Matrix<float> NeuralEdgeGroupGpu::getWeights()
{
	Matrix<float> weights(m_sourceNodes->getNodeCount(), m_targetNodes->getNodeCount());
	ClSystem::getInstance()->getQueue().enqueueReadBuffer(m_weightsBuffer, CL_TRUE, 0, sizeof(cl_float) * weights.getElementCount(), &weights[0]);
	return weights;
}

void NeuralEdgeGroupGpu::update()
{
	m_kernel.setArg(0, m_sourceNodes->getNodeCount());
	m_kernel.setArg(1, m_sourceNodes->getExcitationStatesBuffer());
	m_kernel.setArg(2, m_weightsBuffer);
	m_kernel.setArg(3, m_targetNodes->getExcitationLevelsBuffer());
	ClSystem::getInstance()->getQueue().enqueueNDRangeKernel(m_kernel, cl::NullRange, cl::NDRange(m_sourceNodes->getNodeCount()), cl::NullRange);
	ClSystem::getInstance()->getQueue().finish();
}
