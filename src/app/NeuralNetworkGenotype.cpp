#include "NeuralNetworkGenotype.h"

#include "neural_network/cpu/NeuralNetworkCpu.h"
#include "utility/random/RandomNumberGenerator.h"
#include "utility/logging.h"
#include "utility/utilityRandom.h"

typedef NeuralNetworkCpu NeuralNetwork;

NeuralNetworkGenotype::NeuralNetworkGenotype()
	: inputNodeAmount(0)
	, hiddenNodeAmount(0)
	, outputNodeAmount(0)
	, inputToHiddenNodeWeights(0, 0)
	, hiddenToHiddenNodeWeights(0, 0)
	, hiddenToOutputNodeWeights(0, 0)
{
}

NeuralNetworkGenotype::NeuralNetworkGenotype(int inputNodeAmount, int hiddenNodeAmount, int outputNodeAmount)
	: inputNodeAmount(inputNodeAmount)
	, hiddenNodeAmount(hiddenNodeAmount)
	, outputNodeAmount(outputNodeAmount)
	, inputToHiddenNodeWeights(inputNodeAmount, hiddenNodeAmount)
	, hiddenToHiddenNodeWeights(hiddenNodeAmount, hiddenNodeAmount)
	, hiddenToOutputNodeWeights(hiddenNodeAmount, outputNodeAmount)
{
}

NeuralNetworkGenotype createRandomNeuralNetworkGenotype(int hiddenNodeAmount)
{
	const int inputNodeAmount = 3;
	const int outputNodeAmount = 5;

	NeuralNetworkGenotype genotype(inputNodeAmount, hiddenNodeAmount, outputNodeAmount);

	for (int y = 0; y < genotype.inputToHiddenNodeWeights.getHeight() ; y++)
	{
		for (int x = 0; x < genotype.inputToHiddenNodeWeights.getWidth(); x++)
		{
			genotype.inputToHiddenNodeWeights.setValue(x, y, utility::getRandomFloat(-1.0f, 1.0f));
		}
	}

	for (int y = 0; y < genotype.hiddenToHiddenNodeWeights.getHeight(); y++)
	{
		for (int x = 0; x < genotype.hiddenToHiddenNodeWeights.getWidth(); x++)
		{
			genotype.hiddenToHiddenNodeWeights.setValue(x, y, utility::getRandomFloat(-1.0f, 1.0f));
		}
	}

	for (int y = 0; y < genotype.hiddenToOutputNodeWeights.getHeight(); y++)
	{
		for (int x = 0; x < genotype.hiddenToOutputNodeWeights.getWidth(); x++)
		{
			genotype.hiddenToOutputNodeWeights.setValue(x, y, utility::getRandomFloat(-1.0f, 1.0f));
		}
	}

	return genotype;
}

std::string neuralNetworkGenotypeToXml(NeuralNetworkGenotype genotype)
{
	std::string result;

	result += "<neural_network input_node_group_id=\"0\" output_node_group_id=\"2\">\n";

	result += "	<node_group_linear_excitation id=\"0\" node_count=\"" + std::to_string(genotype.inputNodeAmount) + "\" excitation_factor=\"1.0\" />";
	result += "	<node_group_step_excitation id=\"1\" node_count=\"" + std::to_string(genotype.hiddenNodeAmount) + "\" excitation_threshold=\"0.5\" excitation_fatigue=\"0.5\" />\n";
	result += "	<node_group_step_excitation id=\"2\" node_count=\"" + std::to_string(genotype.outputNodeAmount) + "\" excitation_threshold=\"0.5\" excitation_fatigue=\"0.1\" />\n";
	
	result += "	<edge_group source_group_id=\"0\" target_group_id=\"1\" weights=\"" + genotype.inputToHiddenNodeWeights.getValuesAsString() + "\"/>\n";
	result += "	<edge_group source_group_id=\"1\" target_group_id=\"1\" weights=\"" + genotype.hiddenToHiddenNodeWeights.getValuesAsString() + "\"/>\n";
	result += "	<edge_group source_group_id=\"1\" target_group_id=\"2\" weights=\"" + genotype.hiddenToOutputNodeWeights.getValuesAsString() + "\"/>\n";

	result += "</neural_network>\n";

	return result;
}

float runPingEvaluation(NeuralNetworkGenotype genotype, bool verbose)
{
	return runPingEvaluation(genotype, utility::getRandomInt(0, genotype.outputNodeAmount - 1), verbose);
}

float runPingEvaluation(NeuralNetworkGenotype genotype, const int nunmberOfPings, bool verbose)
{
	if (verbose)
	{
		LOG_INFO("Running Ping Evaluation");
	}

	if (verbose)
	{
		LOG_INFO("Conducted number of pings: " + std::to_string(nunmberOfPings));
	}

	std::shared_ptr<NeuralNetwork> neuralNetwork = NeuralNetwork::load(TextAccess::createFromString(neuralNetworkGenotypeToXml(genotype)));

	if (!neuralNetwork)
	{
		LOG_ERROR("Failed to instantiate Neural Network.");
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

