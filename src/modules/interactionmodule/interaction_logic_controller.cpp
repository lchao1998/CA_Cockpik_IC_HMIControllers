#include "interaction_logic_controller.hpp"
#include "log/log.hpp"

#define MENU_BEGIN 1
#define MENU_END 3


InteractionLogicController::InteractionLogicController()
    : Module(),
    m_InteractionInfo(),
    m_AdasInfo(),
    m_InteractionClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_KEYBOARD)),
    m_AdasClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_ADAS)),
    m_GvmClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_GVM_ID, IPC_SERVICE_INSTANCE_GVM)),
    m_AdasClientAvailability(false),
    m_InteractionClientAvailability(false),
    m_GvmClientAvailability(false)
{
    memset(&m_InteractionInfo, 0, sizeof(SM_KeyBoard));
    memset(&m_AdasInfo, 0, sizeof(SM_ADAS));

    CHECKCLIENTAVAILABILITY(Interaction);
    CHECKCLIENTAVAILABILITY(Adas);
    CHECKCLIENTAVAILABILITY(Gvm);

    m_InteractionClient->setPollable(true);
    m_AdasClient->setPollable(true);
    m_GvmClient->setPollable(true);
}

InteractionLogicController::~InteractionLogicController()
{
}

void InteractionLogicController::onProcess()
{
    m_InteractionClient->poll();
    m_AdasClient->poll();
    m_GvmClient->poll();

    bool gvm_switch_state = false;
    bool adas_switch_state = false;

    if (m_GvmClientAvailability)
    {
        gvm_switch_state = m_GvmClient->getAttribute("gvm_switch_state").toBool();
    }

    if (gvm_switch_state != true)
    {
        if (m_AdasClientAvailability)
        {
            uint8_t tmp_ACC_ACCMode = m_AdasClient->getAttribute("ACC_ACCMode").toUInt();
            if (tmp_ACC_ACCMode >= 3 && tmp_ACC_ACCMode < 7)
            {
                interaction_status_Window_Status.setValue(1); 
            }
            else
            {
                interaction_status_Window_Status.setValue(0);
            }
        }
    }
    else
    {
        interaction_status_Window_Status.setValue(2);
    }

    if(interaction_status_Window_Status.getValue() == 0)
    {
        if (m_InteractionClientAvailability) 
        {
            processMenukeyPress();
        }
    }
}

void InteractionLogicController::processMenukeyPress()
{
    uint8_t tmp_GW_MFS_UP_switch = m_InteractionClient->getAttribute("GW_MFS_UP_switch").toUInt();
    uint8_t tmp_GW_MFS_Down_switch = m_InteractionClient->getAttribute("GW_MFS_Down_switch").toUInt();
    if (tmp_GW_MFS_Down_switch == 1)
    {
        --m_MenuIndex;
        if (m_MenuIndex < MENU_BEGIN)
        {
            m_MenuIndex = MENU_END;
        }
        interaction_status_Menu_Status.setValue(m_MenuIndex);
        logDebug("tmp_GW_MFS_Down_switch: [%d]", interaction_status_Menu_Status.getValue());
    }
    else if (tmp_GW_MFS_UP_switch == 1)
    {
        ++m_MenuIndex;
        if (m_MenuIndex > MENU_END)
        {
            m_MenuIndex = MENU_BEGIN;
        }
        interaction_status_Menu_Status.setValue(m_MenuIndex);

        logDebug("tmp_GW_MFS_UP_switch: [%d]", interaction_status_Menu_Status.getValue());
    }   

    
}


