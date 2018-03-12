#include "neural_network/NeuralNetwork.h"

#include "tinyxml/tinyxml.h"

#include "neural_network/NeuralEdgeGroup.h"
#include "neural_network/NeuralNodeGroupLinearExcitation.h"
#include "neural_network/NeuralNodeGroupStepExcitation.h"

std::shared_ptr<NeuralNetwork> NeuralNetwork::load(std::shared_ptr<const TextAccess> textAccess)
{
	TiXmlDocument doc;
	doc.Parse(
		textAccess->getText().c_str(),
		0,
		TIXML_ENCODING_UTF8
	);

	const TiXmlElement* elementNeuralNetwork = doc.FirstChildElement("neural_network");

	if (!elementNeuralNetwork)
	{
		return std::shared_ptr<NeuralNetwork>();
	}

	std::shared_ptr<NeuralNetwork> neuralNetwork = std::shared_ptr<NeuralNetwork>(new NeuralNetwork());

	{
		const std::string elementName = "node_group_step_excitation";
		const TiXmlElement* elementNodeGroup = elementNeuralNetwork->FirstChildElement(elementName.c_str());
		while (elementNodeGroup)
		{
			std::shared_ptr<NeuralNodeGroup> nodeGroup = NeuralNodeGroupStepExcitation::loadFromXmlElement(elementNodeGroup);
			if (!nodeGroup)
			{
				return std::shared_ptr<NeuralNetwork>();
			}
			neuralNetwork->m_nodeGroups.push_back(nodeGroup);
			elementNodeGroup = elementNodeGroup->NextSiblingElement(elementName.c_str());
		}
	}

	{
		const std::string elementName = "node_group_linear_excitation";
		const TiXmlElement* elementNodeGroup = elementNeuralNetwork->FirstChildElement(elementName.c_str());
		while (elementNodeGroup)
		{
			std::shared_ptr<NeuralNodeGroup> nodeGroup = NeuralNodeGroupLinearExcitation::loadFromXmlElement(elementNodeGroup);
			if (!nodeGroup)
			{
				return std::shared_ptr<NeuralNetwork>();
			}
			neuralNetwork->m_nodeGroups.push_back(nodeGroup);
			elementNodeGroup = elementNodeGroup->NextSiblingElement(elementName.c_str());
		}
	}

	{
		int inputNodeGroupId = -1;
		if (elementNeuralNetwork->QueryIntAttribute("input_node_group_id", &inputNodeGroupId) != TIXML_SUCCESS)
		{
			return std::shared_ptr<NeuralNetwork>();
		}
		for (std::shared_ptr<NeuralNodeGroup> nodeGroup : neuralNetwork->m_nodeGroups)
		{
			if (nodeGroup->getId() == inputNodeGroupId)
			{
				neuralNetwork->m_inputNodeGroup = nodeGroup;
				break;
			}
		}
	}

	{
		int outputNodeGroupId = -1;
		if (elementNeuralNetwork->QueryIntAttribute("output_node_group_id", &outputNodeGroupId) != TIXML_SUCCESS)
		{
			return std::shared_ptr<NeuralNetwork>();
		}
		for (std::shared_ptr<NeuralNodeGroup> nodeGroup : neuralNetwork->m_nodeGroups)
		{
			if (nodeGroup->getId() == outputNodeGroupId)
			{
				neuralNetwork->m_outputNodeGroup = nodeGroup;
				break;
			}
		}
	}

	{
		const std::string elementName = "edge_group";
		const TiXmlElement* elementEdgeGroup = elementNeuralNetwork->FirstChildElement(elementName.c_str());
		while (elementEdgeGroup)
		{
			std::shared_ptr<NeuralEdgeGroup> edgeGroup = NeuralEdgeGroup::loadFromXmlElement(elementEdgeGroup, neuralNetwork->m_nodeGroups);
			if (!edgeGroup)
			{
				return std::shared_ptr<NeuralNetwork>();
			}
			neuralNetwork->m_edgeGroups.push_back(edgeGroup);
			elementEdgeGroup = elementEdgeGroup->NextSiblingElement(elementName.c_str());
		}
	}

	return neuralNetwork;
}

void NeuralNetwork::setInputExcitationLevels(std::vector<float> excitationLevels)
{
	m_inputNodeGroup->setExcitationLevels(excitationLevels);
}

std::vector<float> NeuralNetwork::getOutputExcitationStates()
{
	return m_outputNodeGroup->getExcitationStates();
}

void NeuralNetwork::update()
{
	for (std::shared_ptr<NeuralNodeGroup> nodeGroup : m_nodeGroups)
	{
		nodeGroup->update();
	}
	for (std::shared_ptr<NeuralEdgeGroup> edgeGroup : m_edgeGroups)
	{
		edgeGroup->update();
	}
}

NeuralNetwork::NeuralNetwork()
{
}
