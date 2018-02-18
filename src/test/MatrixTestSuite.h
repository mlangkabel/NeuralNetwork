#include "cxxtest/TestSuite.h"

#include "utility/Matrix.h"

class MatrixTestSuite : public CxxTest::TestSuite
{
public:
	void test_matrix_cal_be_loaded_from_string()
	{
		Matrix<float> m1(3, 2);
		m1.setValue(0, 0, +0.4f);	m1.setValue(1, 0, +0.6f);	m1.setValue(2, 0, -0.9f);
		m1.setValue(0, 1, -8.4f);	m1.setValue(1, 1, +1.4f);	m1.setValue(2, 1, -0.5f);

		Matrix<float> m2(3, 2);
		m2.setValuesFromString(m1.getValuesAsString());

		TS_ASSERT_EQUALS(m1, m2);
	}
};
