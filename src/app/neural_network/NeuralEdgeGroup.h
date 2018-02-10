#ifndef NEURAL_EDGE_GROUP_H
#define NEURAL_EDGE_GROUP_H

#include <memory>
#include <vector>

#include <CL/cl.hpp>

#include "utility/Matrix.h"

class NeuralNodeGroup;

class NeuralEdgeGroup
{
public:
	NeuralEdgeGroup(
		std::shared_ptr<NeuralNodeGroup> sourceNodes, 
		std::shared_ptr<NeuralNodeGroup> targetNodes, 
		cl::Device device,
		cl::Context context,
		cl::CommandQueue queue
	);

	void setWeights(Matrix<float> weights);
	Matrix<float> getWeights();

	void update();

private:
	std::shared_ptr<NeuralNodeGroup> m_sourceNodes;
	std::shared_ptr<NeuralNodeGroup> m_targetNodes;
	cl::Device m_device;
	cl::Context m_context;
	cl::CommandQueue m_queue;
	cl::Buffer m_weightsBuffer;
};


#endif // NEURAL_EDGE_GROUP_H
