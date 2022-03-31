#ifndef EXECUTIONTIMEANALYZER_SIMULATION_DATA_GENERATOR
#define EXECUTIONTIMEANALYZER_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class ExecutionTimeAnalyzerSettings;

class ExecutionTimeSimulationDataGenerator
{
public:
    ExecutionTimeSimulationDataGenerator();
    ~ExecutionTimeSimulationDataGenerator();

    void Initialize( U32 simulation_sample_rate, ExecutionTimeAnalyzerSettings* settings );
    U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
    ExecutionTimeAnalyzerSettings* mSettings;
    U32 mSimulationSampleRateHz;

protected:
    void CreateExecutionTime();
    void CreateSubExecutionTime(U32 ms);

    SimulationChannelDescriptorGroup mSimulationChannels;
    SimulationChannelDescriptor* mTimer;
    SimulationChannelDescriptor* mSubTimer;
};
#endif //EXECUTIONTIMEANALYZER_SIMULATION_DATA_GENERATOR