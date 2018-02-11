#include "cxxtest/TestSuite.h"

#include "neural_network/NeuralNodeGroup.h"

class NeuralNodeGroupTestSuite : public CxxTest::TestSuite
{
public:
	void test_system_gets_initialized_correctly()
	{
		NeuralNodeGroup nodeGroup(5);

		std::vector<float> excitationLevels = { -1.0f, 2.3f, 0.1f, 11.0f, -5.6f };

		nodeGroup.setExcitationLevels(excitationLevels);

		TS_ASSERT_EQUALS(nodeGroup.getExcitationLevels(), excitationLevels);
	}
};
