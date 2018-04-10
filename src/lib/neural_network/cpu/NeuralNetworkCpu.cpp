#include "neural_network/cpu/NeuralNetworkCpu.h"

#include "tinyxml/tinyxml.h"

#include "neural_network/cpu/NeuralEdgeGroupCpu.h"
#include "neural_network/cpu/NeuralNodeGroupCpu.h"
#include "neural_network/NeuralNetworkConfiguration.h"

std::shared_ptr<NeuralNetworkCpu> NeuralNetworkCpu::create(const NeuralNetworkConfiguration& configuration)
{
	std::shared_ptr<NeuralNetworkCpu> neuralNetwork = std::shared_ptr<NeuralNetworkCpu>(new NeuralNetworkCpu());

	for (const NeuralNodeGroupConfiguration& nodeGroupConfiguration : configuration.nodeGroups)
	{
		std::shared_ptr<NeuralNodeGroupCpu> nodeGroup = NeuralNodeGroupCpu::create(nodeGroupConfiguration);
		if (nodeGroup)
		{
			neuralNetwork->m_nodeGroups.push_back(nodeGroup);
			if (nodeGroup->getId() == configuration.inputGroupId)
			{
				neuralNetwork->m_inputNodeGroup = nodeGroup;
			}
			if (nodeGroup->getId() == configuration.outputGroupId)
			{
				neuralNetwork->m_outputNodeGroup = nodeGroup;
			}
		}
		else
		{
			throw std::logic_error("Could not initialize node group from configuration.");
		}
	}
	for (const NeuralEdgeGroupConfiguration& edgeGroupConfiguration : configuration.edgeGroups)
	{
		std::shared_ptr<NeuralEdgeGroupCpu> edgeGroup = NeuralEdgeGroupCpu::create(edgeGroupConfiguration, neuralNetwork->m_nodeGroups);
		if (edgeGroup)
		{
			neuralNetwork->m_edgeGroups.push_back(edgeGroup);
		}
		else
		{
			throw std::logic_error("Could not initialize edge group from configuration.");
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
