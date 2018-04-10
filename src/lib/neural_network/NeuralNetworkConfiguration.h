#ifndef NEURAL_NETWORK_CONFIGURATION_H
#define NEURAL_NETWORK_CONFIGURATION_H

#include <string>

#include "utility/Matrix.h"
#include "utility/types.h"

class TiXmlElement;

struct NeuralNodeGroupConfiguration
{
	Id id;
	int nodeCount;
	float excitationOffset;
	float excitationMultiplier;
	float excitationThreshold;
	float excitationFatigue;
};

struct NeuralEdgeGroupConfiguration
{
	NeuralEdgeGroupConfiguration(Id sourceGroupId, Id targetGroupId, Matrix<float> weights)
		: sourceGroupId(sourceGroupId)
		, targetGroupId(targetGroupId)
		, weights(weights)
	{}

	Id sourceGroupId;
	Id targetGroupId;
	Matrix<float> weights;
};

struct NeuralNetworkConfiguration
{
	NeuralNetworkConfiguration();

	Id inputGroupId;
	Id outputGroupId;
	std::vector<NeuralNodeGroupConfiguration> nodeGroups;
	std::vector<NeuralEdgeGroupConfiguration> edgeGroups;
};

TiXmlElement* matrixToXmlElement(const Matrix<float>& matrix);
Matrix<float> matrixFromXmlElement(const TiXmlElement* element);

TiXmlElement* neuralNodeGroupConfigurationToXmlElement(const NeuralNodeGroupConfiguration& configuration);
NeuralNodeGroupConfiguration neuralNodeGroupConfigurationFromXmlElement(const TiXmlElement* element);

TiXmlElement* neuralEdgeGroupConfigurationToXmlElement(const NeuralEdgeGroupConfiguration& configuration);
NeuralEdgeGroupConfiguration neuralEdgeGroupConfigurationFromXmlElement(const TiXmlElement* element);

TiXmlElement* neuralNetworkConfigurationToXmlElement(const NeuralNetworkConfiguration& configuration);
NeuralNetworkConfiguration neuralNetworkConfigurationFromXmlElement(const TiXmlElement* element);

#endif // NEURAL_NETWORK_CONFIGURATION_H
