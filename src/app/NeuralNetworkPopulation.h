#ifndef NEURAL_NETWORK_POPULATION_H
#define NEURAL_NETWORK_POPULATION_H

#include "neural_network/NeuralNetworkConfiguration.h"

class TiXmlElement;

struct NeuralNetworkPopulation
{
	float fitness;
	int hiddenNodeCount;
	std::vector<NeuralNetworkConfiguration> configurations;
};

TiXmlElement* neuralNetworkPopulationToXmlElement(const NeuralNetworkPopulation& population);
NeuralNetworkPopulation neuralNetworkPopulationFromXmlElement(const TiXmlElement* element);

#endif // NEURAL_NETWORK_POPULATION_H
