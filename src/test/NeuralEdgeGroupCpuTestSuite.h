#include "cxxtest/TestSuite.h"

#include "tinyxml/tinyxml.h"

#include "neural_network/cpu/NeuralNodeGroupLinearExcitationCpu.h"
#include "neural_network/cpu/NeuralNodeGroupStepExcitationCpu.h"
#include "neural_network/cpu/NeuralEdgeGroupCpu.h"

class NeuralEdgeGroupCpuTestSuite : public CxxTest::TestSuite
{
public:
	void test_edge_group_can_be_loaded_from_xml_element()
	{
		std::vector<std::shared_ptr<NeuralNodeGroupCpu>> nodeGroups = {
			std::make_shared<NeuralNodeGroupLinearExcitationCpu>(0, 2, 1.0f),
			std::make_shared<NeuralNodeGroupLinearExcitationCpu>(1, 3, 1.0f)
		};

		TiXmlDocument doc;
		doc.Parse(
			"<edge_group source_group_id=\"0\" target_group_id=\"1\" weights=\"0.16,-0.20;0.67,-0.9;-0.11,0.28\"/>",
			0,
			TIXML_ENCODING_UTF8
		);

		const TiXmlElement* element = doc.FirstChildElement("edge_group");
		std::shared_ptr<NeuralEdgeGroupCpu> edgeGroup = NeuralEdgeGroupCpu::loadFromXmlElement(element, nodeGroups);

		TS_ASSERT(edgeGroup);
	}
};
