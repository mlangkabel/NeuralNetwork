#include "cxxtest/TestSuite.h"

#include "neural_network/NeuralNodeGroupLinearExcitation.h"
#include "neural_network/NeuralNodeGroupStepExcitation.h"

class NeuralNodeGroupTestSuite : public CxxTest::TestSuite
{
public:
	void test_node_group_stores_set_excitation_levels_correctly()
	{
		NeuralNodeGroupLinearExcitation nodeGroup(5);

		const std::vector<float> excitationLevels = { -1.0f, 2.3f, 0.1f, 11.0f, -5.6f };

		nodeGroup.setExcitationLevels(excitationLevels);

		TS_ASSERT_EQUALS(nodeGroup.getExcitationLevels(), excitationLevels);
	}

	void test_node_group_fails_to_set_excitation_levels_if_count_does_not_match()
	{
		NeuralNodeGroupLinearExcitation nodeGroup(1);

		const std::vector<float> excitationLevels = { -1.0f, 2.3f };

		nodeGroup.setExcitationLevels(excitationLevels);

		TS_ASSERT_EQUALS(nodeGroup.getExcitationLevels(), std::vector<float>({ 0.0f }));
	}

	void test_node_group_linear_excitation_updates_excitation_state_correctly()
	{
		NeuralNodeGroupLinearExcitation nodeGroup(2, 0.5f);

		const std::vector<float> excitationLevels = { -1.0f, 2.2f };
		const std::vector<float> expectedExcitationStates = { -0.5f, 1.1f };

		nodeGroup.setExcitationLevels(excitationLevels);

		nodeGroup.update();

		TS_ASSERT_EQUALS(nodeGroup.getExcitationStates(), expectedExcitationStates);
	}

	void test_node_group_step_excitation_updates_excitation_state_correctly()
	{
		NeuralNodeGroupStepExcitation nodeGroup(4, 0.5f);

		const std::vector<float> excitationLevels = { -1.0f, 0.4f, 0.5f, 0.6f };
		const std::vector<float> expectedExcitationStates = { 0.0f, 0.0f, 1.0f, 1.0f };

		nodeGroup.setExcitationLevels(excitationLevels);

		nodeGroup.update();

		TS_ASSERT_EQUALS(nodeGroup.getExcitationStates(), expectedExcitationStates);
	}
};
