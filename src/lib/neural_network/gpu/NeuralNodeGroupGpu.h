#ifndef NEURAL_NODE_GROUP_GPU_H
#define NEURAL_NODE_GROUP_GPU_H

#include <memory>
#include <vector>

#include <CL/cl.hpp>

#include "utility/types.h"

class NeuralNodeGroupGpu
{
public:
	NeuralNodeGroupGpu(const Id id, const int nodeCount);
	virtual ~NeuralNodeGroupGpu() = default;

	Id getId() const;
	int getNodeCount() const;

	cl::Buffer getExcitationLevelsBuffer();
	cl::Buffer getExcitationStatesBuffer();

	void setExcitationLevels(std::vector<float> excitationLevels);
	std::vector<float> getExcitationLevels();

	void setExcitationStates(std::vector<float> excitationLevels);
	std::vector<float> getExcitationStates();

	virtual void update() = 0;

private:
	const Id m_id;
	const int m_nodeCount;
	cl::Buffer m_excitationLevelsBuffer;
	cl::Buffer m_excitationStatesBuffer;
};


#endif // NEURAL_NODE_GROUP_GPU_H
