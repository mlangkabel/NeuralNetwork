#include "neural_network/cpu/NeuralEdgeGroupCpu.h"

#include "tinyxml/tinyxml.h"

#include "neural_network/cpu/NeuralNodeGroupCpu.h"
#include "neural_network/NeuralNetworkConfiguration.h"
#include "utility/logging.h"
#include "utility/utilityString.h"

std::shared_ptr<NeuralEdgeGroupCpu> NeuralEdgeGroupCpu::create(
	const NeuralEdgeGroupConfiguration& configuration,
	const std::vector<std::shared_ptr<NeuralNodeGroupCpu>>& nodeGroups
)
{
	std::shared_ptr<NeuralEdgeGroupCpu> edgeGroup;

	std::shared_ptr<NeuralNodeGroupCpu> sourceNodes;
	std::shared_ptr<NeuralNodeGroupCpu> targetNodes;
	for (const std::shared_ptr<NeuralNodeGroupCpu>& nodeGroup : nodeGroups)
	{
		if (nodeGroup->getId() == configuration.sourceGroupId)
		{
			sourceNodes = nodeGroup;
		}
		if (nodeGroup->getId() == configuration.targetGroupId)
		{
			targetNodes = nodeGroup;
		}
	}

	if (sourceNodes && targetNodes)
	{
		edgeGroup = std::make_shared<NeuralEdgeGroupCpu>(sourceNodes, targetNodes);
		edgeGroup->setWeights(configuration.weights);
	}

	return edgeGroup;
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
