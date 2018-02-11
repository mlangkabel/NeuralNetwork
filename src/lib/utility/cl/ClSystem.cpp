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

void ClSystem::clearInstance()
{
	s_instance.reset();
	s_initialized = false;
}

ClSystem::~ClSystem()
{
}

Optional<cl::Kernel> ClSystem::addKernel(const std::string& name, const std::string& code)
{
	std::map<std::string, std::pair<std::string, cl::Kernel>>::const_iterator it = m_kernels.find(name);
	if (it != m_kernels.end())
	{
		if (it->second.first == code)
		{
			return Optional<cl::Kernel>(it->second.second);
		}
		LOG_ERROR("Existing kernel \"" + name + "\" does not match code of newly created one.");
		return Optional<cl::Kernel>();
	}

	Optional<cl::Kernel> kernel = buildKernel(name, code);
	if (kernel)
	{
		m_kernels.insert(std::make_pair(name, std::make_pair(code, kernel.get())));
	}
	return kernel;
}

Optional<cl::Kernel> ClSystem::getKernel(const std::string& name)
{
	std::map<std::string, std::pair<std::string, cl::Kernel>>::const_iterator it = m_kernels.find(name);
	if (it != m_kernels.end())
	{
		return Optional<cl::Kernel>(it->second.second);
	}
	LOG_ERROR("No kernel found for name \"" + name + "\".");
	return Optional<cl::Kernel>();
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

ClSystem::ClSystem()
{
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

Optional<cl::Kernel> ClSystem::buildKernel(const std::string& name, const std::string& code)
{
	cl::Program::Sources sources;
	sources.push_back({ code.c_str(), code.length() });

	cl::Program program(m_context, sources);
	if (program.build({ m_device }) != CL_SUCCESS)
	{
		LOG_ERROR(std::string("Building OpenCL program \"" + name + "\" failed: ") + program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(ClSystem::getInstance()->getDevice()));
		return Optional<cl::Kernel>();
	}

	cl_int error;
	cl::Kernel kernel(program, name.c_str(), &error);
	if (error != CL_SUCCESS)
	{
		LOG_ERROR("Creating the kernel failed.");
		return Optional<cl::Kernel>();
	}

	return Optional<cl::Kernel>(kernel);
}

std::shared_ptr<ClSystem> ClSystem::s_instance;
bool ClSystem::s_initialized = false;
