#ifndef CL_SYSTEM_H
#define CL_SYSTEM_H

#include <memory>

#include <CL/cl.hpp>

class ClSystem 
{
public:
	static std::shared_ptr<ClSystem> getInstance();

	~ClSystem();

	cl::Platform getPlatform();
	cl::Device getDevice();
	cl::Context getContext();
	cl::CommandQueue getQueue();

private:
	static std::shared_ptr<ClSystem> s_instance;
	static bool s_initialized;

	ClSystem();
	bool initialize();

	cl::Platform m_platform;
	cl::Device m_device;
	cl::Context m_context;
	cl::CommandQueue m_queue;

};

#endif // CL_SYSTEM_H