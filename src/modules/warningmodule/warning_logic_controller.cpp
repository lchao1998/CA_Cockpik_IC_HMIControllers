#include "warning_logic_controller.hpp"
#include "log/log.hpp"
#include "warning_model.hpp"
#include "sm_warning.hpp"



#define CHECK_WARNING(type)                                                    \
    if (m_warningMap.find(WID_##type) != m_warningMap.end())                 \
    {                                                                          \
        checkWarning(m_WarningInfo.type, m_warningMap[WID_##type], #type);   \
    }

const uint32_t WARING_PROCESS_INTERVAL = 100U;

WarningLogicController::WarningLogicController(void):
        AppFw::Module(),
        m_WarningInfo(),
        m_WarningClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_WARNING)),
        m_AdasClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_ADAS)),
        m_GaugeClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_GAUGE)),
        m_AdasClientAvailability(false),
        m_WarningClientAvailability(false),
        m_GaugeClientAvailability(false),
        m_warningTimer(this,  WARING_PROCESS_INTERVAL, [&](){onWarningTimer();}),
        m_warningMap(),
        m_bWarningTimerOn(false),
        m_currentWarningInfo(NULL)
{
    memset(&m_WarningInfo, 0, sizeof(SM_Warning));
    initWarningData();

    CHECKCLIENTAVAILABILITY(Warning);
    CHECKCLIENTAVAILABILITY(Adas);
    CHECKCLIENTAVAILABILITY(Gauge);

    m_WarningClient->setPollable(true);
    m_AdasClient->setPollable(true);
    m_GaugeClient->setPollable(true);

    startWarningTimer();

}

WarningLogicController::~WarningLogicController(void)
{
    stopWarningTimer();
}

void WarningLogicController::initWarningData()
{
    m_warningMap = WarningInfoModel::Instance().getModel();
}

void WarningLogicController::startWarningTimer()
{
    logDebug("WarningLogicController::startWarningTimer");
    if(!m_bWarningTimerOn)
    {
        logDebug("start warning timer...");
        m_warningTimer.start();
        m_bWarningTimerOn = true;
    }
}

void WarningLogicController::stopWarningTimer()
{
    if(m_bWarningTimerOn)
    {
        logDebug("stop warning timer...");
        m_warningTimer.stop();
        m_bWarningTimerOn = false;
    }
}

void WarningLogicController::onProcess(void)
{
    m_WarningClient->poll();
    m_AdasClient->poll();
    m_GaugeClient->poll();

    if (m_WarningClientAvailability)
    {
        checkWarnings();
    }
    // do something to check ADAS ........
    if(m_AdasClientAvailability)
    {
        checkAdasWarnings();
    }

    if (m_GaugeClientAvailability)
    {
        const uint8_t tmp_SRS_DriverBuckleSwitchStatus = static_cast<uint8_t>(m_WarningClient->getAttribute("SRS_DriverBuckleSwitchStatus").toInt());
        const uint8_t tmp_SRS_PassengerBuckleSwitchStatus = static_cast<uint8_t>(m_WarningClient->getAttribute("SRS_PassengerBuckleSwitchStatus").toInt());

        const uint16_t tmp_vehicleSpeed = static_cast<uint16_t>(m_GaugeClient->getAttribute("ESP_VehicleSpeed").toUInt());
        
        if((tmp_SRS_DriverBuckleSwitchStatus == 1) && (tmp_vehicleSpeed > 20 ))
        {
            pushWarningInfo(m_warningMap[WID_SRS_DriverBuckleSwitchStatus]);
        }else
        {
            popWarningInfo(m_warningMap[WID_SRS_DriverBuckleSwitchStatus]);
        }

        if((tmp_SRS_PassengerBuckleSwitchStatus == 1 ) && (tmp_vehicleSpeed > 20 ))
        {
            pushWarningInfo(m_warningMap[WID_SRS_PassengerBuckleSwitchStatus]);
        }else
        {
            popWarningInfo(m_warningMap[WID_SRS_PassengerBuckleSwitchStatus]);
        }
    }
    
}

void WarningLogicController::onWarningTimer()
{
    if(m_warningQueue.size() == 0)
    {
        //logDebug("WarningLogicController:Warning queue is empty.");
        return;
    }

    if(m_currentWarningInfo == NULL)
    {
        logDebug("onWarningTimer:m_currentWarningInfo is empty.");
        showWarningInfo();
        return;
    }

    m_currentWarningInfo->setShowTime(m_currentWarningInfo->getShowTime() - WARING_PROCESS_INTERVAL);
    //logDebug("---------------m_currentWarningInfo showtime [%d]------", m_currentWarningInfo->getShowTime());
    if(m_currentWarningInfo->getShowTime() <= 0)
    {
        logDebug("-----onWarningTimer: show next warninginfo.----");
        m_currentWarningInfo->resetShowTime();
        if (m_warningQueue.size() != 1)
        {
            popWarningInfo(m_currentWarningInfo);
        }
        m_currentWarningInfo = NULL;
        showWarningInfo();
        if(m_currentWarningInfo == NULL)
        {
            logDebug("----onWarningTimer: no next warninginfo.----");
            warning_Flash_Flag.setValue(0);
            warning_Showing_ID.setValue(0);
        }
    }
}

void WarningLogicController::checkWarnings()
{
    //CHECK_WARNING(SRS_DriverBuckleSwitchStatus);
    //CHECK_WARNING(SRS_PassengerBuckleSwitchStatus);
    CHECK_WARNING(EMS_MIL);
    CHECK_WARNING(SRS_WarningLampStatus);
}

void WarningLogicController::checkAdasWarnings()
{
    bool bOk = false;
    static uint8_t ACC_Range_Flag = 0;
    const uint8_t tmp_ACC_ACCMode = static_cast<uint8_t>(m_AdasClient->getAttribute("ACC_ACCMode").toInt());
    if (tmp_ACC_ACCMode >= 3 && tmp_ACC_ACCMode < 7) 
    {
        uint8_t tmp_ACC_HostTargetDetection = static_cast<uint8_t>(m_AdasClient->getAttribute("ACC_HostTargetDetection").toInt());;
        uint8_t tmp_ACC_ACCTargetType = static_cast<uint8_t>(m_AdasClient->getAttribute("ACC_ACCTargetType").toInt());
        
        if ((tmp_ACC_HostTargetDetection == 2) && (tmp_ACC_ACCTargetType == 1 || tmp_ACC_ACCTargetType == 2 || tmp_ACC_ACCTargetType ==3 ||
            tmp_ACC_ACCTargetType == 4 || tmp_ACC_ACCTargetType == 5 || tmp_ACC_ACCTargetType == 7)) 
        {
            const uint16_t tmp_ACC_ACCTargetLngRange  = static_cast<uint16_t>(m_AdasClient->getAttribute("ACC_ACCTargetLngRange").toInt(&bOk));
            //logDebug("ACC_ACCTargetLngRange value : [%d] [%d] tmp_ACC_ACCTargetType value : [%d]", tmp_ACC_ACCTargetLngRange, bOk);
            if(tmp_ACC_ACCTargetLngRange != 0)
            {
                //logInfo("ACC_ACCTargetLngRange value : [%d] [%d]", tmp_ACC_ACCTargetLngRange, bOk);
            }
            if(bOk)
            {
                // ACC_ACCTargetLngRange
                if(tmp_ACC_ACCTargetLngRange >= 80 && tmp_ACC_ACCTargetLngRange <= 180)
                {
                    ACC_Range_Flag = 5U;
                    pushWarningInfo(m_warningMap[WID_ACC_ACCTargetLngRange_180]);
                    // logInfo("Warning The obstacle is about 180 meters");
                }
                else if(tmp_ACC_ACCTargetLngRange >= 30 && tmp_ACC_ACCTargetLngRange < 80)
                {
                    ACC_Range_Flag = 6U;
                    pushWarningInfo(m_warningMap[WID_ACC_ACCTargetLngRange_80]);
                    // logInfo("Warning The obstacle is about 80 meters");
                }
                else if(tmp_ACC_ACCTargetLngRange > 0 && tmp_ACC_ACCTargetLngRange < 30)
                {
                    ACC_Range_Flag = 7U;
                    pushWarningInfo(m_warningMap[WID_ACC_ACCTargetLngRange_30]);
                    // logInfo("Warning The obstacle is about 30 meters");
                }
                else
                {
                    popWarningInfo(m_warningMap[static_cast <WarningID>(ACC_Range_Flag)]);
                    ACC_Range_Flag = 0;
                }
            }
        }
    }
    else
    {   
        if (ACC_Range_Flag != 0)
        {
            lock_guard<mutex> lock(m_queueMutex);
            for(auto itor = m_warningQueue.begin(); itor != m_warningQueue.end();)
            {
                if(((WarningInfo*)(*itor))->getmodeFlag() == ADAS_MODE)
                {
                    itor = m_warningQueue.erase(itor);
                }
                else
                {
                    itor++;
                }
            }
        }
        warning_Showing_ID.setValue(0);
        showWarningInfo();
    }
}

void WarningLogicController::checkWarning(uint8_t& value, WarningInfo* info, const char* name)
{
    bool bOk = false;
    const uint8_t tmp = static_cast<uint8_t>(m_WarningClient->getAttribute(name).toInt(&bOk));
    if(bOk)
    {
        value = tmp;
        if(value != 0)
        {
            pushWarningInfo(info);
        }
        else
        {
            popWarningInfo(info);
        }
    }
}

void WarningLogicController::pushWarningInfo(WarningInfo* const info)
{
    //logDebug("pushWarningInfo id[%d], flash[%d]", info->getID(), info->getFlashFlag());
     if(info == NULL)
    {
        logWarning("WarningLogicController:Push warning info is NULL");
        return ;
    }
    lock_guard<mutex> lock(m_queueMutex);
    bool bPush = true;
    if(m_warningQueue.size() > 0)
    {
       vector<WarningInfo*>::iterator itor = m_warningQueue.begin();
        for(itor; itor != m_warningQueue.end(); itor++)
        {
            if(((WarningInfo*)(*itor))->getID() == info->getID())
            {
                //logDebug("WarningLogicController: warninginfo object is exist [%d]", info->getID());
                bPush = false;
                break;
            }
        }
    }
    
    if(bPush)
    {
        m_warningQueue.push_back(info);

        // info->printInfo("Push info");
        // logDebug("----warninginfo Queue start %d---", m_warningQueue.size());
        // int queueIndex = 0;
        // for(auto itor = m_warningQueue.begin(); itor != m_warningQueue.end(); itor++)
        // {
        //     logDebug("[%d: %d] ", queueIndex++ , ((WarningInfo*)(*itor))->getID());
        // }
        // logDebug("----warninginfo Queue end---");
    }

}

void WarningLogicController::popWarningInfo(WarningInfo* const info)
{
    if(info == NULL)
    {
        //logWarning("WarningLogicController:Pop warning info is NULL");
        return ;
    }
    if(m_warningQueue.size() <= 0)
    {
        //logDebug("WarningLogicController:Warning Queue is NULL");
        return;
    }
    lock_guard<mutex> lock(m_queueMutex);
    vector<WarningInfo*>::iterator itor = m_warningQueue.begin();
    for(itor; itor != m_warningQueue.end(); itor++)
    {
        if(((WarningInfo*)(*itor))->getID() == info->getID())
        {
            m_warningQueue.erase(itor);
            if(warning_Showing_ID.getValue() != 0)
            {
                warning_Showing_ID.setValue(0);
                warning_Flash_Flag.setValue(0);
            }
            logDebug("WarningLogicController: Erase a warninginfo [%d]", info->getID());
            break;
        }
    }

}

void WarningLogicController::showWarningInfo()
{
    if(m_warningQueue.size() <= 0)
    {
        logDebug("There is no warninginfo in dequeue");
        return;
    }
    m_currentWarningInfo = m_warningQueue.front();
    if(m_currentWarningInfo == NULL)
    {
        logError("Get warninginfo error!!!");
        if(warning_Showing_ID.getValue() != 0)
        {
            warning_Showing_ID.setValue(0);
            warning_Flash_Flag.setValue(0);
        }
        return;
    }
    showWarningOnHMI(m_currentWarningInfo);
}


void WarningLogicController::showWarningOnHMI(WarningInfo* const info)
{
    logDebug("show warning info on HMI [id:%d], [flash:%d] current:[id:%d-flash:%d]", info->getID(), info->getFlashFlag(), warning_Showing_ID.getValue(), warning_Flash_Flag.getValue());
    if ((info->getID() != warning_Showing_ID.getValue()) || (info->getFlashFlag() != warning_Flash_Flag.getValue()))
    {
        warning_Flash_Flag.setValue(info->getFlashFlag());
        warning_Showing_ID.setValue(info->getID());
        logDebug("show warning info on HMI [id:%d], [flash:%d]", info->getID(), info->getFlashFlag());
    }
}
