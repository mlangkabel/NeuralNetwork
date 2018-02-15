#ifndef NEURAL_NODE_GROUP_STEP_EXCITATION_H
#define NEURAL_NODE_GROUP_STEP_EXCITATION_H

#include "neural_network/NeuralNodeGroup.h"

class NeuralNodeGroupStepExcitation: public NeuralNodeGroup
{
public:
	NeuralNodeGroupStepExcitation(const Id id, const int nodeCount, const float excitationThreshold, const float excitationFatigue);

	virtual void update() override;

private:
	const float m_excitationThreshold;
	const float m_excitationFatigue;

	cl::Kernel m_kernel;
};

#endif // NEURAL_NODE_GROUP_STEP_EXCITATION_H
