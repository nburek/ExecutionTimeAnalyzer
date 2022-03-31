#include "ExecutionTimeAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "ExecutionTimeAnalyzer.h"
#include "ExecutionTimeAnalyzerSettings.h"
#include <iostream>
#include <fstream>

ExecutionTimeAnalyzerResults::ExecutionTimeAnalyzerResults( ExecutionTimeAnalyzer* analyzer, ExecutionTimeAnalyzerSettings* settings )
:	AnalyzerResults(),
    mSettings( settings ),
    mAnalyzer( analyzer )
{
}

ExecutionTimeAnalyzerResults::~ExecutionTimeAnalyzerResults()
{
}

void ExecutionTimeAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
    ClearResultStrings();
    Frame frame = GetFrame( frame_index );

    if (channel == mSettings->mInputTimerChannel && 0 == frame.mType) {
        char number_str[128];
        AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 32, number_str, 128 );
        AddResultString(number_str);
    } else if (channel == mSettings->mInputSubTimerChannel && 1 == frame.mType) {
        char number_str[128];
        AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 32, number_str, 128 );
        AddResultString(number_str);
    }

}

void ExecutionTimeAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
    //not supported
}

void ExecutionTimeAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
    //not supported
}

void ExecutionTimeAnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
    //not supported
}

void ExecutionTimeAnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
    //not supported
}