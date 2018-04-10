#include "cxxtest/TestSuite.h"

#include "tinyxml/tinyxml.h"

#include "neural_network/cpu/NeuralNetworkCpu.h"
#include "neural_network/NeuralNetworkConfiguration.h"
#include "utility/TextAccess.h"

class NeuralNetworkCpuTestSuite : public CxxTest::TestSuite
{
public:
	void test_neural_network_can_be_loaded_from_text_access()
	{
		try
		{
			TiXmlDocument doc;
			doc.Parse(
				"<neural_network input_node_group_id=\"0\" output_node_group_id=\"1\">\n"
				"	<node_group id=\"0\" node_count=\"2\" excitation_offset=\"0.1\" excitation_multiplier=\"0.2\" excitation_threshold=\"0.3\" excitation_fatigue=\"0.4\" />\n"
				"	<node_group id=\"1\" node_count=\"3\" excitation_offset=\"0.1\" excitation_multiplier=\"0.2\" excitation_threshold=\"0.3\" excitation_fatigue=\"0.4\" />\n"
				"	<edge_group source_group_id=\"0\" target_group_id=\"1\">\n"
				"		<weights width=\"2\" height=\"3\" values=\"0.16,-0.20;0.67,-0.9;-0.11,0.28\" />\n"
				"	</edge_group>\n"
				"</neural_network>\n",
				0,
				TIXML_ENCODING_UTF8
			);

			const TiXmlElement* element = doc.FirstChildElement("neural_network");

			std::shared_ptr<NeuralNetworkCpu> neuralNetwork = NeuralNetworkCpu::create(neuralNetworkConfigurationFromXmlElement(element));

			TS_ASSERT(neuralNetwork);
		}
		catch (const std::exception& e)
		{
			TS_FAIL(e.what());
		}
	}
};
