#ifndef EXECUTIONTIMEANALYZER_ANALYZER_SETTINGS
#define EXECUTIONTIMEANALYZER_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class ExecutionTimeAnalyzerSettings : public AnalyzerSettings
{
public:
    ExecutionTimeAnalyzerSettings();
    virtual ~ExecutionTimeAnalyzerSettings();

    virtual bool SetSettingsFromInterfaces();
    void UpdateInterfacesFromSettings();
    virtual void LoadSettings( const char* settings );
    virtual const char* SaveSettings();

    
    Channel mInputTimerChannel;
    Channel mInputSubTimerChannel;
    bool mIncludeFirst;

protected:
    std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputTimerChannelInterface;
    std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputSubTimerChannelInterface;
    std::auto_ptr< AnalyzerSettingInterfaceBool >	mIncludeFirstBoolInterface;
};

#endif //EXECUTIONTIMEANALYZER_ANALYZER_SETTINGS
