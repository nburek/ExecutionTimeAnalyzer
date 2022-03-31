#include "ExecutionTimeAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


ExecutionTimeAnalyzerSettings::ExecutionTimeAnalyzerSettings()
:	mInputTimerChannel( UNDEFINED_CHANNEL ),
    mInputSubTimerChannel( UNDEFINED_CHANNEL ),
    mIncludeFirst (false)
{
    mInputTimerChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
    mInputTimerChannelInterface->SetTitleAndTooltip( "Timer", "The Timer channel to record time while high (active)" );
    mInputTimerChannelInterface->SetChannel( mInputTimerChannel );

    mInputSubTimerChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
    mInputSubTimerChannelInterface->SetTitleAndTooltip( "SubTimer", "The SubTimer to record periods of high/low during Timer active." );
    mInputSubTimerChannelInterface->SetChannel( mInputSubTimerChannel );
	mInputSubTimerChannelInterface->SetSelectionOfNoneIsAllowed( true );

    mIncludeFirstBoolInterface.reset( new AnalyzerSettingInterfaceBool() );
    mIncludeFirstBoolInterface->SetTitleAndTooltip( "Include First Value", "If false the time between the rising edge of the Timer channel and the first edge of the SubTimer channel is ignored." );
    mIncludeFirstBoolInterface->SetValue( mIncludeFirst );


    AddInterface( mInputTimerChannelInterface.get() );
    AddInterface( mInputSubTimerChannelInterface.get() );
    AddInterface( mIncludeFirstBoolInterface.get() );

    ClearChannels();
    AddChannel( mInputTimerChannel, "Timer", false );
    AddChannel( mInputSubTimerChannel, "SubTimer", false );
}

ExecutionTimeAnalyzerSettings::~ExecutionTimeAnalyzerSettings()
{
}

bool ExecutionTimeAnalyzerSettings::SetSettingsFromInterfaces()
{
    mInputTimerChannel = mInputTimerChannelInterface->GetChannel();
    mInputSubTimerChannel = mInputSubTimerChannelInterface->GetChannel();
    mIncludeFirst = mIncludeFirstBoolInterface->GetValue();

    ClearChannels();
    AddChannel( mInputTimerChannel, "Timer", true );
    AddChannel( mInputSubTimerChannel, "SubTimer", true );

    return true;
}

void ExecutionTimeAnalyzerSettings::UpdateInterfacesFromSettings()
{
    mInputTimerChannelInterface->SetChannel( mInputTimerChannel );
    mInputSubTimerChannelInterface->SetChannel( mInputSubTimerChannel );
    mIncludeFirstBoolInterface->SetValue( mIncludeFirst );
}

void ExecutionTimeAnalyzerSettings::LoadSettings( const char* settings )
{
    SimpleArchive text_archive;
    text_archive.SetString( settings );

    text_archive >> mInputTimerChannel;
    text_archive >> mInputSubTimerChannel;
    text_archive >> mIncludeFirst;

    ClearChannels();
    AddChannel( mInputTimerChannel, "Timer", true );
    AddChannel( mInputSubTimerChannel, "SubTimer", true );

    UpdateInterfacesFromSettings();
}

const char* ExecutionTimeAnalyzerSettings::SaveSettings()
{
    SimpleArchive text_archive;

    text_archive << mInputTimerChannel;
    text_archive << mInputSubTimerChannel;
    text_archive << mIncludeFirst;

    return SetReturnString( text_archive.GetString() );
}
