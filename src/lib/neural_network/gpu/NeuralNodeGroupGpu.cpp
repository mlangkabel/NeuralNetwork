#include "neural_network/gpu/NeuralNodeGroupGpu.h"

#include "utility/cl/ClSystem.h"
#include "utility/logging.h"

NeuralNodeGroupGpu::NeuralNodeGroupGpu(const Id id, const int nodeCount)
	: m_id(id)
	, m_nodeCount(nodeCount)
	, m_excitationLevelsBuffer(ClSystem::getInstance()->getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * nodeCount)
	, m_excitationStatesBuffer(ClSystem::getInstance()->getContext(), CL_MEM_READ_WRITE, sizeof(cl_float) * nodeCount)
{
}

Id NeuralNodeGroupGpu::getId() const
{
	return m_id;
}

int NeuralNodeGroupGpu::getNodeCount() const
{
	return m_nodeCount;
}

cl::Buffer NeuralNodeGroupGpu::getExcitationLevelsBuffer()
{
	return m_excitationLevelsBuffer;
}

cl::Buffer NeuralNodeGroupGpu::getExcitationStatesBuffer()
{
	return m_excitationStatesBuffer;
}

void NeuralNodeGroupGpu::setExcitationLevels(std::vector<float> excitationLevels)
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

std::vector<float> NeuralNodeGroupGpu::getExcitationLevels()
{
	std::vector<float> excitationLevels(m_nodeCount, 0.0f);
	ClSystem::getInstance()->getQueue().enqueueReadBuffer(m_excitationLevelsBuffer, CL_TRUE, 0, sizeof(cl_float) * m_nodeCount, &excitationLevels[0]);
	return excitationLevels;
}

void NeuralNodeGroupGpu::setExcitationStates(std::vector<float> excitationStates)
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

std::vector<float> NeuralNodeGroupGpu::getExcitationStates()
{
	std::vector<float> excitationStates(m_nodeCount, 0.0f);
	ClSystem::getInstance()->getQueue().enqueueReadBuffer(m_excitationStatesBuffer, CL_TRUE, 0, sizeof(cl_float) * m_nodeCount, &excitationStates[0]);
	return excitationStates;
}
