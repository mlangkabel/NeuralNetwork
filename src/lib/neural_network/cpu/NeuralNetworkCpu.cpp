#include "neural_network/cpu/NeuralNetworkCpu.h"

#include "tinyxml/tinyxml.h"

#include "neural_network/cpu/NeuralEdgeGroupCpu.h"
#include "neural_network/cpu/NeuralNodeGroupLinearExcitationCpu.h"
#include "neural_network/cpu/NeuralNodeGroupStepExcitationCpu.h"

std::shared_ptr<NeuralNetworkCpu> NeuralNetworkCpu::load(std::shared_ptr<const TextAccess> textAccess)
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
		return std::shared_ptr<NeuralNetworkCpu>();
	}

	std::shared_ptr<NeuralNetworkCpu> neuralNetwork = std::shared_ptr<NeuralNetworkCpu>(new NeuralNetworkCpu());

	{
		const std::string elementName = "node_group_step_excitation";
		const TiXmlElement* elementNodeGroup = elementNeuralNetwork->FirstChildElement(elementName.c_str());
		while (elementNodeGroup)
		{
			std::shared_ptr<NeuralNodeGroupCpu> nodeGroup = NeuralNodeGroupStepExcitationCpu::loadFromXmlElement(elementNodeGroup);
			if (!nodeGroup)
			{
				return std::shared_ptr<NeuralNetworkCpu>();
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
			std::shared_ptr<NeuralNodeGroupCpu> nodeGroup = NeuralNodeGroupLinearExcitationCpu::loadFromXmlElement(elementNodeGroup);
			if (!nodeGroup)
			{
				return std::shared_ptr<NeuralNetworkCpu>();
			}
			neuralNetwork->m_nodeGroups.push_back(nodeGroup);
			elementNodeGroup = elementNodeGroup->NextSiblingElement(elementName.c_str());
		}
	}

	{
		int inputNodeGroupId = -1;
		if (elementNeuralNetwork->QueryIntAttribute("input_node_group_id", &inputNodeGroupId) != TIXML_SUCCESS)
		{
			return std::shared_ptr<NeuralNetworkCpu>();
		}
		for (std::shared_ptr<NeuralNodeGroupCpu> nodeGroup : neuralNetwork->m_nodeGroups)
		{
			if (nodeGroup->getId() == inputNodeGroupId)
			{
				neuralNetwork->m_inputNodeGroup = nodeGroup;
				break;
			}
		}
		if (!neuralNetwork->m_inputNodeGroup)
		{
			return std::shared_ptr<NeuralNetworkCpu>();
		}
	}

	{
		int outputNodeGroupId = -1;
		if (elementNeuralNetwork->QueryIntAttribute("output_node_group_id", &outputNodeGroupId) != TIXML_SUCCESS)
		{
			return std::shared_ptr<NeuralNetworkCpu>();
		}
		for (std::shared_ptr<NeuralNodeGroupCpu> nodeGroup : neuralNetwork->m_nodeGroups)
		{
			if (nodeGroup->getId() == outputNodeGroupId)
			{
				neuralNetwork->m_outputNodeGroup = nodeGroup;
				break;
			}
		}
		if (!neuralNetwork->m_outputNodeGroup)
		{
			return std::shared_ptr<NeuralNetworkCpu>();
		}
	}

	{
		const std::string elementName = "edge_group";
		const TiXmlElement* elementEdgeGroup = elementNeuralNetwork->FirstChildElement(elementName.c_str());
		while (elementEdgeGroup)
		{
			std::shared_ptr<NeuralEdgeGroupCpu> edgeGroup = NeuralEdgeGroupCpu::loadFromXmlElement(elementEdgeGroup, neuralNetwork->m_nodeGroups);
			if (!edgeGroup)
			{
				return std::shared_ptr<NeuralNetworkCpu>();
			}
			neuralNetwork->m_edgeGroups.push_back(edgeGroup);
			elementEdgeGroup = elementEdgeGroup->NextSiblingElement(elementName.c_str());
		}
	}

	return neuralNetwork;
}

void NeuralNetworkCpu::setInputExcitationLevels(std::vector<float> excitationLevels)
{
	m_inputNodeGroup->setExcitationLevels(excitationLevels);
}

std::vector<float> NeuralNetworkCpu::getOutputExcitationStates()
{
	return m_outputNodeGroup->getExcitationStates();
}

void NeuralNetworkCpu::update()
{
	for (std::shared_ptr<NeuralNodeGroupCpu> nodeGroup : m_nodeGroups)
	{
		nodeGroup->update();
	}
	for (std::shared_ptr<NeuralEdgeGroupCpu> edgeGroup : m_edgeGroups)
	{
		edgeGroup->update();
	}
}
