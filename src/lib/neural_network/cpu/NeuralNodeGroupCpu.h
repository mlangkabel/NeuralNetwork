#ifndef NEURAL_NODE_GROUP_CPU_H
#define NEURAL_NODE_GROUP_CPU_H

#include <memory>
#include <vector>

#include "utility/types.h"
#include "neural_network/NeuralNetworkConfiguration.h"

class NeuralNodeGroupCpu
{
public:
	static std::shared_ptr<NeuralNodeGroupCpu> create(const NeuralNodeGroupConfiguration& configuration);

	NeuralNodeGroupCpu(
		const Id id, const int nodeCount, float excitationOffset, float excitationMultiplier, float excitationThreshold, float excitationFatigue
	);

	Id getId() const;
	int getNodeCount() const;

	void setExcitationLevels(std::vector<float> excitationLevels);
	std::vector<float>& getExcitationLevels();

	void setExcitationStates(std::vector<float> excitationLevels);
	std::vector<float>& getExcitationStates();

	void update();

private:
	const Id m_id;
	const int m_nodeCount;
	const float m_excitationOffset;
	const float m_excitationMultiplier;
	const float m_excitationThreshold;
	const float m_excitationFatigue;

	std::vector<float> m_excitationLevels;
	std::vector<float> m_excitationStates;
};

#endif // NEURAL_NODE_GROUP_CPU_H
