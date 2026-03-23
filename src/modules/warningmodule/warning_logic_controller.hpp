/**
* @file         warning_logic_controller.hpp
* @brief        get warning computer from IPCL. And set it to HMI
* @details      
* @author       xu.qiang@zlingsmart.com
* @date         2022-3-2
* @version      v0.0.1
* @copyright    Copyright (c) 2022, zlingsmart Corporation.
**********************************************************************************
* @attention 
* 
* @par modity log:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2022/03/02  <td>v0.0.1   <td>          <td>Create
* </table>
*
**********************************************************************************
*/
#pragma once

#include "appfw/module.hpp"
#include "appfw/timer.hpp"
#include "sm_struct_define.h"
#include "sharedmemory.hpp"
#include "module_define.h"
#include "warning_info.hpp"
#include <vector>
#include <map>
#include <mutex>

using namespace std;

class WarningLogicController : public AppFw::Module
{
public:
    WarningLogicController(void);
    virtual ~WarningLogicController(void);

public:
    /**
    * @fn initWarningData
    * @brief initialize warning map data.
    * @return void
    */
    void initWarningData();
    /**
    * @fn startWarningTimer
    * @brief start warning timer.
    * @return void
    */
    void startWarningTimer();

    /**
    * @fn stopWarningTimer
    * @brief stop warning timer.
    * @return void
    */
    void stopWarningTimer();

protected:
    /**
    * @fn onProcess
    * @brief main thread loop
    * @return void
    */
    virtual void onProcess(void);
private:
    /**
    * @fn onWarningTimer
    * @brief process warning timer callback
    * @return void
    */
    void onWarningTimer();

    /**
    * @fn checkWarnings
    * @brief check all the warning info.
    * @return void
    */
    void checkWarnings();

    /**
    * @fn checkAdasWarnings
    * @brief check  adas warning info.
    * @return void
    */
    void checkAdasWarnings();

    /**
    * @fn checkWarning
    * @brief check one warning field.
    * @param uint8_t& value : warning field value which want to be check if changed.
    * @param WarningInfo* info : warninginfo object point.
    * @param const char* name : warning field name
    * @return void
    */
    void checkWarning(uint8_t& value, WarningInfo* info, const char* name);

    /**
    * @fn pushWarningInfo
    * @brief push warninginfo into queue.
    * @param WarningInfo* const info: warning info object const point
    * @return void
    */
    void pushWarningInfo(WarningInfo* const info);

    /**
    * @fn popWarningInfo
    * @brief pop warninginfo into queue.
    * @param WarningInfo* const info: warning info object const point
    * @return void
    */
    void popWarningInfo(WarningInfo* const info);

    /**
    * @fn showWarningInfo
    * @brief show warninginfo from queue.
    * @return void
    */
    void showWarningInfo();

    /**
    * @fn hideWarninginfo
    * @brief hide warninginfo from queue.
    * @return void
    */
    void hideWarninginfo();

    /**
    * @fn showWarningOnHMI
    * @brief show the warning info on HMI.
    * @param WarningInfo* const info: warning info object const point
    * @return void
    */
    void showWarningOnHMI(WarningInfo* const info);

private:

    SM_Warning m_WarningInfo;
    AppFw::Timer m_warningTimer;
    shared_ptr<AutoSOCM::Client> m_WarningClient;
    shared_ptr<AutoSOCM::Client> m_AdasClient;
    shared_ptr<AutoSOCM::Client> m_GaugeClient;

    bool m_GaugeClientAvailability;
    bool m_WarningClientAvailability;
    bool m_AdasClientAvailability;
    bool m_bWarningTimerOn;
private:
    vector<WarningInfo*> m_warningQueue;
    map<WarningID, WarningInfo*> m_warningMap;
    WarningInfo* m_currentWarningInfo;
    std::mutex m_queueMutex;
};

