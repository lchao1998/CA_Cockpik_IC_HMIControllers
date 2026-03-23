/**
* @file         adas_logic_controller.hpp
* @brief        
* @details      
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
#include "sm_struct_define.h"
#include "sharedmemory.hpp"
#include "module_define.h"


class AdasLogicController : public AppFw::Module
{
public:
    AdasLogicController();
    virtual ~AdasLogicController();

protected:
    /**
    * @fn onProcess
    * @brief main thread loop
    * @return void
    */
    virtual void onProcess();

private:
    SM_ADAS                         m_AdasInfo;
    shared_ptr<AutoSOCM::Client>    m_AdasClient;
    shared_ptr<AutoSOCM::Client>    m_TelltaleClient;
    shared_ptr<AutoSOCM::Client>    m_GaugeClient;

    bool m_GaugeClientAvailability;
    bool m_TelltaleClientAvailability;
    bool m_AdasClientAvailability;
};
