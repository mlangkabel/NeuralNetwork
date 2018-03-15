#ifndef NEURAL_NODE_GROUP_STEP_EXCITATION_GPU_H
#define NEURAL_NODE_GROUP_STEP_EXCITATION_GPU_H

#include "neural_network/gpu/NeuralNodeGroupGpu.h"

class TiXmlElement;

class NeuralNodeGroupStepExcitationGpu : public NeuralNodeGroupGpu
{
public:
	static std::shared_ptr<NeuralNodeGroupStepExcitationGpu> loadFromXmlElement(const TiXmlElement* element);

	NeuralNodeGroupStepExcitationGpu(const Id id, const int nodeCount, const float excitationThreshold, const float excitationFatigue);

	float getExcitationThreshold() const;
	float getExcitationFatigue() const;

	virtual void update() override;

private:
	const float m_excitationThreshold;
	const float m_excitationFatigue;

	cl::Kernel m_kernel;
};

#endif // NEURAL_NODE_GROUP_STEP_EXCITATION_GPU_H
