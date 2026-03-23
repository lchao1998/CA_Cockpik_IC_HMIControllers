#include "adas_logic_controller.hpp"

#include "sm_adas.hpp"
#include "sm_telltale.hpp"


AdasLogicController::AdasLogicController() : AppFw::Module(), m_AdasInfo(),
m_AdasClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_ADAS)),
m_TelltaleClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_TELLTALE)),
m_GaugeClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_GAUGE)),
m_GaugeClientAvailability(false),
m_TelltaleClientAvailability(false),
m_AdasClientAvailability(false)
{
    memset(&m_AdasInfo, 0, sizeof(SM_ADAS));

    CHECKCLIENTAVAILABILITY(Adas);
    CHECKCLIENTAVAILABILITY(Telltale);
    CHECKCLIENTAVAILABILITY(Gauge);

    m_GaugeClient->setPollable(true);
    m_AdasClient->setPollable(true);
    m_TelltaleClient->setPollable(true);
}


AdasLogicController::~AdasLogicController()
{
    // TODO Auto-generated destructor stub
}


void AdasLogicController::onProcess()
{
    m_AdasClient->poll();
    m_TelltaleClient->poll();
    m_GaugeClient->poll();

    if (m_AdasClientAvailability) {

        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_AdasClient, Adas_, m_AdasInfo, ACC_ACCMode);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_AdasClient, Adas_, m_AdasInfo, ACC_LaneChangeStatus);
        
        const uint8_t tmp_ACC_ACCMode = static_cast<uint8_t>(m_AdasClient->getAttribute("ACC_ACCMode").toInt());
        if (tmp_ACC_ACCMode >= 3 && tmp_ACC_ACCMode < 7)
        {
            const uint8_t tmp_ACC_LaneChangeStatus = static_cast<uint8_t>(m_AdasClient->getAttribute("ACC_LaneChangeStatus").toInt());
            // turn right
            uint8_t tmp_BCM_TurnIndicatorRight = 0;
            // ture left
            uint8_t tmp_BCM_TurnIndicatorLeft = 0;
            // speed 
            uint8_t tmp_ESP_VehicleSpeedValid = 0;
            uint8_t tmp_EMS_EngineSpeed = 0;
            uint8_t tmp_ACC_HostTargetDetection = static_cast<uint8_t>(m_AdasClient->getAttribute("ACC_HostTargetDetection").toInt());;
            uint8_t tmp_ACC_ACCTargetType = static_cast<uint8_t>(m_AdasClient->getAttribute("ACC_ACCTargetType").toInt());
            
            if ((tmp_ACC_HostTargetDetection == 2) && (tmp_ACC_ACCTargetType == 1 || tmp_ACC_ACCTargetType == 2 || tmp_ACC_ACCTargetType ==3 ||
                tmp_ACC_ACCTargetType == 4 || tmp_ACC_ACCTargetType == 5 || tmp_ACC_ACCTargetType == 7)) 
            {
                // ACC_ACCTargetLngRange
                const uint16_t tmp_ACC_ACCTargetLngRange  = static_cast<uint16_t>(m_AdasClient->getAttribute("ACC_ACCTargetLngRange").toInt());
                if(tmp_ACC_ACCTargetLngRange >= 170 && tmp_ACC_ACCTargetLngRange <= 180)
                {
                    Adas_ACC_ACCTargetLngRange.setValue(3);
                }
                else if(tmp_ACC_ACCTargetLngRange >= 80 && tmp_ACC_ACCTargetLngRange <= 90)
                {
                    Adas_ACC_ACCTargetLngRange.setValue(2);
                }
                else if(tmp_ACC_ACCTargetLngRange >= 30 && tmp_ACC_ACCTargetLngRange <= 40)
                {
                    Adas_ACC_ACCTargetLngRange.setValue(1);
                }
                else
                {
                    Adas_ACC_ACCTargetLngRange.setValue(0);
                }
            }
       
            if(m_GaugeClientAvailability)
            {
                tmp_ESP_VehicleSpeedValid = m_GaugeClient->getAttribute("ESP_VehicleSpeedValid").toUInt();
                if (tmp_ESP_VehicleSpeedValid)
                {
                    tmp_EMS_EngineSpeed = m_GaugeClient->getAttribute("EMS_EngineSpeed").toUInt();
                }  
            }

            if (m_TelltaleClientAvailability)
            {
                tmp_BCM_TurnIndicatorRight = static_cast<uint8_t>(m_TelltaleClient->getAttribute("BCM_TurnIndicatorRight").toInt());
                tmp_BCM_TurnIndicatorLeft = static_cast<uint8_t>(m_TelltaleClient->getAttribute("BCM_TurnIndicatorLeft").toInt());
            }
            
            if (tmp_ACC_LaneChangeStatus == 1 && tmp_BCM_TurnIndicatorRight == 1)
            {
                Adas_ACC_VehicleAnimationStatus.setValue(2);
            }
            else if (tmp_ACC_LaneChangeStatus == 1 && tmp_BCM_TurnIndicatorLeft == 1)
            {
                Adas_ACC_VehicleAnimationStatus.setValue(3);
            }
            else if (tmp_ACC_LaneChangeStatus == 2)
            {
                Adas_ACC_VehicleAnimationStatus.setValue(0);
            }
            else if (tmp_EMS_EngineSpeed > 0)
            {
                Adas_ACC_VehicleAnimationStatus.setValue(1);
            }
            else 
            {
                Adas_ACC_VehicleAnimationStatus.setValue(0);
            }
        }
        else 
        {
            Adas_ACC_VehicleAnimationStatus.setValue(0);
        }
        
        const uint8_t tmp_ACC_HostTargetDetection = static_cast<uint8_t>(m_AdasClient->getAttribute("ACC_HostTargetDetection").toInt());
        if (tmp_ACC_HostTargetDetection == 2)
        {
            CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_AdasClient, Adas_, m_AdasInfo, ACC_ACCTargetType);
        }
        else 
        {
            Adas_ACC_ACCTargetType.setValue(0);
        }
    }
}

