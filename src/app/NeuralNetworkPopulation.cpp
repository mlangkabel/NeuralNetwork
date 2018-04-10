#include "NeuralNetworkPopulation.h"

#include "tinyxml/tinyxml.h"

TiXmlElement* neuralNetworkPopulationToXmlElement(const NeuralNetworkPopulation& population)
{
	TiXmlElement* elementPopulation = new TiXmlElement("population");
	elementPopulation->SetAttribute("fitness", std::to_string(population.fitness).c_str());
	elementPopulation->SetAttribute("hidden_node_count", std::to_string(population.hiddenNodeCount).c_str());
	for (const NeuralNetworkConfiguration& configuration : population.configurations)
	{
		elementPopulation->LinkEndChild(neuralNetworkConfigurationToXmlElement(configuration));
	}
	return elementPopulation;
}

NeuralNetworkPopulation neuralNetworkPopulationFromXmlElement(const TiXmlElement* element)
{
	if (!element)
	{
		throw std::logic_error("Xml element is null.");
	}

	NeuralNetworkPopulation population;

	if (element->QueryFloatAttribute("fitness", &population.fitness) != TIXML_SUCCESS)
	{
		throw std::logic_error("Unable to parse fitness of population.");
	}

	if (element->QueryIntAttribute("hidden_node_count", &population.hiddenNodeCount) != TIXML_SUCCESS)
	{
		throw std::logic_error("Unable to parse hidden_node_amount of population.");
	}

	const std::string elementNeuralNetworkName = "neural_network";
	const TiXmlElement* elementNeuralNetwork = element->FirstChildElement(elementNeuralNetworkName.c_str());
	while (elementNeuralNetwork)
	{
		population.configurations.push_back(neuralNetworkConfigurationFromXmlElement(elementNeuralNetwork));
		elementNeuralNetwork = elementNeuralNetwork->NextSiblingElement(elementNeuralNetworkName.c_str());
	}

	return population;
}
