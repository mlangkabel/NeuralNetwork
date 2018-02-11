#include "neural_network/NeuralNodeGroupLinearExcitation.h"

#include "tinyxml/tinyxml.h"

#include "utility/cl/ClSystem.h"

std::shared_ptr<NeuralNodeGroupLinearExcitation> NeuralNodeGroupLinearExcitation::loadFromXmlElement(const TiXmlElement* element)
{
	std::shared_ptr<NeuralNodeGroupLinearExcitation> nodeGroup;
	if (element)
	{
		if (std::string(element->Value()) != "node_group_linear_excitation")
		{
			return nodeGroup;
		}

		int id;
		if (!element->QueryIntAttribute("id", &id) == TIXML_SUCCESS || id < 0)
		{
			return nodeGroup;
		}

		int nodeCount;
		if (!element->QueryIntAttribute("node_count", &nodeCount) == TIXML_SUCCESS || nodeCount <= 0)
		{
			return nodeGroup;
		}

		float excitationFactor;
		if (!element->QueryFloatAttribute("excitation_factor", &excitationFactor) == TIXML_SUCCESS)
		{
			return nodeGroup;
		}

		nodeGroup = std::make_shared<NeuralNodeGroupLinearExcitation>(id, nodeCount, excitationFactor);
	}
	return nodeGroup;
}

NeuralNodeGroupLinearExcitation::NeuralNodeGroupLinearExcitation(const Id id, const int nodeCount, const float excitationFactor)
	: NeuralNodeGroup(id, nodeCount)
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

float NeuralNodeGroupLinearExcitation::getExcitationFactor() const
{
	return m_excitationFactor;
}

void NeuralNodeGroupLinearExcitation::update()
{
	m_kernel.setArg(0, getExcitationLevelsBuffer());
	m_kernel.setArg(1, m_excitationFactor);
	m_kernel.setArg(2, getExcitationStatesBuffer());
	ClSystem::getInstance()->getQueue().enqueueNDRangeKernel(m_kernel, cl::NullRange, cl::NDRange(getNodeCount()), cl::NullRange);
	ClSystem::getInstance()->getQueue().finish();
}
