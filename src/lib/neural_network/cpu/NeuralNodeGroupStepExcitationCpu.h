#ifndef NEURAL_NODE_GROUP_STEP_EXCITATION_CPU_H
#define NEURAL_NODE_GROUP_STEP_EXCITATION_CPU_H

#include "neural_network/cpu/NeuralNodeGroupCpu.h"

class TiXmlElement;

class NeuralNodeGroupStepExcitationCpu : public NeuralNodeGroupCpu
{
public:
	static std::shared_ptr<NeuralNodeGroupStepExcitationCpu> loadFromXmlElement(const TiXmlElement* element);

	NeuralNodeGroupStepExcitationCpu(const Id id, const int nodeCount, const float excitationThreshold, const float excitationFatigue);

	float getExcitationThreshold() const;
	float getExcitationFatigue() const;

	virtual void update() override;

private:
	const float m_excitationThreshold;
	const float m_excitationFatigue;
};

#endif // NEURAL_NODE_GROUP_STEP_EXCITATION_CPU_H
