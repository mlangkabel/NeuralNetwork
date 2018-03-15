#ifndef NEURAL_EDGE_GROUP_GPU_H
#define NEURAL_EDGE_GROUP_GPU_H

#include <memory>
#include <vector>

#include <CL/cl.hpp>

#include "utility/Matrix.h"

class TiXmlElement;
class NeuralNodeGroupGpu;

class NeuralEdgeGroupGpu
{
public:
	static std::shared_ptr<NeuralEdgeGroupGpu> loadFromXmlElement(const TiXmlElement* element, const std::vector<std::shared_ptr<NeuralNodeGroupGpu>>& nodeGroups);

	NeuralEdgeGroupGpu(
		std::shared_ptr<NeuralNodeGroupGpu> sourceNodes,
		std::shared_ptr<NeuralNodeGroupGpu> targetNodes
	);

	void setWeights(Matrix<float> weights);
	Matrix<float> getWeights();

	void update();

private:
	std::shared_ptr<NeuralNodeGroupGpu> m_sourceNodes;
	std::shared_ptr<NeuralNodeGroupGpu> m_targetNodes;
	cl::Kernel m_kernel;
	cl::Buffer m_weightsBuffer;
};


#endif // NEURAL_EDGE_GROUP_GPU_H
