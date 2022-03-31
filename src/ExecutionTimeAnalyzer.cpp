#include "ExecutionTimeAnalyzer.h"
#include "ExecutionTimeAnalyzerSettings.h"
#include <AnalyzerChannelData.h>
#include <sstream>

ExecutionTimeAnalyzer::ExecutionTimeAnalyzer()
:	Analyzer2(),  
    mSettings(new ExecutionTimeAnalyzerSettings()),
    mSimulationInitilized(false),
    mTimer(NULL),
    mSubTimer(NULL)
{
    SetAnalyzerSettings( mSettings.get() );
    UseFrameV2();
}

ExecutionTimeAnalyzer::~ExecutionTimeAnalyzer()
{
    KillThread();
}

void ExecutionTimeAnalyzer::SetupResults()
{
    mResults.reset( new ExecutionTimeAnalyzerResults( this, mSettings.get() ) );
    SetAnalyzerResults( mResults.get() );
    mResults->AddChannelBubblesWillAppearOn( mSettings->mInputTimerChannel );
    if( mSettings->mInputSubTimerChannel != UNDEFINED_CHANNEL ) {
        mResults->AddChannelBubblesWillAppearOn( mSettings->mInputSubTimerChannel );
    }
}

void ExecutionTimeAnalyzer::WorkerThread()
{
    U32 frame_counter = 0;
    mSampleRateHz = GetSampleRate();

    mTimer = GetAnalyzerChannelData( mSettings->mInputTimerChannel );
    if( mSettings->mInputSubTimerChannel != UNDEFINED_CHANNEL ) {
        mSubTimer = GetAnalyzerChannelData( mSettings->mInputSubTimerChannel );
    }

    if( mTimer->GetBitState() == BIT_HIGH )
        mTimer->AdvanceToNextEdge();


    for( ; ; )
    {
        mTimer->AdvanceToNextEdge(); //rising edge -- start of the timer
        U64 starting_sample = mTimer->GetSampleNumber();

        mTimer->AdvanceToNextEdge(); //rising edge -- start of the timer
        U64 ending_sample = mTimer->GetSampleNumber();

        FrameV2 frame_v2;
        frame_v2.AddInteger("index", frame_counter);

        // If we have a channel for a SubTimer process those frames
        if (NULL != mSubTimer) {
            mSubTimer->AdvanceToAbsPosition(starting_sample);

            // If we're skipping the first sample point, advanced to the first edge
            if (!mSettings->mIncludeFirst) {
                if (mSubTimer->WouldAdvancingToAbsPositionCauseTransition(ending_sample)) {
                    mSubTimer->AdvanceToNextEdge();
                } else {
                    mSubTimer->AdvanceToAbsPosition(ending_sample);
                }
            }

            U64 sub_frame_counter = 0;
            while (mSubTimer->GetSampleNumber() < ending_sample) {
                U64 sub_start_sample = mSubTimer->GetSampleNumber();
                if (mSubTimer->WouldAdvancingToAbsPositionCauseTransition(ending_sample)) {
                    mSubTimer->AdvanceToNextEdge();
                } else {
                    mSubTimer->AdvanceToAbsPosition(ending_sample);
                }
                U64 sub_end_sample = mSubTimer->GetSampleNumber();
                U64 sub_time = (sub_end_sample - sub_start_sample) / (mSampleRateHz / 1000);

                //Create a sub-timer frame 
                Frame sub_frame;
                sub_frame.mData1 = sub_time;
                sub_frame.mData2 = frame_counter;
                sub_frame.mFlags = 0;
                sub_frame.mType = 1;
                sub_frame.mStartingSampleInclusive = sub_start_sample;
                sub_frame.mEndingSampleInclusive = sub_end_sample;
                mResults->AddFrame(sub_frame);

                std::stringstream ss;
                ss << "sub_time_" << (sub_frame_counter++);
                frame_v2.AddInteger(ss.str().c_str(), sub_time);
            }
        }

        U64 total_time = (ending_sample - starting_sample) / (mSampleRateHz / 1000);
        frame_v2.AddInteger("total", total_time);
        mResults->AddFrameV2(frame_v2, "timer", starting_sample, ending_sample);

        // BUG: You must add this after the sub_frame or it causes display issues
        // Add the frame for the first Timer channel being active
        Frame frame;
        frame.mData1 = total_time;
        frame.mData2 = frame_counter;
        frame.mFlags = 0;
        frame.mType = 0;
        frame.mStartingSampleInclusive = starting_sample;
        frame.mEndingSampleInclusive = ending_sample;
        mResults->AddFrame(frame);

        frame_counter++;

        mResults->CommitResults();
        ReportProgress(ending_sample);
    }
}

bool ExecutionTimeAnalyzer::NeedsRerun()
{
    return false;
}

U32 ExecutionTimeAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
    if( mSimulationInitilized == false )
    {
        mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
        mSimulationInitilized = true;
    }

    return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 ExecutionTimeAnalyzer::GetMinimumSampleRateHz()
{
    return 1000;
}

const char* ExecutionTimeAnalyzer::GetAnalyzerName() const
{
    return "Execution Time Analyzer";
}

const char* GetAnalyzerName()
{
    return "Execution Time Analyzer";
}

Analyzer* CreateAnalyzer()
{
    return new ExecutionTimeAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
    delete analyzer;
}