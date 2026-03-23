/**
* @file         common_logic_controller.hpp
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
#include "appfw/timer.hpp"
#include "sm_struct_define.h"
#include "sharedmemory.hpp"
#include "module_define.h"

#include "sm_common.hpp"

class CommonLogicController : public AppFw::Module
{
public:
    CommonLogicController();
    virtual ~CommonLogicController();

protected:
    /**
    * @fn onProcess
    * @brief main thread loop
    * @return void
    */
    virtual void onProcess();
private:

private:
    SM_Common 						m_CommonInfo;
    shared_ptr<AutoSOCM::Client> 	m_CommonClient;
    bool 							m_CommonClientAvailability;
};

