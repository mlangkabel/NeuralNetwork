#include "utility/ScopedProfileTracer.h"

#include <iostream>

#include "utility/Profiler.h"

ScopedProfileTracer::ScopedProfileTracer(const std::string& name)
	: m_name(name)
	, m_start(std::chrono::high_resolution_clock::now())
{
}

ScopedProfileTracer::~ScopedProfileTracer()
{
	const std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count();
	Profiler::getInstance()->recordTrace(m_name, duration);
}
