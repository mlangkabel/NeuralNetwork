#include "neural_network/cpu/NeuralNodeGroupStepExcitationCpu.h"

#include "tinyxml/tinyxml.h"

std::shared_ptr<NeuralNodeGroupStepExcitationCpu> NeuralNodeGroupStepExcitationCpu::loadFromXmlElement(const TiXmlElement* element)
{
	std::shared_ptr<NeuralNodeGroupStepExcitationCpu> nodeGroup;
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

		nodeGroup = std::make_shared<NeuralNodeGroupStepExcitationCpu>(id, nodeCount, excitationThreshold, excitationFatigue);
	}
	return nodeGroup;
}

NeuralNodeGroupStepExcitationCpu::NeuralNodeGroupStepExcitationCpu(const Id id, const int nodeCount, const float excitationThreshold, const float excitationFatigue)
	: NeuralNodeGroupCpu(id, nodeCount)
	, m_excitationThreshold(excitationThreshold)
	, m_excitationFatigue(excitationFatigue)
{
}

float NeuralNodeGroupStepExcitationCpu::getExcitationThreshold() const
{
	return m_excitationThreshold;
}

float NeuralNodeGroupStepExcitationCpu::getExcitationFatigue() const
{
	return m_excitationFatigue;
}

void NeuralNodeGroupStepExcitationCpu::update()
{
	for (int i = 0; i < getNodeCount(); i++)
	{
		if (m_excitationLevels[i] > m_excitationThreshold)
		{
			m_excitationLevels[i] *= 1.0f - m_excitationFatigue;
			m_excitationStates[i] = 1.0f;
		}
		else
		{
			m_excitationStates[i] = 0.0f;
		}
	}
}
