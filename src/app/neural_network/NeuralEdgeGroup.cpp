#include "neural_network/NeuralEdgeGroup.h"

#include "neural_network/NeuralNodeGroup.h"
#include "utility/logging.h"

NeuralEdgeGroup::NeuralEdgeGroup(
	std::shared_ptr<NeuralNodeGroup> sourceNodes, 
	std::shared_ptr<NeuralNodeGroup> targetNodes, 
	cl::Device device,
	cl::Context context,
	cl::CommandQueue queue
)
	: m_sourceNodes(sourceNodes)
	, m_targetNodes(targetNodes)
	, m_device(device)
	, m_context(context)
	, m_queue(queue)
	, m_weightsBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_float) * m_sourceNodes->getNodeCount() * m_targetNodes->getNodeCount())
{

}

void NeuralEdgeGroup::setWeights(Matrix<float> weights)
{
	if (weights.getWidth() == m_sourceNodes->getNodeCount() && weights.getHeight() == m_targetNodes->getNodeCount())
	{
		m_queue.enqueueWriteBuffer(m_weightsBuffer, CL_TRUE, 0, sizeof(cl_float) * weights.getElementCount(), &weights[0]);
	}
	else
	{
		LOG_ERROR("The size of the weights matrix does not match the source and target node count.");
	}
}

Matrix<float> NeuralEdgeGroup::getWeights()
{
	Matrix<float> weights(m_sourceNodes->getNodeCount(), m_targetNodes->getNodeCount());
	m_queue.enqueueReadBuffer(m_weightsBuffer, CL_TRUE, 0, sizeof(cl_float) * weights.getElementCount(), &weights[0]);
	return weights;
}

void NeuralEdgeGroup::update()
{
	const std::string kernel_code = (
		"void kernel ss(const int source_value_count, const __global float* source_values, const __global float* weights, __global float* target_values){\n" // __constant image2d_t weights,
		"	const int id = get_global_id(0);\n"
		"	float acc = 0.0f;\n"
		"	for (int i = 0; i < source_value_count; i++)\n"
		"	{\n"
		"		acc += weights[source_value_count * id + i] * source_values[i];\n"
		"	}\n"
		"	target_values[id] = acc;\n"
		"}\n"
	);

	cl::Program::Sources sources;
	sources.push_back({ kernel_code.c_str(), kernel_code.length() });

	cl::Program program(m_context, sources);
	if (program.build({ m_device }) != CL_SUCCESS) 
	{
		LOG_ERROR(std::string("building OpenCL program failed: ") + program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(m_device));
		return;
	}

	cl_int error;
	cl::Kernel kernel = cl::Kernel(program, "ss", &error);
	if (error != CL_SUCCESS)
	{
		LOG_ERROR("Creating the kernel failed.");
		return;
	}

	kernel.setArg(0, m_sourceNodes->getNodeCount());
	kernel.setArg(1, m_sourceNodes->getBuffer());
	kernel.setArg(2, m_weightsBuffer);
	kernel.setArg(3, m_targetNodes->getBuffer());
	m_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(10), cl::NullRange);
	m_queue.finish();
}
