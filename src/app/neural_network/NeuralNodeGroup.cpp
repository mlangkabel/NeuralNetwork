#include "neural_network/NeuralNodeGroup.h"

#include "utility/logging.h"

NeuralNodeGroup::NeuralNodeGroup(const int nodeCount, cl::Context context, cl::CommandQueue queue)
	: m_nodeCount(nodeCount)
	, m_context(context)
	, m_queue(queue)
	, m_activationLevelsBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_float) * nodeCount)
{
}

int NeuralNodeGroup::getNodeCount() const
{
	return m_nodeCount;
}

cl::Buffer NeuralNodeGroup::getBuffer()
{
	return m_activationLevelsBuffer;
}

void NeuralNodeGroup::setActivationLevels(std::vector<float> activationLevels)
{
	if (activationLevels.size() == m_nodeCount)
	{
		m_queue.enqueueWriteBuffer(m_activationLevelsBuffer, CL_TRUE, 0, sizeof(cl_float) * m_nodeCount, &activationLevels[0]);
	}
	else
	{
		LOG_ERROR("The amount of values mismatches the amount of nodes in this group.");
	}
}

std::vector<float> NeuralNodeGroup::getActivationLevels()
{
	std::vector<float> activationLevels(m_nodeCount, 0.0f);
	m_queue.enqueueReadBuffer(m_activationLevelsBuffer, CL_TRUE, 0, sizeof(cl_float) * m_nodeCount, &activationLevels[0]);
	return activationLevels;
}
