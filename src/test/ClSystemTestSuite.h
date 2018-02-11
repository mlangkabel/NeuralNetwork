#include "cxxtest/TestSuite.h"

#include "utility/cl/ClSystem.h"

class ClSystemTestSuite : public CxxTest::TestSuite
{
public:
	void test_system_gets_initialized_correctly()
	{
		std::shared_ptr<ClSystem> clSystem = ClSystem::getInstance();

		TS_ASSERT(clSystem.use_count() > 0);
	}
};
