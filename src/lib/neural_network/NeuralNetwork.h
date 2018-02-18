#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <memory>
#include <vector>

#include "utility/TextAccess.h"

class NeuralNodeGroup;
class NeuralEdgeGroup;

class NeuralNetwork
{
public:
	static std::shared_ptr<NeuralNetwork> load(std::shared_ptr<const TextAccess> textAccess);

private:
	NeuralNetwork();

	std::vector<std::shared_ptr<NeuralNodeGroup>> m_nodeGroups;
	std::vector<std::shared_ptr<NeuralEdgeGroup>> m_edgeGroups;
};


#endif // NEURAL_NETWORK_H
