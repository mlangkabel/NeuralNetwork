#include "neural_network/cpu/NeuralNodeGroupCpu.h"

#include "utility/logging.h"

std::shared_ptr<NeuralNodeGroupCpu> NeuralNodeGroupCpu::create(const NeuralNodeGroupConfiguration& configuration)
{
	return std::make_shared<NeuralNodeGroupCpu>(
		configuration.id,
		configuration.nodeCount,
		configuration.excitationOffset,
		configuration.excitationMultiplier,
		configuration.excitationThreshold,
		configuration.excitationFatigue
	);
}

NeuralNodeGroupCpu::NeuralNodeGroupCpu(const Id id, const int nodeCount, float excitationOffset, float excitationMultiplier, float excitationThreshold, float excitationFatigue)
	: m_id(id)
	, m_nodeCount(nodeCount)
	, m_excitationOffset(excitationOffset)
	, m_excitationMultiplier(excitationMultiplier)
	, m_excitationThreshold(excitationThreshold)
	, m_excitationFatigue(excitationFatigue)
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

void NeuralNodeGroupCpu::update()
{
	for (int i = 0; i < getNodeCount(); i++)
	{
		if (m_excitationLevels[i] > m_excitationThreshold)
		{
			m_excitationStates[i] = m_excitationLevels[i] * m_excitationMultiplier + m_excitationOffset;
			m_excitationLevels[i] *= 1.0f - m_excitationFatigue;
		}
		else
		{
			m_excitationStates[i] = 0.0f;
		}
	}
}