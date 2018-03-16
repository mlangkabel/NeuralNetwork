#include "neural_network/cpu/NeuralNodeGroupLinearExcitationCpu.h"

#include "tinyxml/tinyxml.h"

std::shared_ptr<NeuralNodeGroupLinearExcitationCpu> NeuralNodeGroupLinearExcitationCpu::loadFromXmlElement(const TiXmlElement* element)
{
	std::shared_ptr<NeuralNodeGroupLinearExcitationCpu> nodeGroup;
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

		nodeGroup = std::make_shared<NeuralNodeGroupLinearExcitationCpu>(id, nodeCount, excitationFactor);
	}
	return nodeGroup;
}

NeuralNodeGroupLinearExcitationCpu::NeuralNodeGroupLinearExcitationCpu(const Id id, const int nodeCount, const float excitationFactor)
	: NeuralNodeGroupCpu(id, nodeCount)
	, m_excitationFactor(excitationFactor)
{
}

float NeuralNodeGroupLinearExcitationCpu::getExcitationFactor() const
{
	return m_excitationFactor;
}

void NeuralNodeGroupLinearExcitationCpu::update()
{
	for (int i = 0; i < getNodeCount(); i++)
	{
		m_excitationStates[i] = m_excitationLevels[i] * m_excitationFactor;
	}
}
