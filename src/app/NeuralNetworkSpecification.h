#ifndef NEURAL_NETWORK_SPECIFICATION_H
#define NEURAL_NETWORK_SPECIFICATION_H

#include <string>

#include "utility/Matrix.h"

class TiXmlElement;

struct NeuralNetworkSpecification
{
	NeuralNetworkSpecification();
	NeuralNetworkSpecification(int inputNodeAmount, int hiddenNodeAmount, int outputNodeAmount);

	int inputNodeAmount;
	int hiddenNodeAmount;
	int outputNodeAmount;

	Matrix<float> inputToHiddenNodeWeights;
	Matrix<float> hiddenToHiddenNodeWeights;
	Matrix<float> hiddenToOutputNodeWeights;
};

NeuralNetworkSpecification createRandomNeuralNetworkSpecification(int hiddenNodeAmount);
TiXmlElement* neuralNetworkSpecificationToXml(const NeuralNetworkSpecification& genotype);
std::string neuralNetworkSpecificationToString(const NeuralNetworkSpecification& genotype);
//NeuralNetworkGenotype neuralNetworkGenotypeFromXml(const std::string& genotype);

float runPingEvaluation(NeuralNetworkSpecification genotype, bool verbose);
float runPingEvaluation(NeuralNetworkSpecification genotype, const int nunmberOfPings, bool verbose);
std::vector<float>& addNoise(std::vector<float>& vec);
std::vector<float> addNoise(const std::vector<float>& vec);

#endif // NEURAL_NETWORK_SPECIFICATION_H
