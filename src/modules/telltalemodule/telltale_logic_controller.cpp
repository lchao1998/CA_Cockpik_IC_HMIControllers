
#include "telltale_logic_controller.hpp"
#include "log/log.hpp"
#include "sm_telltale.hpp"

TelltaleLogicController::TelltaleLogicController() :
        AppFw::Module(),
        m_TelltaleInfo(),
        m_TelltaleClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_TELLTALE)),
        m_GaugeClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_GAUGE)),
        m_TelltaleClientAvailability(false),
        m_GaugeClientAvailability(false)
{
    memset(&m_TelltaleInfo, 0, sizeof(SM_Telltale));

    CHECKCLIENTAVAILABILITY(Telltale);
    CHECKCLIENTAVAILABILITY(Gauge);

    m_TelltaleClient->setPollable(true);
    m_GaugeClient->setPollable(true);
}

TelltaleLogicController::~TelltaleLogicController()
{

}


void TelltaleLogicController::onProcess()
{
    m_TelltaleClient->poll();
    m_GaugeClient->poll();

    if (m_TelltaleClientAvailability)
    {
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TelltaleClient, telltale_, m_TelltaleInfo, EMS_CoolanTemperatureError);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TelltaleClient, telltale_, m_TelltaleInfo, EMS_OilPressureSts);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TelltaleClient, telltale_, m_TelltaleInfo, EMS_SVS);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TelltaleClient, telltale_, m_TelltaleInfo, EMS_MIL);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TelltaleClient, telltale_, m_TelltaleInfo, ESP_ABSFailStatus);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TelltaleClient, telltale_, m_TelltaleInfo, SRS_WarningLampStatus);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TelltaleClient, telltale_, m_TelltaleInfo, SRS_BuckleSwitchStatus);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TelltaleClient, telltale_, m_TelltaleInfo, EPB_FunctionLamp);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TelltaleClient, telltale_, m_TelltaleInfo, BCM_BatteryStatus);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TelltaleClient, telltale_, m_TelltaleInfo, BCM_DoorStatus);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TelltaleClient, telltale_, m_TelltaleInfo, BCM_HighBeamStatus);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TelltaleClient, telltale_, m_TelltaleInfo, BCM_LowBeamStatus);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TelltaleClient, telltale_, m_TelltaleInfo, BCM_TurnIndicatorRight);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TelltaleClient, telltale_, m_TelltaleInfo, BCM_TurnIndicatorLeft);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TelltaleClient, telltale_, m_TelltaleInfo, BCM_RearFoglampStatus);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TelltaleClient, telltale_, m_TelltaleInfo, BCM_FrontFoglampStatus);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TelltaleClient, telltale_, m_TelltaleInfo, BCM_PositionLampStatus);


        // SRS_BuckleSwitchStatus
        const uint8_t tmp_SRS_DriverBuckleSwitchStatus = static_cast<uint8_t>(m_TelltaleClient->getAttribute("SRS_DriverBuckleSwitchStatus").toInt());
        const uint8_t tmp_SRS_PassengerBuckleSwitchStatus = static_cast<uint8_t>(m_TelltaleClient->getAttribute("SRS_PassengerBuckleSwitchStatus").toInt());

        if (m_GaugeClientAvailability)
        {
            if(tmp_SRS_DriverBuckleSwitchStatus == 1 || tmp_SRS_PassengerBuckleSwitchStatus == 1)
            {
                telltale_SRS_BuckleSwitchStatus.setValue(1);
            }else
            {
                telltale_SRS_BuckleSwitchStatus.setValue(0);
            }
        }
        
        // BCM_BatteryStatus
        const uint8_t tmp_BCM_PMSLevel = static_cast<uint8_t>(m_TelltaleClient->getAttribute("BCM_PMSLevel").toInt());
        const uint8_t tmp_BCM_BSNLevel = static_cast<uint8_t>(m_TelltaleClient->getAttribute("BCM_BSNLevel").toInt());
        if(tmp_BCM_PMSLevel == 1 || tmp_BCM_PMSLevel == 2 || tmp_BCM_BSNLevel == 1 || tmp_BCM_BSNLevel ==2)
        {
            telltale_BCM_BatteryStatus.setValue(1);
        }else
        {
            telltale_BCM_BatteryStatus.setValue(0);
        }

        // BCM_DoorStatus
        const uint8_t tmp_BCM_PassengerDoorStatus = static_cast<uint8_t>(m_TelltaleClient->getAttribute("BCM_PassengerDoorStatus").toInt());
        const uint8_t tmp_BCM_RightRearDoorStatus = static_cast<uint8_t>(m_TelltaleClient->getAttribute("BCM_RightRearDoorStatus").toInt());
        const uint8_t tmp_BCM_TrunkDoorStatus = static_cast<uint8_t>(m_TelltaleClient->getAttribute("BCM_TrunkDoorStatus").toInt());
        const uint8_t tmp_BCM_LeftRearDoorStatus = static_cast<uint8_t>(m_TelltaleClient->getAttribute("BCM_LeftRearDoorStatus").toInt());
        const uint8_t tmp_BCM_DriverDoorStatus = static_cast<uint8_t>(m_TelltaleClient->getAttribute("BCM_DriverDoorStatus").toInt());
        if(tmp_BCM_PassengerDoorStatus == 1 || tmp_BCM_RightRearDoorStatus == 1 
         || tmp_BCM_TrunkDoorStatus == 1 || tmp_BCM_LeftRearDoorStatus ==1 || tmp_BCM_DriverDoorStatus ==1)
        {
            telltale_BCM_DoorStatus.setValue(1);
        }else
        {
            telltale_BCM_DoorStatus.setValue(0);
        }

         // TCU_TransStatus
        const uint8_t tmp_TCU_TransFailureStatus = static_cast<uint8_t>(m_TelltaleClient->getAttribute("TCU_TransFailureStatus").toInt());
        if(tmp_TCU_TransFailureStatus == 1 || tmp_TCU_TransFailureStatus == 2)
        {
            telltale_TCU_TransFailureStatus.setValue(1);
        }else
        {
            telltale_TCU_TransFailureStatus.setValue(0);
        }

    }
}
