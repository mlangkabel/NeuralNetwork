#include "cxxtest/TestSuite.h"

#include "tinyxml/tinyxml.h"

#include "neural_network/cpu/NeuralNodeGroupCpu.h"

class NeuralNodeGroupCpuTestSuite : public CxxTest::TestSuite
{
public:
	void test_node_group_linear_excitation_can_be_loaded_from_xml_element()
	{
		try
		{
			TiXmlDocument doc;
			doc.Parse(
				"<node_group id=\"3\" node_count=\"5\" excitation_offset=\"0.1\" excitation_multiplier=\"0.2\" excitation_threshold=\"0.3\" excitation_fatigue=\"0.4\" />\n",
				0,
				TIXML_ENCODING_UTF8
			);

			const TiXmlElement* element = doc.FirstChildElement("node_group");
			std::shared_ptr<NeuralNodeGroupCpu> nodeGroup = NeuralNodeGroupCpu::create(neuralNodeGroupConfigurationFromXmlElement(element));

			TS_ASSERT(nodeGroup);

			if (nodeGroup)
			{
				TS_ASSERT_EQUALS(nodeGroup->getId(), 3);
				TS_ASSERT_EQUALS(nodeGroup->getNodeCount(), 5);
			}
		}
		catch (const std::exception& e)
		{
			TS_FAIL(e.what());
		}
	}

	void test_node_group_stores_set_excitation_levels_correctly()
	{
		NeuralNodeGroupCpu nodeGroup(1, 5, 1.0f, 0.5f, 0.5f, 0.3f);

		const std::vector<float> excitationLevels = { -1.0f, 2.3f, 0.1f, 11.0f, -5.6f };

		nodeGroup.setExcitationLevels(excitationLevels);

		TS_ASSERT_EQUALS(nodeGroup.getExcitationLevels(), excitationLevels);
	}

	void test_node_group_fails_to_set_excitation_levels_if_count_does_not_match()
	{
		NeuralNodeGroupCpu nodeGroup(1, 1, 1.0f, 0.5f, 0.5f, 0.3f);

		const std::vector<float> excitationLevels = { -1.0f, 2.3f };

		nodeGroup.setExcitationLevels(excitationLevels);

		TS_ASSERT_EQUALS(nodeGroup.getExcitationLevels(), std::vector<float>({ 0.0f }));
	}

	void test_node_group_linear_excitation_updates_excitation_state_correctly()
	{
		NeuralNodeGroupCpu nodeGroup(1, 2, 1.0f, 0.3f, 0.5f, 0.1f);

		const std::vector<float> excitationLevels = { -1.0f, 2.0f };
		const std::vector<float> expectedExcitationLevels = { -1.0f, 1.8f };
		const std::vector<float> expectedExcitationStates = { 0.0f, 1.6f };

		nodeGroup.setExcitationLevels(excitationLevels);

		nodeGroup.update();

		TS_ASSERT_EQUALS(nodeGroup.getExcitationStates(), expectedExcitationStates);
	}
};
