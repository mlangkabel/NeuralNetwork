#ifndef NEURAL_NETWORK_GENOTYPE_H
#define NEURAL_NETWORK_GENOTYPE_H

#include <string>

#include "utility/Matrix.h"

struct NeuralNetworkGenotype
{
	NeuralNetworkGenotype();
	NeuralNetworkGenotype(int inputNodeAmount, int hiddenNodeAmount, int outputNodeAmount);

	int inputNodeAmount;
	int hiddenNodeAmount;
	int outputNodeAmount;

	Matrix<float> inputToHiddenNodeWeights;
	Matrix<float> hiddenToHiddenNodeWeights;
	Matrix<float> hiddenToOutputNodeWeights;
};

NeuralNetworkGenotype createRandomNeuralNetworkGenotype(int hiddenNodeAmount);
std::string neuralNetworkGenotypeToXml(NeuralNetworkGenotype genotype);

float runPingEvaluation(NeuralNetworkGenotype genotype, bool verbose);
std::vector<float>& addNoise(std::vector<float>& vec);
std::vector<float> addNoise(const std::vector<float>& vec);

#endif // NEURAL_NETWORK_GENOTYPE_H
