#include "cxxtest/TestSuite.h"

#include "tinyxml/tinyxml.h"

#include "neural_network/gpu/NeuralNodeGroupLinearExcitationGpu.h"
#include "neural_network/gpu/NeuralNodeGroupStepExcitationGpu.h"

class NeuralNodeGroupGpuTestSuite : public CxxTest::TestSuite
{
public:
	void test_node_group_linear_excitation_can_be_loaded_from_xml_element()
	{
		TiXmlDocument doc;
		doc.Parse(
			"<node_group_linear_excitation id=\"3\" node_count=\"5\" excitation_factor=\"0.5\" />",
			0,
			TIXML_ENCODING_UTF8
		);

		const TiXmlElement* element = doc.FirstChildElement("node_group_linear_excitation");
		std::shared_ptr<NeuralNodeGroupLinearExcitationGpu> nodeGroup = NeuralNodeGroupLinearExcitationGpu::loadFromXmlElement(element);

		TS_ASSERT(nodeGroup);

		if (nodeGroup)
		{
			TS_ASSERT_EQUALS(nodeGroup->getId(), 3);
			TS_ASSERT_EQUALS(nodeGroup->getNodeCount(), 5);
			TS_ASSERT_EQUALS(nodeGroup->getExcitationFactor(), 0.5f);
		}
	}

	void test_node_group_stores_set_excitation_levels_correctly()
	{
		NeuralNodeGroupLinearExcitationGpu nodeGroup(1, 5, 1.0f);

		const std::vector<float> excitationLevels = { -1.0f, 2.3f, 0.1f, 11.0f, -5.6f };

		nodeGroup.setExcitationLevels(excitationLevels);

		TS_ASSERT_EQUALS(nodeGroup.getExcitationLevels(), excitationLevels);
	}

	void test_node_group_fails_to_set_excitation_levels_if_count_does_not_match()
	{
		NeuralNodeGroupLinearExcitationGpu nodeGroup(1, 1, 1.0f);

		const std::vector<float> excitationLevels = { -1.0f, 2.3f };

		nodeGroup.setExcitationLevels(excitationLevels);

		TS_ASSERT_EQUALS(nodeGroup.getExcitationLevels(), std::vector<float>({ 0.0f }));
	}

	void test_node_group_linear_excitation_updates_excitation_state_correctly()
	{
		NeuralNodeGroupLinearExcitationGpu nodeGroup(1, 2, 0.5f);

		const std::vector<float> excitationLevels = { -1.0f, 2.2f };
		const std::vector<float> expectedExcitationStates = { -0.5f, 1.1f };

		nodeGroup.setExcitationLevels(excitationLevels);

		nodeGroup.update();

		TS_ASSERT_EQUALS(nodeGroup.getExcitationStates(), expectedExcitationStates);
	}

	void test_node_group_step_excitation_can_be_loaded_from_xml_element()
	{
		TiXmlDocument doc;
		doc.Parse(
			"<node_group_step_excitation id=\"3\" node_count=\"5\" excitation_threshold=\"0.5\" excitation_fatigue=\"0.1\" />",
			0,
			TIXML_ENCODING_UTF8
		);

		const TiXmlElement* element = doc.FirstChildElement("node_group_step_excitation");
		std::shared_ptr<NeuralNodeGroupStepExcitationGpu> nodeGroup = NeuralNodeGroupStepExcitationGpu::loadFromXmlElement(element);

		TS_ASSERT(nodeGroup);

		if (nodeGroup)
		{
			TS_ASSERT_EQUALS(nodeGroup->getId(), 3);
			TS_ASSERT_EQUALS(nodeGroup->getNodeCount(), 5);
			TS_ASSERT_EQUALS(nodeGroup->getExcitationThreshold(), 0.5f);
			TS_ASSERT_EQUALS(nodeGroup->getExcitationFatigue(), 0.1f);
		}
	}

	void test_node_group_step_excitation_updates_excitation_state_correctly()
	{
		NeuralNodeGroupStepExcitationGpu nodeGroup(1, 4, 0.5f, 0.0f);

		const std::vector<float> excitationLevels = { -1.0f, 0.49f, 0.51f, 0.6f };
		const std::vector<float> expectedExcitationStates = { 0.0f, 0.0f, 1.0f, 1.0f };

		nodeGroup.setExcitationLevels(excitationLevels);

		nodeGroup.update();

		TS_ASSERT_EQUALS(nodeGroup.getExcitationStates(), expectedExcitationStates);
	}

	void test_node_group_step_excitation_with_no_fatigue_maintains_excitation_levels()
	{
		NeuralNodeGroupStepExcitationGpu nodeGroup(1, 4, 0.5f, 0.0f);

		const std::vector<float> excitationLevels = { -1.0f, 0.49f, 0.51f, 0.6f };

		nodeGroup.setExcitationLevels(excitationLevels);

		nodeGroup.update();

		TS_ASSERT_EQUALS(nodeGroup.getExcitationLevels(), excitationLevels);
	}

	void test_node_group_step_excitation_with_full_fatigue_discards_excitation_levels_for_excited_nodes()
	{
		NeuralNodeGroupStepExcitationGpu nodeGroup(1, 4, 0.5f, 1.0f);

		const std::vector<float> excitationLevels = { -1.0f, 0.49f, 0.51f, 0.6f };
		const std::vector<float> expectedExcitationLevels = { -1.0f, 0.49f, 0.0f, 0.0f };

		nodeGroup.setExcitationLevels(excitationLevels);

		nodeGroup.update();

		TS_ASSERT_EQUALS(nodeGroup.getExcitationLevels(), expectedExcitationLevels);
	}
};
