#ifndef NEURAL_NODE_GROUP_LINEAR_EXCITATION_GPU_H
#define NEURAL_NODE_GROUP_LINEAR_EXCITATION_GPU_H

#include "neural_network/gpu/NeuralNodeGroupGpu.h"

class TiXmlElement;

class NeuralNodeGroupLinearExcitationGpu : public NeuralNodeGroupGpu
{
public:
	static std::shared_ptr<NeuralNodeGroupLinearExcitationGpu> loadFromXmlElement(const TiXmlElement* element);

	NeuralNodeGroupLinearExcitationGpu(const Id id, const int nodeCount, const float excitationFactor);

	float getExcitationFactor() const;

	virtual void update() override;

private:
	const float m_excitationFactor;

	cl::Kernel m_kernel;
};

#endif // NEURAL_NODE_GROUP_LINEAR_EXCITATION_GPU_H
