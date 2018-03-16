#include "neural_network/cpu/NeuralEdgeGroupCpu.h"

#include "tinyxml/tinyxml.h"

#include "neural_network/cpu/NeuralNodeGroupCpu.h"
#include "utility/logging.h"
#include "utility/utilityString.h"

std::shared_ptr<NeuralEdgeGroupCpu> NeuralEdgeGroupCpu::loadFromXmlElement(const TiXmlElement* element, const std::vector<std::shared_ptr<NeuralNodeGroupCpu>>& nodeGroups)
{
	std::shared_ptr<NeuralEdgeGroupCpu> nodeGroup;
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

		std::shared_ptr<NeuralNodeGroupCpu> sourceNodes;
		std::shared_ptr<NeuralNodeGroupCpu> targetNodes;
		for (const std::shared_ptr<NeuralNodeGroupCpu>& nodeGroup : nodeGroups)
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
			nodeGroup = std::make_shared<NeuralEdgeGroupCpu>(sourceNodes, targetNodes);
			nodeGroup->setWeights(weights);
		}
	}
	return nodeGroup;
}

NeuralEdgeGroupCpu::NeuralEdgeGroupCpu(
	std::shared_ptr<NeuralNodeGroupCpu> sourceNodes,
	std::shared_ptr<NeuralNodeGroupCpu> targetNodes
)
	: m_sourceNodes(sourceNodes)
	, m_targetNodes(targetNodes)
	, m_weights(sourceNodes->getNodeCount(), targetNodes->getNodeCount())
{
}

void NeuralEdgeGroupCpu::setWeights(Matrix<float> weights)
{
	if (weights.getWidth() == m_sourceNodes->getNodeCount() && weights.getHeight() == m_targetNodes->getNodeCount())
	{
		m_weights = weights;
	}
	else
	{
		LOG_ERROR("The size of the weights matrix does not match the source and target node count.");
	}
}

Matrix<float> NeuralEdgeGroupCpu::getWeights() const
{
	return m_weights;
}

void NeuralEdgeGroupCpu::update()
{
	std::vector<float>& sourceExcitationStates = m_sourceNodes->getExcitationStates();
	std::vector<float>& targetExcitationLevels = m_targetNodes->getExcitationLevels();

	for (int y = 0; y < m_targetNodes->getNodeCount(); y++)
	{
		float v = 0.0f;
		for (int x = 0; x < m_sourceNodes->getNodeCount(); x++)
		{
			v += m_weights.getValue(x, y) * sourceExcitationStates[x];
		}
		targetExcitationLevels[y] += v;
	}
}
