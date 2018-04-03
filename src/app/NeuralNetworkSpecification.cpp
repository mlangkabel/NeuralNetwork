#include "NeuralNetworkSpecification.h"

#include "tinyxml/tinyxml.h"

#include "neural_network/cpu/NeuralNetworkCpu.h"
#include "utility/random/RandomNumberGenerator.h"
#include "utility/logging.h"
#include "utility/utilityRandom.h"

typedef NeuralNetworkCpu NeuralNetwork;

NeuralNetworkSpecification::NeuralNetworkSpecification()
	: inputNodeAmount(0)
	, hiddenNodeAmount(0)
	, outputNodeAmount(0)
	, inputToHiddenNodeWeights(0, 0)
	, hiddenToHiddenNodeWeights(0, 0)
	, hiddenToOutputNodeWeights(0, 0)
{
}

NeuralNetworkSpecification::NeuralNetworkSpecification(int inputNodeAmount, int hiddenNodeAmount, int outputNodeAmount)
	: inputNodeAmount(inputNodeAmount)
	, hiddenNodeAmount(hiddenNodeAmount)
	, outputNodeAmount(outputNodeAmount)
	, inputToHiddenNodeWeights(inputNodeAmount, hiddenNodeAmount)
	, hiddenToHiddenNodeWeights(hiddenNodeAmount, hiddenNodeAmount)
	, hiddenToOutputNodeWeights(hiddenNodeAmount, outputNodeAmount)
{
}

NeuralNetworkSpecification createRandomNeuralNetworkSpecification(int hiddenNodeAmount)
{
	const int inputNodeAmount = 3;
	const int outputNodeAmount = 5;

	NeuralNetworkSpecification specification(inputNodeAmount, hiddenNodeAmount, outputNodeAmount);

	for (int y = 0; y < specification.inputToHiddenNodeWeights.getHeight() ; y++)
	{
		for (int x = 0; x < specification.inputToHiddenNodeWeights.getWidth(); x++)
		{
			specification.inputToHiddenNodeWeights.setValue(x, y, utility::getRandomFloat(-1.0f, 1.0f));
		}
	}

	for (int y = 0; y < specification.hiddenToHiddenNodeWeights.getHeight(); y++)
	{
		for (int x = 0; x < specification.hiddenToHiddenNodeWeights.getWidth(); x++)
		{
			specification.hiddenToHiddenNodeWeights.setValue(x, y, utility::getRandomFloat(-1.0f, 1.0f));
		}
	}

	for (int y = 0; y < specification.hiddenToOutputNodeWeights.getHeight(); y++)
	{
		for (int x = 0; x < specification.hiddenToOutputNodeWeights.getWidth(); x++)
		{
			specification.hiddenToOutputNodeWeights.setValue(x, y, utility::getRandomFloat(-1.0f, 1.0f));
		}
	}

	return specification;
}

TiXmlElement* neuralNetworkSpecificationToXml(const NeuralNetworkSpecification& genotype)
{
	TiXmlElement* elementNeuralNetwork = new TiXmlElement("neural_network");
	elementNeuralNetwork->SetAttribute("input_node_group_id", 0);
	elementNeuralNetwork->SetAttribute("output_node_group_id", 2);

	{
		TiXmlElement* elementNodeGroup = new TiXmlElement("node_group_linear_excitation");
		elementNodeGroup->SetAttribute("id", 0);
		elementNodeGroup->SetAttribute("node_count", genotype.inputNodeAmount);
		elementNodeGroup->SetAttribute("excitation_factor", "1.0");
		elementNeuralNetwork->LinkEndChild(elementNodeGroup);
	}
	{
		TiXmlElement* elementNodeGroup = new TiXmlElement("node_group_step_excitation");
		elementNodeGroup->SetAttribute("id", 1);
		elementNodeGroup->SetAttribute("node_count", genotype.hiddenNodeAmount);
		elementNodeGroup->SetAttribute("excitation_threshold", "0.5");
		elementNodeGroup->SetAttribute("excitation_fatigue", "0.5");
		elementNeuralNetwork->LinkEndChild(elementNodeGroup);
	}
	{
		TiXmlElement* elementNodeGroup = new TiXmlElement("node_group_step_excitation");
		elementNodeGroup->SetAttribute("id", 2);
		elementNodeGroup->SetAttribute("node_count", genotype.outputNodeAmount);
		elementNodeGroup->SetAttribute("excitation_threshold", "0.5");
		elementNodeGroup->SetAttribute("excitation_fatigue", "0.1");
		elementNeuralNetwork->LinkEndChild(elementNodeGroup);
	}
	{
		TiXmlElement* elementEdgeGroup = new TiXmlElement("edge_group");
		elementEdgeGroup->SetAttribute("source_group_id", 0);
		elementEdgeGroup->SetAttribute("target_group_id", 1);
		elementEdgeGroup->SetAttribute("weights", genotype.inputToHiddenNodeWeights.getValuesAsString().c_str());
		elementNeuralNetwork->LinkEndChild(elementEdgeGroup);
	}
	{
		TiXmlElement* elementEdgeGroup = new TiXmlElement("edge_group");
		elementEdgeGroup->SetAttribute("source_group_id", 1);
		elementEdgeGroup->SetAttribute("target_group_id", 1);
		elementEdgeGroup->SetAttribute("weights", genotype.hiddenToHiddenNodeWeights.getValuesAsString().c_str());
		elementNeuralNetwork->LinkEndChild(elementEdgeGroup);
	}
	{
		TiXmlElement* elementEdgeGroup = new TiXmlElement("edge_group");
		elementEdgeGroup->SetAttribute("source_group_id", 1);
		elementEdgeGroup->SetAttribute("target_group_id", 2);
		elementEdgeGroup->SetAttribute("weights", genotype.hiddenToOutputNodeWeights.getValuesAsString().c_str());
		elementNeuralNetwork->LinkEndChild(elementEdgeGroup);
	}

	return elementNeuralNetwork;
}

std::string neuralNetworkSpecificationToString(const NeuralNetworkSpecification& genotype)
{
	TiXmlDocument doc;
	doc.LinkEndChild(neuralNetworkSpecificationToXml(genotype));

	TiXmlPrinter printer;
	doc.Accept(&printer);

	return printer.CStr();
}

//NeuralNetworkGenotype neuralNetworkGenotypeFromXml(const std::string& xml)
//{
//	TiXmlDocument doc;
//	doc.Parse(
//		xml.c_str(),
//		0,
//		TIXML_ENCODING_UTF8
//	);
//
//	const TiXmlElement* elementNeuralNetwork = doc.FirstChildElement("neural_network");
//
//	if (!elementNeuralNetwork)
//	{
//		return std::shared_ptr<NeuralNetworkCpu>();
//	}
//
//	std::shared_ptr<NeuralNetworkCpu> neuralNetwork = std::shared_ptr<NeuralNetworkCpu>(new NeuralNetworkCpu());
//
//	{
//		const std::string elementName = "node_group_step_excitation";
//		const TiXmlElement* elementNodeGroup = elementNeuralNetwork->FirstChildElement(elementName.c_str());
//		while (elementNodeGroup)
//		{
//			std::shared_ptr<NeuralNodeGroupCpu> nodeGroup = NeuralNodeGroupStepExcitationCpu::loadFromXmlElement(elementNodeGroup);
//			if (!nodeGroup)
//			{
//				return std::shared_ptr<NeuralNetworkCpu>();
//			}
//			neuralNetwork->m_nodeGroups.push_back(nodeGroup);
//			elementNodeGroup = elementNodeGroup->NextSiblingElement(elementName.c_str());
//		}
//	}
//
//	{
//		const std::string elementName = "node_group_linear_excitation";
//		const TiXmlElement* elementNodeGroup = elementNeuralNetwork->FirstChildElement(elementName.c_str());
//		while (elementNodeGroup)
//		{
//			std::shared_ptr<NeuralNodeGroupCpu> nodeGroup = NeuralNodeGroupLinearExcitationCpu::loadFromXmlElement(elementNodeGroup);
//			if (!nodeGroup)
//			{
//				return std::shared_ptr<NeuralNetworkCpu>();
//			}
//			neuralNetwork->m_nodeGroups.push_back(nodeGroup);
//			elementNodeGroup = elementNodeGroup->NextSiblingElement(elementName.c_str());
//		}
//	}
//
//	{
//		int inputNodeGroupId = -1;
//		if (elementNeuralNetwork->QueryIntAttribute("input_node_group_id", &inputNodeGroupId) != TIXML_SUCCESS)
//		{
//			return std::shared_ptr<NeuralNetworkCpu>();
//		}
//		for (std::shared_ptr<NeuralNodeGroupCpu> nodeGroup : neuralNetwork->m_nodeGroups)
//		{
//			if (nodeGroup->getId() == inputNodeGroupId)
//			{
//				neuralNetwork->m_inputNodeGroup = nodeGroup;
//				break;
//			}
//		}
//		if (!neuralNetwork->m_inputNodeGroup)
//		{
//			return std::shared_ptr<NeuralNetworkCpu>();
//		}
//	}
//
//	{
//		int outputNodeGroupId = -1;
//		if (elementNeuralNetwork->QueryIntAttribute("output_node_group_id", &outputNodeGroupId) != TIXML_SUCCESS)
//		{
//			return std::shared_ptr<NeuralNetworkCpu>();
//		}
//		for (std::shared_ptr<NeuralNodeGroupCpu> nodeGroup : neuralNetwork->m_nodeGroups)
//		{
//			if (nodeGroup->getId() == outputNodeGroupId)
//			{
//				neuralNetwork->m_outputNodeGroup = nodeGroup;
//				break;
//			}
//		}
//		if (!neuralNetwork->m_outputNodeGroup)
//		{
//			return std::shared_ptr<NeuralNetworkCpu>();
//		}
//	}
//
//	{
//		const std::string elementName = "edge_group";
//		const TiXmlElement* elementEdgeGroup = elementNeuralNetwork->FirstChildElement(elementName.c_str());
//		while (elementEdgeGroup)
//		{
//			std::shared_ptr<NeuralEdgeGroupCpu> edgeGroup = NeuralEdgeGroupCpu::loadFromXmlElement(elementEdgeGroup, neuralNetwork->m_nodeGroups);
//			if (!edgeGroup)
//			{
//				return std::shared_ptr<NeuralNetworkCpu>();
//			}
//			neuralNetwork->m_edgeGroups.push_back(edgeGroup);
//			elementEdgeGroup = elementEdgeGroup->NextSiblingElement(elementName.c_str());
//		}
//	}
//
//	return neuralNetwork;
//}

float runPingEvaluation(NeuralNetworkSpecification genotype, bool verbose)
{
	return runPingEvaluation(genotype, utility::getRandomInt(0, genotype.outputNodeAmount - 1), verbose);
}

float runPingEvaluation(NeuralNetworkSpecification genotype, const int nunmberOfPings, bool verbose)
{
	if (verbose)
	{
		LOG_INFO("Running Ping Evaluation");
	}

	if (verbose)
	{
		LOG_INFO("Conducted number of pings: " + std::to_string(nunmberOfPings));
	}

	std::shared_ptr<NeuralNetwork> neuralNetwork = NeuralNetwork::load(TextAccess::createFromString(neuralNetworkSpecificationToString(genotype)));

	if (!neuralNetwork)
	{
		LOG_ERROR("Failed to instantiate Neural Network.");
		return 0.0f;
	}

	for (int i = 0; i < nunmberOfPings; i++)
	{
		const int silenceDuration = utility::getRandomInt(10, 30);
		const int pingDuration = utility::getRandomInt(10, 20);

		if (verbose)
		{
			LOG_INFO("Silence frames: " + std::to_string(silenceDuration));
			LOG_INFO("Ping frames:    " + std::to_string(pingDuration));
		}

		for (int j = 0; j < silenceDuration; j++)
		{
			neuralNetwork->setInputExcitationLevels(addNoise({ 0.0f, 0.0f, 1.0f }));
			neuralNetwork->update();
		}

		for (int j = 0; j < pingDuration; j++)
		{
			neuralNetwork->setInputExcitationLevels(addNoise({ 1.0f, 0.0f, 1.0f }));
			neuralNetwork->update();
		}
	}

	{
		const int silenceDuration = utility::getRandomInt(10, 30);

		if (verbose)
		{
			LOG_INFO("Silence frames: " + std::to_string(silenceDuration));
		}

		for (int j = 0; j < silenceDuration; j++)
		{
			neuralNetwork->setInputExcitationLevels(addNoise({ 0.0f, 0.0f, 1.0f }));
			neuralNetwork->update();
		}
	}
	
	float score = 0.0f;

	{
		const int answerDuration = utility::getRandomInt(20, 50);

		if (verbose)
		{
			LOG_INFO("Answer frames:  " + std::to_string(answerDuration));
		}

		for (int j = 0; j < answerDuration; j++)
		{
			neuralNetwork->setInputExcitationLevels(addNoise({ 0.0f, 1.0f, 1.0f }));
			neuralNetwork->update();
			const std::vector<float> output = neuralNetwork->getOutputExcitationStates();

			int answer = -1;
			float answerExcitationState = -std::numeric_limits<float>::infinity();
			for (int i = 0; i < output.size(); i++)
			{
				if (output[i] > answerExcitationState)
				{
					answer = i;
					answerExcitationState = output[i];
				}
			}

			//if (verbose)
			//{
			//	std::string visualAnswer = "";
			//	for (int i = 0; i < output.size(); i++)
			//	{
			//		if (i == answer)
			//		{
			//			visualAnswer += "[x]";
			//		}
			//		else
			//		{
			//			visualAnswer += "[o]";
			//		}
			//	}
			//	LOG_INFO("Answer: " + visualAnswer);
			//}

			if (answer == nunmberOfPings)
			{
				score += 1.0f;
			}
		}
		score /= answerDuration;
	}


	if (verbose)
	{
		LOG_INFO("Score: " + std::to_string(score));
	}

	return score;
}

std::vector<float>& addNoise(std::vector<float>& vec)
{
	const float noiseMagniture = 0.1f;

	for (float& val : vec)
	{
		val += utility::getRandomFloat(-noiseMagniture, noiseMagniture);
	}
	return vec;
}

std::vector<float> addNoise(const std::vector<float>& vec)
{
	const float noiseMagniture = 0.1f;
	std::vector<float> ret = vec;
	for (float& val : ret)
	{
		val += utility::getRandomFloat(-noiseMagniture, noiseMagniture);
	}
	return vec;
}

