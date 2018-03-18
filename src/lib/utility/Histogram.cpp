#include "utility/Histogram.h"

#include <algorithm>

Histogram::Bin::Bin(float minValue, float binSize)
	: minValue(minValue)
	, maxValue(minValue + binSize)
	, meanValue(minValue + 0.5f * binSize)
	, count(0)
{
}

Histogram::Histogram(float binSize)
	: m_binSize(binSize)
{
}

void Histogram::addValue(float value)
{
	int binId = floor(value / m_binSize);
	std::map<int, Bin>::iterator it = m_bins.find(binId);
	if (it != m_bins.end())
	{
		it->second.count++;
	}
	else
	{
		Bin bin(binId * m_binSize, m_binSize);
		bin.count = 1;
		m_bins.insert(std::make_pair(binId, bin));
	}
}

std::vector<Histogram::Bin> Histogram::getSortedBins()
{
	if (m_bins.empty())
	{
		return std::vector<Bin>();
	}
	float minValue = std::numeric_limits<float>::infinity();
	float maxValue = -std::numeric_limits<float>::infinity();

	for (std::map<int, Bin>::const_iterator it = m_bins.begin(); it != m_bins.end(); it++)
	{
		if (it->second.meanValue < minValue)
		{
			minValue = it->second.meanValue;
		}
		if (it->second.meanValue > maxValue)
		{
			maxValue = it->second.meanValue;
		}
	}
	return getSortedBins(minValue, maxValue);
}

std::vector<Histogram::Bin> Histogram::getSortedBins(float minValue, float maxValue)
{
	std::vector<Bin> bins;

	for (int i = floor(minValue / m_binSize); i < ceil(maxValue / m_binSize); i++)
	{
		std::map<int, Bin>::iterator it = m_bins.find(i);
		if (it != m_bins.end())
		{
			bins.push_back(it->second);
		}
		else
		{
			bins.push_back(Bin(i * m_binSize, m_binSize));
		}
	}

	std::sort(bins.begin(), bins.end(), [](const Bin& a, const Bin& b) { return a.minValue < b.minValue; });

	return bins;
}
