#ifndef EXECUTIONTIMEANALYZER_ANALYZER_H
#define EXECUTIONTIMEANALYZER_ANALYZER_H

#include <Analyzer.h>
#include "ExecutionTimeAnalyzerResults.h"
#include "ExecutionTimeSimulationDataGenerator.h"

class ExecutionTimeAnalyzerSettings;
class ANALYZER_EXPORT ExecutionTimeAnalyzer : public Analyzer2
{
public:
    ExecutionTimeAnalyzer();
    virtual ~ExecutionTimeAnalyzer();

    virtual void SetupResults();
    virtual void WorkerThread();

    virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
    virtual U32 GetMinimumSampleRateHz();

    virtual const char* GetAnalyzerName() const;
    virtual bool NeedsRerun();

protected: //vars
    std::auto_ptr< ExecutionTimeAnalyzerSettings > mSettings;
    std::auto_ptr< ExecutionTimeAnalyzerResults > mResults;
    AnalyzerChannelData* mTimer;
    AnalyzerChannelData* mSubTimer;

    ExecutionTimeSimulationDataGenerator mSimulationDataGenerator;
    bool mSimulationInitilized;

    //Serial analysis vars:
    U32 mSampleRateHz;
    U32 mStartOfStopBitOffset;
    U32 mEndOfStopBitOffset;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //EXECUTIONTIMEANALYZER_ANALYZER_H
