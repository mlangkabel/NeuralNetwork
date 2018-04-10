#ifndef NEURAL_EDGE_GROUP_CPU_H
#define NEURAL_EDGE_GROUP_CPU_H

#include <memory>
#include <vector>

#include "utility/Matrix.h"

class TiXmlElement;
class NeuralNodeGroupCpu;

struct NeuralEdgeGroupConfiguration;

class NeuralEdgeGroupCpu
{
public:
	static std::shared_ptr<NeuralEdgeGroupCpu> create(
		const NeuralEdgeGroupConfiguration& configuration,
		const std::vector<std::shared_ptr<NeuralNodeGroupCpu>>& nodeGroups
	);

	NeuralEdgeGroupCpu(
		std::shared_ptr<NeuralNodeGroupCpu> sourceNodes,
		std::shared_ptr<NeuralNodeGroupCpu> targetNodes
	);

	void setWeights(Matrix<float> weights);
	Matrix<float> getWeights() const;

	void update();

private:
	std::shared_ptr<NeuralNodeGroupCpu> m_sourceNodes;
	std::shared_ptr<NeuralNodeGroupCpu> m_targetNodes;
	Matrix<float> m_weights;
};


#endif // NEURAL_EDGE_GROUP_CPU_H
