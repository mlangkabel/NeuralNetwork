#include "NeuroEvolutionEnvironment.h"

#include "neural_network/cpu/NeuralNetworkCpu.h"
#include "utility/random/WeightedRandomGenerator.h"
#include "utility/utilityRandom.h"
#include "utility/logging.h"

typedef NeuralNetworkCpu NeuralNetwork;

const int INPUT_NODE_COUNT = 3;
const int OUTPUT_NODE_COUNT = 5;

NeuroEvolutionEnvironment::NeuroEvolutionEnvironment(int populatioSize, int offspringSize, float crossoverProbability)
	: EvolutionEnvironment(populatioSize, offspringSize, crossoverProbability)
{
}

NeuralNetworkConfiguration NeuroEvolutionEnvironment::crossover(const NeuralNetworkConfiguration& genotype1, const NeuralNetworkConfiguration& genotype2)
{
	const float minInfluence = 0.2f;

	NeuralNetworkConfiguration mergedGenotype = genotype1;

	for (int j = 0; j < mergedGenotype.edgeGroups.size(); j++)
	{
		Matrix<float>& weights = mergedGenotype.edgeGroups[j].weights;
		for (int i = 0; i < weights.getElementCount(); i++)
		{
			const float preferabilityGenotype1 = utility::getRandomFloat(minInfluence, 1.0f - minInfluence);
			const float preferabilityGenotype2 = 1.0f - preferabilityGenotype1;

			weights[i] =
				genotype1.edgeGroups[j].weights[i] * preferabilityGenotype1 +
				genotype2.edgeGroups[j].weights[i] * preferabilityGenotype2;
		}
	}

	return mergedGenotype;
}

NeuralNetworkConfiguration NeuroEvolutionEnvironment::mutate(const NeuralNetworkConfiguration& genotype)
{
	WeightedRandomGenerator<int> countRng(utility::getRandomInt(0, 10000));
	countRng.addResult(1, 1.0f / 2.0f);
	countRng.addResult(2, 1.0f / 4.0f);
	countRng.addResult(3, 1.0f / 8.0f);
	countRng.addResult(4, 1.0f / 16.0f);
	countRng.addResult(5, 1.0f / 32.0f);

	const int mutationCount = countRng.getResult();
	NeuralNetworkConfiguration mutatedGenotype = genotype;

	WeightedRandomGenerator<Matrix<float>*> weightsRng(utility::getRandomInt(0, 10000));
	for (int j = 0; j < mutatedGenotype.edgeGroups.size(); j++)
	{
		weightsRng.addResult(&mutatedGenotype.edgeGroups[j].weights, mutatedGenotype.edgeGroups[j].weights.getElementCount());
	}

	for (int i = 0; i < mutationCount; i++)
	{
		Matrix<float>* weights = weightsRng.getResult();

		const int indexToMutate = utility::getRandomInt(0, weights->getElementCount() - 1);
		(*weights)[indexToMutate] += utility::getRandomFloat(-0.1f, 0.1f);
	}

	return mutatedGenotype;
}

float NeuroEvolutionEnvironment::evaluate(const NeuralNetworkConfiguration& genotype)
{
	const int repetitions = 3;
	float score = 0.0f;
	for (int i = 0; i < repetitions; i++)
	{
		score += runPingEvaluation(genotype, 0, false);
		score += runPingEvaluation(genotype, 1, false);
		score += runPingEvaluation(genotype, 2, false);
		score += runPingEvaluation(genotype, 3, false);
		score += runPingEvaluation(genotype, 4, false);
	}
	return score / repetitions / 5.0f;
}

NeuralNetworkConfiguration createRandomNeuralNetworkConfiguration(int hiddenNodeAmount)
{
	NeuralNetworkConfiguration configuration;
	configuration.inputGroupId = 0;
	configuration.outputGroupId = 2;

	{
		NeuralNodeGroupConfiguration nodeGroupConfig;
		nodeGroupConfig.id = 0;
		nodeGroupConfig.nodeCount = INPUT_NODE_COUNT;
		nodeGroupConfig.excitationOffset = 0.0f;
		nodeGroupConfig.excitationMultiplier = 1.0f;
		nodeGroupConfig.excitationThreshold = 0.0f;
		nodeGroupConfig.excitationFatigue = 0.0f;
		configuration.nodeGroups.push_back(nodeGroupConfig);
	}
	{
		NeuralNodeGroupConfiguration nodeGroupConfig;
		nodeGroupConfig.id = 1;
		nodeGroupConfig.nodeCount = hiddenNodeAmount;
		nodeGroupConfig.excitationOffset = 1.0f;
		nodeGroupConfig.excitationMultiplier = 0.0f;
		nodeGroupConfig.excitationThreshold = 0.5f;
		nodeGroupConfig.excitationFatigue = 0.5f;
		configuration.nodeGroups.push_back(nodeGroupConfig);
	}
	{
		NeuralNodeGroupConfiguration nodeGroupConfig;
		nodeGroupConfig.id = 2;
		nodeGroupConfig.nodeCount = OUTPUT_NODE_COUNT;
		nodeGroupConfig.excitationOffset = 1.0f;
		nodeGroupConfig.excitationMultiplier = 0.0f;
		nodeGroupConfig.excitationThreshold = 0.5f;
		nodeGroupConfig.excitationFatigue = 0.1f;
		configuration.nodeGroups.push_back(nodeGroupConfig);
	}

	{
		Matrix<float> weights(INPUT_NODE_COUNT, hiddenNodeAmount);
		for (int y = 0; y < weights.getHeight(); y++)
		{
			for (int x = 0; x < weights.getWidth(); x++)
			{
				weights.setValue(x, y, utility::getRandomFloat(-1.0f, 1.0f));
			}
		}
		configuration.edgeGroups.push_back(NeuralEdgeGroupConfiguration(0, 1, weights));
	}
	{
		Matrix<float> weights(hiddenNodeAmount, hiddenNodeAmount);
		for (int y = 0; y < weights.getHeight(); y++)
		{
			for (int x = 0; x < weights.getWidth(); x++)
			{
				weights.setValue(x, y, utility::getRandomFloat(-1.0f, 1.0f));
			}
		}
		configuration.edgeGroups.push_back(NeuralEdgeGroupConfiguration(1, 1, weights));
	}
	{
		Matrix<float> weights(hiddenNodeAmount, OUTPUT_NODE_COUNT);
		for (int y = 0; y < weights.getHeight(); y++)
		{
			for (int x = 0; x < weights.getWidth(); x++)
			{
				weights.setValue(x, y, utility::getRandomFloat(-1.0f, 1.0f));
			}
		}
		configuration.edgeGroups.push_back(NeuralEdgeGroupConfiguration(1, 2, weights));
	}

	return configuration;
}

float runPingEvaluation(NeuralNetworkConfiguration genotype, bool verbose)
{
	return runPingEvaluation(genotype, utility::getRandomInt(0, OUTPUT_NODE_COUNT - 1), verbose);
}

float runPingEvaluation(NeuralNetworkConfiguration genotype, const int nunmberOfPings, bool verbose)
{
	if (verbose)
	{
		LOG_INFO("Running Ping Evaluation");
	}

	if (verbose)
	{
		LOG_INFO("Conducted number of pings: " + std::to_string(nunmberOfPings));
	}

	std::shared_ptr<NeuralNetwork> neuralNetwork = NeuralNetwork::create(genotype);

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
