#ifndef NEURAL_NODE_GROUP_LINEAR_EXCITATION_CPU_H
#define NEURAL_NODE_GROUP_LINEAR_EXCITATION_CPU_H

#include "neural_network/cpu/NeuralNodeGroupCpu.h"

class TiXmlElement;

class NeuralNodeGroupLinearExcitationCpu : public NeuralNodeGroupCpu
{
public:
	static std::shared_ptr<NeuralNodeGroupLinearExcitationCpu> loadFromXmlElement(const TiXmlElement* element);

	NeuralNodeGroupLinearExcitationCpu(const Id id, const int nodeCount, const float excitationFactor);

	float getExcitationFactor() const;

	virtual void update() override;

private:
	const float m_excitationFactor;
};

#endif // NEURAL_NODE_GROUP_LINEAR_EXCITATION_CPU_H
