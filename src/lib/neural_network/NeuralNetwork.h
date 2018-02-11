#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <memory>
#include <vector>

#include <CL/cl.hpp>

class NeuralNodeGroup;
class NeuralEdgeGroup;

class NeuralNetwork
{
public:
	NeuralNetwork();

private:
	std::vector<std::shared_ptr<NeuralNodeGroup>> m_nodeGroups;
	std::vector<std::shared_ptr<NeuralEdgeGroup>> m_edgeGroups;
};


#endif // NEURAL_NETWORK_H
