#include "neural_network/gpu/NeuralNetworkGpu.h"

#include "tinyxml/tinyxml.h"

#include "neural_network/gpu/NeuralEdgeGroupGpu.h"
#include "neural_network/gpu/NeuralNodeGroupLinearExcitationGpu.h"
#include "neural_network/gpu/NeuralNodeGroupStepExcitationGpu.h"

std::shared_ptr<NeuralNetworkGpu> NeuralNetworkGpu::load(std::shared_ptr<const TextAccess> textAccess)
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
		return std::shared_ptr<NeuralNetworkGpu>();
	}

	std::shared_ptr<NeuralNetworkGpu> neuralNetwork = std::shared_ptr<NeuralNetworkGpu>(new NeuralNetworkGpu());

	{
		const std::string elementName = "node_group_step_excitation";
		const TiXmlElement* elementNodeGroup = elementNeuralNetwork->FirstChildElement(elementName.c_str());
		while (elementNodeGroup)
		{
			std::shared_ptr<NeuralNodeGroupGpu> nodeGroup = NeuralNodeGroupStepExcitationGpu::loadFromXmlElement(elementNodeGroup);
			if (!nodeGroup)
			{
				return std::shared_ptr<NeuralNetworkGpu>();
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
			std::shared_ptr<NeuralNodeGroupGpu> nodeGroup = NeuralNodeGroupLinearExcitationGpu::loadFromXmlElement(elementNodeGroup);
			if (!nodeGroup)
			{
				return std::shared_ptr<NeuralNetworkGpu>();
			}
			neuralNetwork->m_nodeGroups.push_back(nodeGroup);
			elementNodeGroup = elementNodeGroup->NextSiblingElement(elementName.c_str());
		}
	}

	{
		int inputNodeGroupId = -1;
		if (elementNeuralNetwork->QueryIntAttribute("input_node_group_id", &inputNodeGroupId) != TIXML_SUCCESS)
		{
			return std::shared_ptr<NeuralNetworkGpu>();
		}
		for (std::shared_ptr<NeuralNodeGroupGpu> nodeGroup : neuralNetwork->m_nodeGroups)
		{
			if (nodeGroup->getId() == inputNodeGroupId)
			{
				neuralNetwork->m_inputNodeGroup = nodeGroup;
				break;
			}
		}
		if (!neuralNetwork->m_inputNodeGroup)
		{
			return std::shared_ptr<NeuralNetworkGpu>();
		}
	}

	{
		int outputNodeGroupId = -1;
		if (elementNeuralNetwork->QueryIntAttribute("output_node_group_id", &outputNodeGroupId) != TIXML_SUCCESS)
		{
			return std::shared_ptr<NeuralNetworkGpu>();
		}
		for (std::shared_ptr<NeuralNodeGroupGpu> nodeGroup : neuralNetwork->m_nodeGroups)
		{
			if (nodeGroup->getId() == outputNodeGroupId)
			{
				neuralNetwork->m_outputNodeGroup = nodeGroup;
				break;
			}
		}
		if (!neuralNetwork->m_outputNodeGroup)
		{
			return std::shared_ptr<NeuralNetworkGpu>();
		}
	}

	{
		const std::string elementName = "edge_group";
		const TiXmlElement* elementEdgeGroup = elementNeuralNetwork->FirstChildElement(elementName.c_str());
		while (elementEdgeGroup)
		{
			std::shared_ptr<NeuralEdgeGroupGpu> edgeGroup = NeuralEdgeGroupGpu::loadFromXmlElement(elementEdgeGroup, neuralNetwork->m_nodeGroups);
			if (!edgeGroup)
			{
				return std::shared_ptr<NeuralNetworkGpu>();
			}
			neuralNetwork->m_edgeGroups.push_back(edgeGroup);
			elementEdgeGroup = elementEdgeGroup->NextSiblingElement(elementName.c_str());
		}
	}

	return neuralNetwork;
}

void NeuralNetworkGpu::setInputExcitationLevels(std::vector<float> excitationLevels)
{
	m_inputNodeGroup->setExcitationLevels(excitationLevels);
}

std::vector<float> NeuralNetworkGpu::getOutputExcitationStates()
{
	return m_outputNodeGroup->getExcitationStates();
}

void NeuralNetworkGpu::update()
{
	for (std::shared_ptr<NeuralNodeGroupGpu> nodeGroup : m_nodeGroups)
	{
		nodeGroup->update();
	}
	for (std::shared_ptr<NeuralEdgeGroupGpu> edgeGroup : m_edgeGroups)
	{
		edgeGroup->update();
	}
}
