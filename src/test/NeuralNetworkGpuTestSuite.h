#include "cxxtest/TestSuite.h"

#include "tinyxml/tinyxml.h"

#include "neural_network/gpu/NeuralNetworkGpu.h"
#include "utility/TextAccess.h"

class NeuralNetworkGpuTestSuite : public CxxTest::TestSuite
{
public:
	void test_neural_network_can_be_loaded_from_text_access()
	{
		std::shared_ptr<TextAccess> textAccess = TextAccess::createFromString(
			"<neural_network input_node_group_id=\"0\" output_node_group_id=\"1\">\n"
			"	<node_group_step_excitation id=\"0\" node_count=\"2\" excitation_threshold=\"0.5\" excitation_fatigue=\"0.1\" />\n"
			"	<node_group_step_excitation id=\"1\" node_count=\"3\" excitation_threshold=\"0.5\" excitation_fatigue=\"0.1\" />\n"
			"	<edge_group source_group_id=\"0\" target_group_id=\"1\" weights=\"0.16,-0.20;0.67,-0.9;-0.11,0.28\"/>\n"
			"</neural_network>\n"
		);

		std::shared_ptr<NeuralNetworkGpu> neuralNetwork = NeuralNetworkGpu::load(textAccess);

		TS_ASSERT(neuralNetwork);
	}
};
