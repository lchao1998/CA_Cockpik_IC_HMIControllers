#include "chime_logic_controller.hpp"
#include "auto_sound_service.hpp"

using namespace AutoSoundService;

#define  TURN_ON   1
#define  TURN_OFF  0
#define  WARNING_SPEED 20
ChimeLogicController::ChimeLogicController()
    : m_SoundClient(AutoSOCM::getApplication()->createClient(IPC_SOUND_SERVICE_ID,IPC_SERVICE_INSTANCE_SOUND)),
	  m_TelltaleClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_TELLTALE)),
      m_GaugeClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_GAUGE)),
      m_TelltaleClientAvailability(false),
      m_GaugeClientAvailability(false),
      m_SoundClientAvailability(false),
      m_TurnFlag(false),
      m_BuckleSwitchWarningFlag(false)
{
    CHECKCLIENTAVAILABILITY(Gauge);
    CHECKCLIENTAVAILABILITY(Sound);
    CHECKCLIENTAVAILABILITY(Telltale);

    m_TelltaleClient->setPollable(true);
    m_GaugeClient->setPollable(true);
}

ChimeLogicController::~ChimeLogicController()
{

}

void ChimeLogicController::onProcess()
{
    m_TelltaleClient->poll();
    m_GaugeClient->poll();

    if(m_SoundClientAvailability) 
    {
   
        const uint16_t tmp_BCM_TurnIndicatorLeft = static_cast<uint16_t>(m_TelltaleClient->getAttribute("BCM_TurnIndicatorLeft").toUInt());
        const uint16_t tmp_BCM_TurnIndicatorRight = static_cast<uint16_t>(m_TelltaleClient->getAttribute("BCM_TurnIndicatorRight").toUInt());
        const uint16_t tmp_SRS_DriverBuckleSwitchStatus = static_cast<uint16_t>(m_TelltaleClient->getAttribute("SRS_DriverBuckleSwitchStatus").toUInt());
        const uint16_t tmp_SRS_PassengerBuckleSwitchStatus = static_cast<uint16_t>(m_TelltaleClient->getAttribute("SRS_PassengerBuckleSwitchStatus").toUInt());
        const uint16_t tmp_vehicleSpeed = static_cast<uint16_t>(m_GaugeClient->getAttribute("ESP_VehicleSpeed").toUInt());
        
        // TurnIndicator  warning sound
        if(m_TurnFlag == true)
        {
            if (tmp_BCM_TurnIndicatorLeft == TURN_OFF && tmp_BCM_TurnIndicatorRight == TURN_OFF)
            {            
                logDebug("===========Indicator_OFF===============");
                auto parameter = GetPlayParams(SoundType::Indicator_OFF,
                                                PlayPriority::Normal,
                                                PlayType::TIMES,
                                                0,1);

                m_SoundClient->invokeMethodAsync("reqPlaySnd",  parameter,
                            [&](const AutoSOCM::Data& returnValue){
                });
                m_TurnFlag = false;
             }
            
        }    
        else 
        {
            if (tmp_BCM_TurnIndicatorLeft == TURN_ON || tmp_BCM_TurnIndicatorRight == TURN_ON)
            {          
                logDebug("===========Indicator_ON===============");
                auto parameter = GetPlayParams(SoundType::Indicator_ON,
                                            PlayPriority::Normal,
                                            PlayType::TIMES,
                                            0,1);

                m_SoundClient->invokeMethodAsync("reqPlaySnd",  parameter,
                        [&](const AutoSOCM::Data& returnValue){
                    
                    });
                m_TurnFlag = true;
            }
        }

        // BuckleSwitch warning sound
        if (m_BuckleSwitchWarningFlag == false)
        {
            if((tmp_SRS_DriverBuckleSwitchStatus == TURN_ON || tmp_SRS_PassengerBuckleSwitchStatus == TURN_ON) && (tmp_vehicleSpeed > WARNING_SPEED))
            {
                auto parameter = GetPlayParams(SoundType::BuckleSwitchWarning,
                                           PlayPriority::High,
                                           PlayType::DURATION,
                                           100);
                logDebug("=============play BuckleSwitch warning sound=============");
                m_SoundClient->invokeMethodAsync("reqPlaySnd",  parameter,
                    [&](const AutoSOCM::Data& returnValue){

                });
                m_BuckleSwitchWarningFlag = true;
            }
            
        }
        else
        {
            if(((tmp_SRS_DriverBuckleSwitchStatus == TURN_OFF) && (tmp_SRS_PassengerBuckleSwitchStatus == TURN_OFF)) || (tmp_vehicleSpeed <= WARNING_SPEED ))
            {
         
                auto parameter = GetStopParams(StopType::IMMEDIATELY);
                logDebug("=============stop BuckleSwitch warning sound=============");
                m_SoundClient->invokeMethodAsync("reqStopSnd",  parameter,
                        [&](const AutoSOCM::Data& returnValue){
                            
                        });
                m_BuckleSwitchWarningFlag = false;
            }
        }
    }
}

