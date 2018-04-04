#ifndef NEURAL_NETWORK_CONFIGURATION_H
#define NEURAL_NETWORK_CONFIGURATION_H

#include <string>

#include "utility/Matrix.h"

class TiXmlElement;

struct NeuralNetworkConfiguration
{
	NeuralNetworkConfiguration();
	NeuralNetworkConfiguration(int inputNodeAmount, int hiddenNodeAmount, int outputNodeAmount);

	int inputNodeAmount;
	int hiddenNodeAmount;
	int outputNodeAmount;

	Matrix<float> inputToHiddenNodeWeights;
	Matrix<float> hiddenToHiddenNodeWeights;
	Matrix<float> hiddenToOutputNodeWeights;
};

NeuralNetworkConfiguration createRandomNeuralNetworkConfiguration(int hiddenNodeAmount);
TiXmlElement* neuralNetworkConfigurationToXml(const NeuralNetworkConfiguration& configuration);
std::string neuralNetworkConfigurationToString(const NeuralNetworkConfiguration& configuration);
//NeuralNetworkGenotype neuralNetworkGenotypeFromXml(const std::string& genotype);

float runPingEvaluation(NeuralNetworkConfiguration genotype, bool verbose);
float runPingEvaluation(NeuralNetworkConfiguration genotype, const int nunmberOfPings, bool verbose);
std::vector<float>& addNoise(std::vector<float>& vec);
std::vector<float> addNoise(const std::vector<float>& vec);

#endif // NEURAL_NETWORK_CONFIGURATION_H
