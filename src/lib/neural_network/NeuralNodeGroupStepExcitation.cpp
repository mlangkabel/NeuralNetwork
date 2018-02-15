#include "neural_network/NeuralNodeGroupStepExcitation.h"

#include "tinyxml/tinyxml.h"

#include "utility/cl/ClSystem.h"

std::shared_ptr<NeuralNodeGroupStepExcitation> NeuralNodeGroupStepExcitation::loadFromXmlElement(const TiXmlElement* element)
{
	std::shared_ptr<NeuralNodeGroupStepExcitation> nodeGroup;
	if (element)
	{
		if (std::string(element->Value()) != "node_group_step_excitation")
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

		float excitationThreshold;
		if (!element->QueryFloatAttribute("excitation_threshold", &excitationThreshold) == TIXML_SUCCESS)
		{
			return nodeGroup;
		}

		float excitationFatigue;
		if (!element->QueryFloatAttribute("excitation_fatigue", &excitationFatigue) == TIXML_SUCCESS)
		{
			return nodeGroup;
		}

		nodeGroup = std::make_shared<NeuralNodeGroupStepExcitation>(id, nodeCount, excitationThreshold, excitationFatigue);
	}
	return nodeGroup;
}

NeuralNodeGroupStepExcitation::NeuralNodeGroupStepExcitation(const Id id, const int nodeCount, const float excitationThreshold, const float excitationFatigue)
	: NeuralNodeGroup(id, nodeCount)
	, m_excitationThreshold(excitationThreshold)
	, m_excitationFatigue(excitationFatigue)
{
	const std::string kernelName = "excitation_step";
	const std::string kernelCode = (
		"void kernel " + kernelName + "(__global float* source_values, const float excitation_threshold, const float excitation_fatigue, __global float* target_values) {\n"
		"	const int id = get_global_id(0);\n"
		"	if (source_values[id] > excitation_threshold) {\n"
		"		source_values[id] *= 1.0 - excitation_fatigue;\n"
		"		target_values[id] = 1.0;\n"
		"	} else {\n"
		"		target_values[id] = 0.0;\n"
		"	}\n"
		"}\n"
	);

	Optional<cl::Kernel> kernel = ClSystem::getInstance()->addKernel(kernelName, kernelCode);
	if (kernel)
	{
		m_kernel = kernel.get();
	}
}

float NeuralNodeGroupStepExcitation::getExcitationThreshold() const
{
	return m_excitationThreshold;
}

float NeuralNodeGroupStepExcitation::getExcitationFatigue() const
{
	return m_excitationFatigue;
}

void NeuralNodeGroupStepExcitation::update()
{
	m_kernel.setArg(0, getExcitationLevelsBuffer());
	m_kernel.setArg(1, m_excitationThreshold);
	m_kernel.setArg(2, m_excitationFatigue);
	m_kernel.setArg(3, getExcitationStatesBuffer());
	ClSystem::getInstance()->getQueue().enqueueNDRangeKernel(m_kernel, cl::NullRange, cl::NDRange(getNodeCount()), cl::NullRange);
	ClSystem::getInstance()->getQueue().finish();
}
