/**
* @file         tripcomputer_logic_controller.hpp
* @brief
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
#include <sys/time.h>

#include "sm_tripcomputer.hpp"


class TripComputerLogicController : public AppFw::Module {
public:
    TripComputerLogicController();
    virtual ~TripComputerLogicController();

protected:
    /**
    * @fn onProcess
    * @brief main thread loop
    * @return void
    */
    virtual void onProcess();
private:
    /**
    * @fn getLocalTime
    * @brief get local time from system
    * @param uint8_t& year      : output the year
    * @param uint8_t& month     : output the month
    * @param uint8_t& day       : output the day
    * @param uint8_t& hour      : output the hour
    * @param uint8_t& minute    : output the minute
    * @param uint8_t& second    : output the second
    * @return void
    */
    void getLocalTime(uint8_t &year, uint8_t &month, uint8_t &day, uint8_t &hour, uint8_t &minute, uint8_t &second);

    /**
    * @fn setDriveTime
    * @brief set local drive time after power on
    * @return void
    */
    void setDriveTime();
private:
    SM_TripComputer m_TripComputerInfo;
    shared_ptr<AutoSOCM::Client> m_TripComputerClient;
    shared_ptr<AutoSOCM::Client> m_GaugeClient;

    bool m_GaugeClientAvailability;
    bool m_TripComputerClientAvailability;

    uint16_t m_averageSpeed = 0;
    uint16_t m_driveOdo = 0;
    uint32_t m_defaultDriveOdo = 0;
    bool m_bInitOdo = false;

    struct timeval m_start;

};
