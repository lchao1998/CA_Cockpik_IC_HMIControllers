#include "gauge_logic_controller.hpp"
#include "log/log.hpp"

GaugeLogicController::GaugeLogicController()
    : Module(),
    m_GaugeInfo(),
    m_GaugeClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_GAUGE)),
    m_GaugeClientAvailability(false)
{
    memset(&m_GaugeInfo, 0, sizeof(SM_Gauge));
    CHECKCLIENTAVAILABILITY(Gauge);

    m_GaugeClient->setPollable(true);
}

GaugeLogicController::~GaugeLogicController()
{
}

void GaugeLogicController::onProcess()
{
    m_GaugeClient->poll();
    if (m_GaugeClientAvailability) {
        uint8_t engineSpeedValid = m_GaugeClient->getAttribute("ESP_VehicleSpeedValid").toUInt();
        if (engineSpeedValid == 0) {
            CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_GaugeClient, gauge_, m_GaugeInfo, ESP_VehicleSpeed);
        }
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_GaugeClient, gauge_, m_GaugeInfo, EMS_EngineSpeed);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_GaugeClient, gauge_, m_GaugeInfo, TCU_GearForDisplay);
    }
}


