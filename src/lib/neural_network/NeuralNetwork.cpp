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

NeuralNetwork::NeuralNetwork()
{
}