#ifndef NEURAL_NETWORK_CPU_H
#define NEURAL_NETWORK_CPU_H

#include <memory>
#include <vector>

#include "utility/TextAccess.h"

class NeuralNodeGroupCpu;
class NeuralEdgeGroupCpu;

struct NeuralNetworkConfiguration;

class NeuralNetworkCpu
{
public:
	static std::shared_ptr<NeuralNetworkCpu> create(const NeuralNetworkConfiguration& configuration);

	void setInputExcitationLevels(std::vector<float> excitationLevels);
	std::vector<float> getOutputExcitationStates();

	void update();

private:
	NeuralNetworkCpu() = default;

	std::vector<std::shared_ptr<NeuralNodeGroupCpu>> m_nodeGroups;
	std::vector<std::shared_ptr<NeuralEdgeGroupCpu>> m_edgeGroups;

	std::shared_ptr<NeuralNodeGroupCpu> m_inputNodeGroup;
	std::shared_ptr<NeuralNodeGroupCpu> m_outputNodeGroup;
};

#endif // NEURAL_NETWORK_CPU_H
