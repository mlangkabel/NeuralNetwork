#include "neural_network/cpu/NeuralNodeGroupCpu.h"

#include "utility/logging.h"

NeuralNodeGroupCpu::NeuralNodeGroupCpu(const Id id, const int nodeCount)
	: m_id(id)
	, m_nodeCount(nodeCount)
	, m_excitationLevels(nodeCount, 0.0f)
	, m_excitationStates(nodeCount, 0.0f)
{
}

Id NeuralNodeGroupCpu::getId() const
{
	return m_id;
}

int NeuralNodeGroupCpu::getNodeCount() const
{
	return m_nodeCount;
}

void NeuralNodeGroupCpu::setExcitationLevels(std::vector<float> excitationLevels)
{
	if (excitationLevels.size() == m_nodeCount)
	{
		m_excitationLevels = excitationLevels;
	}
	else
	{
		LOG_ERROR("The amount of values mismatches the amount of nodes in this group.");
	}
}

std::vector<float>& NeuralNodeGroupCpu::getExcitationLevels()
{
	return m_excitationLevels;
}

void NeuralNodeGroupCpu::setExcitationStates(std::vector<float> excitationStates)
{
	if (excitationStates.size() == m_nodeCount)
	{
		m_excitationStates = excitationStates;
	}
	else
	{
		LOG_ERROR("The amount of values mismatches the amount of nodes in this group.");
	}
}

std::vector<float>& NeuralNodeGroupCpu::getExcitationStates()
{
	return m_excitationStates;
}
