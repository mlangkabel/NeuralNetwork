#ifndef SCOPED_PROFILE_TRACER_H
#define SCOPED_PROFILE_TRACER_H

#include <string>
#include <chrono>

class ScopedProfileTracer
{
public:
	ScopedProfileTracer(const std::string& name);
	~ScopedProfileTracer();

private:
	const std::string m_name;
	const std::chrono::high_resolution_clock::time_point m_start;
};

#endif // SCOPED_PROFILE_TRACER_H
