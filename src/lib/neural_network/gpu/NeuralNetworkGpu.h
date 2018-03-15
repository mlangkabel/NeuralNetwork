#ifndef NEURAL_NETWORK_GPU_H
#define NEURAL_NETWORK_GPU_H

#include <memory>
#include <vector>

#include "utility/TextAccess.h"

class NeuralNodeGroupGpu;
class NeuralEdgeGroupGpu;

class NeuralNetworkGpu
{
public:
	static std::shared_ptr<NeuralNetworkGpu> load(std::shared_ptr<const TextAccess> textAccess);

	void setInputExcitationLevels(std::vector<float> excitationLevels);
	std::vector<float> getOutputExcitationStates();

	void update();

private:
	NeuralNetworkGpu() = default;

	std::vector<std::shared_ptr<NeuralNodeGroupGpu>> m_nodeGroups;
	std::vector<std::shared_ptr<NeuralEdgeGroupGpu>> m_edgeGroups;

	std::shared_ptr<NeuralNodeGroupGpu> m_inputNodeGroup;
	std::shared_ptr<NeuralNodeGroupGpu> m_outputNodeGroup;
};


#endif // NEURAL_NETWORK_GPU_H
