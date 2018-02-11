#include "utility/cl/ClSystem.h"
#include "utility/logging.h"

std::shared_ptr<ClSystem> ClSystem::getInstance()
{
	if (!s_instance && !s_initialized)
	{
		s_instance = std::shared_ptr<ClSystem>(new ClSystem());
		bool success = s_instance->initialize();
		if (!success)
		{
			s_instance.reset();
		}
	}
	return s_instance;
}

ClSystem::~ClSystem()
{
}

cl::Platform ClSystem::getPlatform()
{
	return m_platform;
}

cl::Device ClSystem::getDevice()
{
	return m_device;
}

cl::Context ClSystem::getContext()
{
	return m_context;
}

cl::CommandQueue ClSystem::getQueue()
{
	return m_queue;
}

bool ClSystem::initialize()
{
	s_initialized = true;

	LOG_INFO("Initializing OpenCL system.");

	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	if (platforms.empty())
	{
		LOG_ERROR("No platforms found. Check OpenCL installation!");
		return false;
	}
	m_platform = platforms[0];
	LOG_INFO("Using platform: " + m_platform.getInfo<CL_PLATFORM_NAME>() + " Version: " + m_platform.getInfo<CL_PLATFORM_VERSION>());

	//get default device of the default platform
	std::vector<cl::Device> devices;
	m_platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
	if (devices.empty())
	{
		LOG_ERROR("No devices found. Check OpenCL installation!");
		return false;
	}
	m_device = devices[0];
	LOG_INFO("Using device: " + m_device.getInfo<CL_DEVICE_NAME>());

	m_context = cl::Context(m_device);

	m_queue = cl::CommandQueue(m_context, m_device);

	return true;
}

std::shared_ptr<ClSystem> ClSystem::s_instance;
bool ClSystem::s_initialized = false;

ClSystem::ClSystem()
{
	
}
