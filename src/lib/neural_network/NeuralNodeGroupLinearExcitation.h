#ifndef NEURAL_NODE_GROUP_LINEAR_EXCITATION_H
#define NEURAL_NODE_GROUP_LINEAR_EXCITATION_H

#include "neural_network/NeuralNodeGroup.h"

class TiXmlElement;

class NeuralNodeGroupLinearExcitation: public NeuralNodeGroup
{
public:
	static std::shared_ptr<NeuralNodeGroupLinearExcitation> loadFromXmlElement(const TiXmlElement* element);

	NeuralNodeGroupLinearExcitation(const Id id, const int nodeCount, const float excitationFactor);

	float getExcitationFactor() const;

	virtual void update() override;

private:
	const float m_excitationFactor;

	cl::Kernel m_kernel;
};

#endif // NEURAL_NODE_GROUP_LINEAR_EXCITATION_H
