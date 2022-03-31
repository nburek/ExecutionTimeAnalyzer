#include "ExecutionTimeSimulationDataGenerator.h"
#include "ExecutionTimeAnalyzerSettings.h"

#include <AnalyzerHelpers.h>
#include <cstdlib>

U32 MillsecondsToSampleCount(U32 ms, U32 sample_rate_hz) {
   return sample_rate_hz / 1000 * ms;
}

ExecutionTimeSimulationDataGenerator::ExecutionTimeSimulationDataGenerator()
{
}

ExecutionTimeSimulationDataGenerator::~ExecutionTimeSimulationDataGenerator()
{
}

void ExecutionTimeSimulationDataGenerator::Initialize( U32 simulation_sample_rate, ExecutionTimeAnalyzerSettings* settings )
{
    mSimulationSampleRateHz = simulation_sample_rate;
    mSettings = settings;

    mTimer = mSimulationChannels.Add( settings->mInputTimerChannel, mSimulationSampleRateHz, BIT_LOW );

    if( settings->mInputSubTimerChannel != UNDEFINED_CHANNEL )
        mSubTimer = mSimulationChannels.Add( settings->mInputSubTimerChannel, mSimulationSampleRateHz, BIT_LOW );
    else
        mSubTimer = NULL;

    mSimulationChannels.AdvanceAll(100); //insert 100 samples of idle
}

U32 ExecutionTimeSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel )
{
    U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

    while( mTimer->GetCurrentSampleNumber() < adjusted_largest_sample_requested)
    {
        CreateExecutionTime();
    }

    while(NULL != mSubTimer && mSubTimer->GetCurrentSampleNumber() < adjusted_largest_sample_requested)
    {
        CreateExecutionTime();
    }

    *simulation_channel = mSimulationChannels.GetArray();
    return mSimulationChannels.GetCount();
}

void ExecutionTimeSimulationDataGenerator::CreateSubExecutionTime(U32 ms)
{
    U32 samples = MillsecondsToSampleCount(ms, mSimulationSampleRateHz);

    mTimer->Advance(samples);
    if (NULL != mSubTimer) {
        mSubTimer->Transition();
        mSubTimer->Advance(samples);
    }
}

void ExecutionTimeSimulationDataGenerator::CreateExecutionTime()
{
    U32 samples_in_five_ms = MillsecondsToSampleCount(5, mSimulationSampleRateHz);
    mTimer->TransitionIfNeeded(BIT_HIGH);
    
    // advanced the clock a little bit
    mTimer->Advance(samples_in_five_ms);
    if (NULL != mSubTimer) {
        mSubTimer->Advance(samples_in_five_ms);
    }

    // Add a series of sub timers over the course of the timer
    for (int i = 0; i < 7; ++i) {
        CreateSubExecutionTime((rand() % 15) + 1);
    }

    mTimer->TransitionIfNeeded(BIT_LOW);

    // advanced the clock a little bit
    mTimer->Advance(samples_in_five_ms);
    if (NULL != mSubTimer) {
        mSubTimer->Advance(samples_in_five_ms);
    }
}
