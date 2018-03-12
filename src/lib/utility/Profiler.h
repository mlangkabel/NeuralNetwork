#ifndef PROFILER_H
#define PROFILER_H

#include <string>
#include <map>
#include <memory>

class Profiler
{
public:
	static std::shared_ptr<Profiler> getInstance();

	void recordTrace(const std::string& name, long long duration);
	void printTraces() const;

private:
	static std::shared_ptr<Profiler> s_instance;

	Profiler();

	std::map<std::string, long long> m_traces;
};

#endif // PROFILER_H
