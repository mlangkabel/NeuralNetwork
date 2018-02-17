#ifndef NEURAL_EDGE_GROUP_H
#define NEURAL_EDGE_GROUP_H

#include <memory>
#include <vector>

#include <CL/cl.hpp>

#include "utility/Matrix.h"

class TiXmlElement;
class NeuralNodeGroup;

class NeuralEdgeGroup
{
public:
	static std::shared_ptr<NeuralEdgeGroup> loadFromXmlElement(const TiXmlElement* element, const std::vector<std::shared_ptr<NeuralNodeGroup>>& nodeGroups);
	NeuralEdgeGroup(
		std::shared_ptr<NeuralNodeGroup> sourceNodes, 
		std::shared_ptr<NeuralNodeGroup> targetNodes
	);

	void setWeights(Matrix<float> weights);
	Matrix<float> getWeights();

	void update();

private:
	std::shared_ptr<NeuralNodeGroup> m_sourceNodes;
	std::shared_ptr<NeuralNodeGroup> m_targetNodes;
	cl::Kernel m_kernel;
	cl::Buffer m_weightsBuffer;
};


#endif // NEURAL_EDGE_GROUP_H
