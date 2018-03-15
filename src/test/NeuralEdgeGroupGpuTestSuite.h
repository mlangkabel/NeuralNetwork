#include "cxxtest/TestSuite.h"

#include "tinyxml/tinyxml.h"

#include "neural_network/gpu/NeuralNodeGroupLinearExcitationGpu.h"
#include "neural_network/gpu/NeuralNodeGroupStepExcitationGpu.h"
#include "neural_network/gpu/NeuralEdgeGroupGpu.h"

class NeuralEdgeGroupGpuTestSuite : public CxxTest::TestSuite
{
public:
	void test_edge_group_can_be_loaded_from_xml_element()
	{
		std::vector<std::shared_ptr<NeuralNodeGroupGpu>> nodeGroups = {
			std::make_shared<NeuralNodeGroupLinearExcitationGpu>(0, 2, 1.0f),
			std::make_shared<NeuralNodeGroupLinearExcitationGpu>(1, 3, 1.0f)
		};

		TiXmlDocument doc;
		doc.Parse(
			"<edge_group source_group_id=\"0\" target_group_id=\"1\" weights=\"0.16,-0.20;0.67,-0.9;-0.11,0.28\"/>",
			0,
			TIXML_ENCODING_UTF8
		);

		const TiXmlElement* element = doc.FirstChildElement("edge_group");
		std::shared_ptr<NeuralEdgeGroupGpu> edgeGroup = NeuralEdgeGroupGpu::loadFromXmlElement(element, nodeGroups);

		TS_ASSERT(edgeGroup);
	}
};
