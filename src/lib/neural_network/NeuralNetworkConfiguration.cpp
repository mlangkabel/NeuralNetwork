#include "NeuralNetworkConfiguration.h"

#include "tinyxml/tinyxml.h"

NeuralNetworkConfiguration::NeuralNetworkConfiguration()
	: inputGroupId(0)
	, outputGroupId(0)
{
}

TiXmlElement* matrixToXmlElement(const Matrix<float>& matrix)
{
	TiXmlElement* elementMatrix = new TiXmlElement("matrix");
	elementMatrix->SetAttribute("width", matrix.getWidth());
	elementMatrix->SetAttribute("height", matrix.getHeight());
	elementMatrix->SetAttribute("values", matrix.getValuesAsString().c_str());
	return elementMatrix;
}

Matrix<float> matrixFromXmlElement(const TiXmlElement* element)
{
	if (!element)
	{
		throw std::logic_error("Xml element is null.");
	}

	int width;
	if (!element->QueryIntAttribute("width", &width) == TIXML_SUCCESS)
	{
		throw std::logic_error(std::string("Unable to parse \"width\" attribute of \"") + element->Value() + "\" xml element.");
	}

	int height;
	if (!element->QueryIntAttribute("height", &height) == TIXML_SUCCESS)
	{
		throw std::logic_error(std::string("Unable to parse \"height\" attribute of \"") + element->Value() + "\" xml element.");
	}

	Matrix<float> matrix(width, height);

	const std::string valuesString = element->Attribute("values");
	if (valuesString.empty())
	{
		throw std::logic_error(std::string("Unable to parse \"values\" attribute of \"") + element->Value() + "\" xml element.");
	}

	if (!matrix.setValuesFromString(valuesString))
	{
		throw std::logic_error(std::string("Unable to set matrix values from string \"") + valuesString + "\".");
	}

	return matrix;
}

TiXmlElement* neuralNodeGroupConfigurationToXmlElement(const NeuralNodeGroupConfiguration& configuration)
{
	TiXmlElement* elementNodeGroup = new TiXmlElement("node_group");
	elementNodeGroup->SetAttribute("id", configuration.id);
	elementNodeGroup->SetAttribute("node_count", configuration.nodeCount);
	elementNodeGroup->SetAttribute("excitation_offset", std::to_string(configuration.excitationOffset).c_str());
	elementNodeGroup->SetAttribute("excitation_multiplier", std::to_string(configuration.excitationMultiplier).c_str());
	elementNodeGroup->SetAttribute("excitation_threshold", std::to_string(configuration.excitationThreshold).c_str());
	elementNodeGroup->SetAttribute("excitation_fatigue", std::to_string(configuration.excitationFatigue).c_str());
	return elementNodeGroup;
}

NeuralNodeGroupConfiguration neuralNodeGroupConfigurationFromXmlElement(const TiXmlElement* element)
{
	if (!element)
	{
		throw std::logic_error("Xml element is null.");
	}

	NeuralNodeGroupConfiguration configuration;

	{
		int id;
		if (!element->QueryIntAttribute("id", &id) == TIXML_SUCCESS || id < 0)
		{
			throw std::logic_error(std::string("Unable to parse \"id\" attribute of \"") + element->Value() + "\" xml element.");
		}
		configuration.id = id;
	}
	{
		int nodeCount;
		if (!element->QueryIntAttribute("node_count", &nodeCount) == TIXML_SUCCESS || nodeCount <= 0)
		{
			throw std::logic_error(std::string("Unable to parse \"node_count\" attribute of \"") + element->Value() + "\" xml element.");
		}
		configuration.nodeCount = nodeCount;
	}
	{
		float excitationOffset;
		if (!element->QueryFloatAttribute("excitation_offset", &excitationOffset) == TIXML_SUCCESS)
		{
			throw std::logic_error(std::string("Unable to parse \"excitation_offset\" attribute of \"") + element->Value() + "\" xml element.");
		}
		configuration.excitationOffset = excitationOffset;
	}
	{
		float excitationMultiplyer;
		if (!element->QueryFloatAttribute("excitation_multiplier", &excitationMultiplyer) == TIXML_SUCCESS)
		{
			throw std::logic_error(std::string("Unable to parse \"excitation_multiplier\" attribute of \"") + element->Value() + "\" xml element.");
		}
		configuration.excitationMultiplier = excitationMultiplyer;
	}
	{
		float excitationThreshold;
		if (!element->QueryFloatAttribute("excitation_threshold", &excitationThreshold) == TIXML_SUCCESS)
		{
			throw std::logic_error(std::string("Unable to parse \"excitation_threshold\" attribute of \"") + element->Value() + "\" xml element.");
		}
		configuration.excitationThreshold = excitationThreshold;
	}
	{
		float excitationFatigue;
		if (!element->QueryFloatAttribute("excitation_fatigue", &excitationFatigue) == TIXML_SUCCESS)
		{
			throw std::logic_error(std::string("Unable to parse \"excitation_fatigue\" attribute of \"") + element->Value() + "\" xml element.");
		}
		configuration.excitationFatigue = excitationFatigue;
	}

	return configuration;
}

TiXmlElement* neuralEdgeGroupConfigurationToXmlElement(const NeuralEdgeGroupConfiguration& configuration)
{
	TiXmlElement* elementEdgeGroup = new TiXmlElement("edge_group");
	elementEdgeGroup->SetAttribute("source_group_id", configuration.sourceGroupId);
	elementEdgeGroup->SetAttribute("target_group_id", configuration.targetGroupId);
	TiXmlElement* elementWeights = matrixToXmlElement(configuration.weights);
	elementWeights->SetValue("weights");
	elementEdgeGroup->LinkEndChild(elementWeights);
	return elementEdgeGroup;
}

NeuralEdgeGroupConfiguration neuralEdgeGroupConfigurationFromXmlElement(const TiXmlElement* element)
{
	if (!element)
	{
		throw std::logic_error("Xml element is null.");
	}

	int sourceGroupId;
	if (!element->QueryIntAttribute("source_group_id", &sourceGroupId) == TIXML_SUCCESS || sourceGroupId < 0)
	{
		throw std::logic_error(std::string("Unable to parse \"source_group_id\" attribute of \"") + element->Value() + "\" xml element.");
	}

	int targetGroupId;
	if (!element->QueryIntAttribute("target_group_id", &targetGroupId) == TIXML_SUCCESS || targetGroupId < 0)
	{
		throw std::logic_error(std::string("Unable to parse \"target_group_id\" attribute of \"") + element->Value() + "\" xml element.");
	}

	return NeuralEdgeGroupConfiguration(sourceGroupId, targetGroupId, matrixFromXmlElement(element->FirstChildElement("weights")));
}

TiXmlElement* neuralNetworkConfigurationToXmlElement(const NeuralNetworkConfiguration& configuration)
{
	TiXmlElement* elementNeuralNetwork = new TiXmlElement("neural_network");

	elementNeuralNetwork->SetAttribute("input_node_group_id", configuration.inputGroupId);
	elementNeuralNetwork->SetAttribute("output_node_group_id", configuration.outputGroupId);

	for (const NeuralNodeGroupConfiguration& nodeGroup: configuration.nodeGroups)
	{
		elementNeuralNetwork->LinkEndChild(neuralNodeGroupConfigurationToXmlElement(nodeGroup));
	}
	
	for (const NeuralEdgeGroupConfiguration& edgeGroup : configuration.edgeGroups)
	{
		elementNeuralNetwork->LinkEndChild(neuralEdgeGroupConfigurationToXmlElement(edgeGroup));
	}

	return elementNeuralNetwork;
}

NeuralNetworkConfiguration neuralNetworkConfigurationFromXmlElement(const TiXmlElement* element)
{
	if (!element)
	{
		throw std::logic_error("Xml element is null.");
	}

	NeuralNetworkConfiguration configuration;

	{
		int inputNodeGroupId = -1;
		if (element->QueryIntAttribute("input_node_group_id", &inputNodeGroupId) != TIXML_SUCCESS)
		{
			throw std::logic_error("Unable to parse input node group id of neural network configuration.");
		}
		configuration.inputGroupId = inputNodeGroupId;
	}
	{
		int outputNodeGroupId = -1;
		if (element->QueryIntAttribute("output_node_group_id", &outputNodeGroupId) != TIXML_SUCCESS)
		{
			throw std::logic_error("Unable to parse output node group id of neural network configuration.");
		}
		configuration.outputGroupId = outputNodeGroupId;
	}
	{
		const std::string elementName = "node_group";
		const TiXmlElement* elementNodeGroup = element->FirstChildElement(elementName.c_str());
		while (elementNodeGroup)
		{
			configuration.nodeGroups.push_back(neuralNodeGroupConfigurationFromXmlElement(elementNodeGroup));
			elementNodeGroup = elementNodeGroup->NextSiblingElement(elementName.c_str());
		}
	}

	{
		const std::string elementName = "edge_group";
		const TiXmlElement* elementEdgeGroup = element->FirstChildElement(elementName.c_str());
		while (elementEdgeGroup)
		{
			configuration.edgeGroups.push_back(neuralEdgeGroupConfigurationFromXmlElement(elementEdgeGroup));
			elementEdgeGroup = elementEdgeGroup->NextSiblingElement(elementName.c_str());
		}
	}

	return configuration;
}
