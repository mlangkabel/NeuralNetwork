#ifndef NEURAL_NODE_GROUP_H
#define NEURAL_NODE_GROUP_H

#include <memory>
#include <vector>

#include <CL/cl.hpp>

class NeuralNodeGroup
{
public:
	NeuralNodeGroup(const int nodeCount);
	virtual ~NeuralNodeGroup();

	int getNodeCount() const;
	cl::Buffer getExcitationLevelsBuffer();
	cl::Buffer getExcitationStatesBuffer();

	void setExcitationLevels(std::vector<float> excitationLevels);
	std::vector<float> getExcitationLevels();

	void setExcitationStates(std::vector<float> excitationLevels);
	std::vector<float> getExcitationStates();

	virtual void update() = 0;

private:
	const int m_nodeCount;
	cl::Buffer m_excitationLevelsBuffer;
	cl::Buffer m_excitationStatesBuffer;
};


#endif // NEURAL_NODE_GROUP_H
