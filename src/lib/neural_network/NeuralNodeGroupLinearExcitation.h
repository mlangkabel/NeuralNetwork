#ifndef NEURAL_NODE_GROUP_LINEAR_EXCITATION_H
#define NEURAL_NODE_GROUP_LINEAR_EXCITATION_H

#include "neural_network/NeuralNodeGroup.h"

class NeuralNodeGroupLinearExcitation: public NeuralNodeGroup
{
public:
	NeuralNodeGroupLinearExcitation(const int nodeCount, const float excitationFactor = 1.0f);

	virtual void update() override;

private:
	const float m_excitationFactor;

	cl::Kernel m_kernel;
};

#endif // NEURAL_NODE_GROUP_LINEAR_EXCITATION_H
