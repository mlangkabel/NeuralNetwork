#include "neural_network/NeuralNodeGroup.h"

#include "utility/cl/ClSystem.h"
#include "utility/logging.h"

NeuralNodeGroup::NeuralNodeGroup(const int nodeCount)
	: m_nodeCount(nodeCount)
	, m_excitationLevelsBuffer(ClSystem::getInstance()->getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * nodeCount)
	, m_excitationStatesBuffer(ClSystem::getInstance()->getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * nodeCount)
{
}

NeuralNodeGroup::~NeuralNodeGroup()
{
}

int NeuralNodeGroup::getNodeCount() const
{
	return m_nodeCount;
}

cl::Buffer NeuralNodeGroup::getExcitationLevelsBuffer()
{
	return m_excitationLevelsBuffer;
}

cl::Buffer NeuralNodeGroup::getExcitationStatesBuffer()
{
	return m_excitationStatesBuffer;
}

void NeuralNodeGroup::setExcitationLevels(std::vector<float> excitationLevels)
{
	if (excitationLevels.size() == m_nodeCount)
	{
		ClSystem::getInstance()->getQueue().enqueueWriteBuffer(m_excitationLevelsBuffer, CL_TRUE, 0, sizeof(cl_float) * m_nodeCount, &excitationLevels[0]);
	}
	else
	{
		LOG_ERROR("The amount of values mismatches the amount of nodes in this group.");
	}
}

std::vector<float> NeuralNodeGroup::getExcitationLevels()
{
	std::vector<float> excitationLevels(m_nodeCount, 0.0f);
	ClSystem::getInstance()->getQueue().enqueueReadBuffer(m_excitationLevelsBuffer, CL_TRUE, 0, sizeof(cl_float) * m_nodeCount, &excitationLevels[0]);
	return excitationLevels;
}

void NeuralNodeGroup::setExcitationStates(std::vector<float> excitationStates)
{
	if (excitationStates.size() == m_nodeCount)
	{
		ClSystem::getInstance()->getQueue().enqueueWriteBuffer(m_excitationStatesBuffer, CL_TRUE, 0, sizeof(cl_float) * m_nodeCount, &excitationStates[0]);
	}
	else
	{
		LOG_ERROR("The amount of values mismatches the amount of nodes in this group.");
	}
}

std::vector<float> NeuralNodeGroup::getExcitationStates()
{
	std::vector<float> excitationStates(m_nodeCount, 0.0f);
	ClSystem::getInstance()->getQueue().enqueueReadBuffer(m_excitationStatesBuffer, CL_TRUE, 0, sizeof(cl_float) * m_nodeCount, &excitationStates[0]);
	return excitationStates;
}
