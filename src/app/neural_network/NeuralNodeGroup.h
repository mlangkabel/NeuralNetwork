#ifndef NEURAL_NODE_GROUP_H
#define NEURAL_NODE_GROUP_H

#include <memory>
#include <vector>

#include <CL/cl.hpp>

class NeuralNodeGroup
{
public:
	NeuralNodeGroup(const int nodeCount, cl::Context context, cl::CommandQueue queue);

	int getNodeCount() const;
	cl::Buffer getBuffer();

	void setActivationLevels(std::vector<float> activationLevels);
	std::vector<float> getActivationLevels();
private:
	const int m_nodeCount;
	cl::Context m_context;
	cl::CommandQueue m_queue;
	cl::Buffer m_activationLevelsBuffer;
};


#endif // NEURAL_NODE_GROUP_H
