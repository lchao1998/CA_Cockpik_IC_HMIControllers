/**
* @file         warning_initialize.hpp
* @brief        warning initialize
* @details      warning map data
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
#include <string.h>
#include <cstdint>
#include <map>
#include "warning_info.hpp"

using namespace std;

class WarningInfoModel {
public:
    static WarningInfoModel& Instance() {
        static WarningInfoModel theSingleton;
        return theSingleton;
    }

    /**
    * @fn initModel
    * @brief initialize a warning model map.
    * @return void
    */
    static map<WarningID, WarningInfo*>& initModel();

    /**
    * @fn uninitModel
    * @brief uninitialize a warning model map.
    * @return void
    */
    void uninitModel();

    /**
    * @fn getModel
    * @brief return a warning model point.
    * @return model point
    */
    map<WarningID, WarningInfo*>& getModel();
private:
    WarningInfoModel()  = default;                                 
    WarningInfoModel(WarningInfoModel const&)  = delete;            
    WarningInfoModel& operator=(WarningInfoModel const&)  = delete; 
    ~WarningInfoModel();
    static bool m_bInitModel;
    static map<WarningID, WarningInfo*> m_model;
};
