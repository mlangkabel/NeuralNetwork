#include "utility/Profiler.h"

#include <iostream>

std::shared_ptr<Profiler> Profiler::getInstance()
{
	if (!s_instance)
	{
		s_instance = std::shared_ptr<Profiler>(new Profiler());
	}
	return s_instance;
}

void Profiler::recordTrace(const std::string& name, long long duration)
{
	if (m_traces.find(name) == m_traces.end())
	{
		m_traces[name] = duration;
	}
	else
	{
		m_traces[name] += duration;
	}
}

void Profiler::printTraces() const
{
	for (std::map<std::string, long long>::const_iterator it = m_traces.begin(); it != m_traces.end(); it++)
	{
		std::cout << "Trace \"" << it->first << "\": " << it->second << std::endl;
	}
}

std::shared_ptr<Profiler> Profiler::s_instance;

Profiler::Profiler()
{
}
