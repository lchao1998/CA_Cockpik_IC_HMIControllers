#include "common_logic_controller.hpp"
#include "sm_common.hpp"
#include <sys/stat.h>
#include <unistd.h>

CommonLogicController::CommonLogicController() : AppFw::Module(), m_CommonInfo(),
m_CommonClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_COMMON)),
m_CommonClientAvailability(false)
{
    memset(&m_CommonInfo, 0, sizeof(SM_Common));

    CHECKCLIENTAVAILABILITY(Common);

    m_CommonClient->setPollable(true);
}

CommonLogicController::~CommonLogicController()
{
    // TODO Auto-generated destructor stub
}



void CommonLogicController::onProcess()
{
    m_CommonClient->poll();
    if (m_CommonClientAvailability) {
        //uint8_t power_status = m_CommonClient->getAttribute("PEPS_PowerDistributionStatus").toUInt();
        //printf("...........receive PEPS_PowerDistributionStatus : %d.............\n", power_status);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_CommonClient, common_, m_CommonInfo, PEPS_PowerDistributionStatus);
    }
}


