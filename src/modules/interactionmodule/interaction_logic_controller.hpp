/**
* @file         interaction_logic_controller.hpp
* @brief        menu controller , keyboard status and so on 
* @details      about interactions.
* @author       xu.qiang@zlingsmart.com
* @date         2022-3-1
* @version      v0.0.1
* @copyright    Copyright (c) 2022, zlingsmart Corporation.
**********************************************************************************
* @attention 
* 
* @par modity log:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2022/03/01  <td>v0.0.1   <td>          <td>Create
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

#include "sm_interaction_status.hpp"


using namespace std;
using namespace AppFw;

class InteractionLogicController:public Module
{
public:
    InteractionLogicController();
    ~InteractionLogicController();

protected:
    /**
    * @fn onProcess
    * @brief main thread loop
    * @return void
    */
    virtual void onProcess();

private:
    /**
    * @fn processMenukeyPress
    * @brief process keyboard info and set menu index.
    * @return void
    */
    void processMenukeyPress();
private:
    SM_KeyBoard m_InteractionInfo;
    SM_ADAS m_AdasInfo;

    shared_ptr<AutoSOCM::Client> m_InteractionClient;
    shared_ptr<AutoSOCM::Client> m_AdasClient;
    shared_ptr<AutoSOCM::Client> m_GvmClient;

    bool    m_InteractionClientAvailability;
    bool    m_AdasClientAvailability;
    bool    m_GvmClientAvailability;

    uint8_t m_MenuIndex = 1;
};