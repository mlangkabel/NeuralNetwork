
add_files(
	LIB_FILES
	
	evolution/EvaluatedGenotype.h
	evolution/EvolutionEnvironment.h
	
	neural_network/cpu/NeuralEdgeGroupCpu.cpp
	neural_network/cpu/NeuralEdgeGroupCpu.h
	neural_network/cpu/NeuralNetworkCpu.cpp
	neural_network/cpu/NeuralNetworkCpu.h
	neural_network/cpu/NeuralNodeGroupCpu.cpp
	neural_network/cpu/NeuralNodeGroupCpu.h
	neural_network/cpu/NeuralNodeGroupLinearExcitationCpu.cpp
	neural_network/cpu/NeuralNodeGroupLinearExcitationCpu.h
	neural_network/cpu/NeuralNodeGroupStepExcitationCpu.cpp
	neural_network/cpu/NeuralNodeGroupStepExcitationCpu.h
	
	neural_network/gpu/NeuralEdgeGroupGpu.cpp
	neural_network/gpu/NeuralEdgeGroupGpu.h
	neural_network/gpu/NeuralNetworkGpu.cpp
	neural_network/gpu/NeuralNetworkGpu.h
	neural_network/gpu/NeuralNodeGroupGpu.cpp
	neural_network/gpu/NeuralNodeGroupGpu.h
	neural_network/gpu/NeuralNodeGroupLinearExcitationGpu.cpp
	neural_network/gpu/NeuralNodeGroupLinearExcitationGpu.h
	neural_network/gpu/NeuralNodeGroupStepExcitationGpu.cpp
	neural_network/gpu/NeuralNodeGroupStepExcitationGpu.h
	
	utility/cl/ClSystem.cpp
	utility/cl/ClSystem.h
	
	utility/random/RandomNumberGenerator.cpp
	utility/random/RandomNumberGenerator.h
	utility/random/WeightedRandomGenerator.h
	
	utility/Histogram.cpp
	utility/Histogram.h
	utility/logging.h
	utility/Matrix.h
	utility/Optional.h
	utility/Profiler.cpp
	utility/Profiler.h
	utility/ScopedProfileTracer.cpp
	utility/ScopedProfileTracer.h
	utility/TextAccess.cpp
	utility/TextAccess.h
	utility/types.h
	utility/utility.cpp
	utility/utility.h
	utility/utilityRandom.cpp
	utility/utilityRandom.h
	utility/utilityString.cpp
	utility/utilityString.h
)