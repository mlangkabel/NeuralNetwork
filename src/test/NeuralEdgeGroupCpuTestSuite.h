#include "cxxtest/TestSuite.h"

#include "tinyxml/tinyxml.h"

#include "neural_network/cpu/NeuralNodeGroupCpu.h"
#include "neural_network/cpu/NeuralEdgeGroupCpu.h"

class NeuralEdgeGroupCpuTestSuite : public CxxTest::TestSuite
{
public:
	void test_edge_group_can_be_loaded_from_xml_element()
	{
		try
		{
			std::vector<std::shared_ptr<NeuralNodeGroupCpu>> nodeGroups = {
				std::make_shared<NeuralNodeGroupCpu>(0, 2, 1.0f, 0.0f, 0.4f, 0.3f),
				std::make_shared<NeuralNodeGroupCpu>(1, 3, 1.0f, 0.0f, 0.4f, 0.3f)
			};

			TiXmlDocument doc;
			doc.Parse(
				"<edge_group source_group_id=\"0\" target_group_id=\"1\">\n"
				"	<weights width=\"2\" height=\"3\" values=\"0.16,-0.20;0.67,-0.9;-0.11,0.28\" />\n"
				"</edge_group>\n",
				0,
				TIXML_ENCODING_UTF8
			);

			const TiXmlElement* element = doc.FirstChildElement("edge_group");
			std::shared_ptr<NeuralEdgeGroupCpu> edgeGroup = NeuralEdgeGroupCpu::create(neuralEdgeGroupConfigurationFromXmlElement(element), nodeGroups);

			TS_ASSERT(edgeGroup);
		}
		catch (const std::exception& e)
		{
			TS_FAIL(e.what());
		}
	}
};
