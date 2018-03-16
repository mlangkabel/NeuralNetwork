#ifndef NEURAL_NODE_GROUP_CPU_H
#define NEURAL_NODE_GROUP_CPU_H

#include <memory>
#include <vector>

#include "utility/types.h"

class NeuralNodeGroupCpu
{
public:
	NeuralNodeGroupCpu(const Id id, const int nodeCount);
	virtual ~NeuralNodeGroupCpu() = default;

	Id getId() const;
	int getNodeCount() const;

	void setExcitationLevels(std::vector<float> excitationLevels);
	std::vector<float>& getExcitationLevels();

	void setExcitationStates(std::vector<float> excitationLevels);
	std::vector<float>& getExcitationStates();

	virtual void update() = 0;

protected:
	std::vector<float> m_excitationLevels;
	std::vector<float> m_excitationStates;

private:
	const Id m_id;
	const int m_nodeCount;
};


#endif // NEURAL_NODE_GROUP_CPU_H
