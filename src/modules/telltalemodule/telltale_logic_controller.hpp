/**
* @file         tripcomputer_logic_controller.hpp
* @brief        get telltale from IPCL. And set it to HMI
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
#include "sm_struct_define.h"
#include "sharedmemory.hpp"
#include "module_define.h"

#include "sm_telltale.hpp"

using namespace std;

class TelltaleLogicController : public AppFw::Module
{
public:
    TelltaleLogicController();
    virtual ~TelltaleLogicController();

protected:

    /**
    * @fn onProcess
    * @brief main thread loop
    * @return void
    */
    virtual void onProcess();

private:
    SM_Telltale m_TelltaleInfo;


    shared_ptr<AutoSOCM::Client> m_TelltaleClient;
    shared_ptr<AutoSOCM::Client> m_GaugeClient;

    bool m_TelltaleClientAvailability;
    bool m_GaugeClientAvailability;
};
