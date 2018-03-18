#ifndef HISTOGRAM_H
#define HISTOGRAM_H

//#include <string>
#include <map>
#include <vector>

class Histogram
{
public:
	struct Bin
	{
		Bin(float minValue, float binSize);

		float minValue;
		float maxValue;
		float meanValue;
		int count;
	};

	Histogram(float binSize);

	void addValue(float value);
	std::vector<Bin> getSortedBins();
	std::vector<Bin> getSortedBins(float minValue, float maxValue);

private:
	float m_binSize;

	std::map<int, Bin> m_bins;
};

#endif // HISTOGRAM_H
