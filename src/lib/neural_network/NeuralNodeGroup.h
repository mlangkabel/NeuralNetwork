#ifndef NEURAL_NODE_GROUP_H
#define NEURAL_NODE_GROUP_H

#include <memory>
#include <vector>

#include <CL/cl.hpp>

class NeuralNodeGroup
{
public:
	NeuralNodeGroup(const int nodeCount);

	int getNodeCount() const;
	cl::Buffer getBuffer();

	void setExcitationLevels(std::vector<float> excitationLevels);
	std::vector<float> getExcitationLevels();
private:
	const int m_nodeCount;
	cl::Buffer m_excitationLevelsBuffer;
};


#endif // NEURAL_NODE_GROUP_H
