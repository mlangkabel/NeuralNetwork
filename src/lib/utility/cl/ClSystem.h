#ifndef CL_SYSTEM_H
#define CL_SYSTEM_H

#include <map>
#include <memory>

#include <CL/cl.hpp>

#include "utility/Optional.h"

class ClSystem 
{
public:
	static std::shared_ptr<ClSystem> getInstance();
	static void clearInstance();

	~ClSystem();

	Optional<cl::Kernel> addKernel(const std::string& name, const std::string& code);
	Optional<cl::Kernel> getKernel(const std::string& name);

	cl::Platform getPlatform();
	cl::Device getDevice();
	cl::Context getContext();
	cl::CommandQueue getQueue();

private:
	static std::shared_ptr<ClSystem> s_instance;
	static bool s_initialized;

	ClSystem();
	bool initialize();

	Optional<cl::Kernel> buildKernel(const std::string& name, const std::string& code);

	cl::Platform m_platform;
	cl::Device m_device;
	cl::Context m_context;
	cl::CommandQueue m_queue;
	std::map<std::string, std::pair<std::string, cl::Kernel>> m_kernels;

};

#endif // CL_SYSTEM_H