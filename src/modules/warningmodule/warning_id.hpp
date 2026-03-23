/**
* @file         warning_id.hpp
* @brief        warning id
* @details      
* @author       xu.qiang@zlingsmart.com
* @date         2022-3-16
* @version      v0.0.1
* @copyright    Copyright (c) 2022, zlingsmart Corporation.
**********************************************************************************
* @attention 
* 
* @par modity log:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2022/03/16  <td>v0.0.1   <td>          <td>Create
* </table>
*
**********************************************************************************
*/
#pragma once

enum WarningID{
    WID_Unknown = 0U,
    WID_SRS_DriverBuckleSwitchStatus = 1U,                          // The main driver seat belt is not fastened
    WID_SRS_PassengerBuckleSwitchStatus = 2U,                       // The co-driver's seat belt is not fastened
    WID_EMS_MIL = 3U,                                               // Engine failure, please repair in time
    WID_SRS_WarningLampStatus = 4U,                                 // Airbag malfunction, please repair in time
    WID_ACC_ACCTargetLngRange_30 = 5U,                              // The obstacle is about 30 meters away 30~40
    WID_ACC_ACCTargetLngRange_80 = 6U,                              // The obstacle is about 80 meters away 80~90
    WID_ACC_ACCTargetLngRange_180 = 7U                              // The obstacle is about 180 meters away 170~180
};



