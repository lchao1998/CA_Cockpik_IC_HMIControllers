/**
* @file         autosocm_define.hpp
* @brief        shared memory service access define
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
#include <cstdint>
#include "autosocm/application.hpp"
#include "sharedmemory.hpp"
#include "log/log.hpp"

using namespace std;

namespace AutoLogicController
{
#define CHECKCLIENTAVAILABILITY(name) \
        m_##name##Client->setAvailabilityChangedHandler([&](bool availability) \
        { \
            logDebug("%s Client %s-->%s", #name, m_##name##ClientAvailability ? "OK" : "NG", availability ? "OK" : "NG"); \
            m_##name##ClientAvailability = availability; \
        });

    // IPC Client name
    extern const char*  IPC_Client_Name_Common;
    extern const char*  IPC_Client_Name_Gauge;
    extern const char*  IPC_Client_Name_Telltale;
    extern const char*  IPC_Client_Name_Chime;
    extern const char*  IPC_Client_Name_TripComputer;
    extern const char*  IPC_Client_Name_Warning;
    extern const char*  IPC_Client_Name_Keyboard;
    extern const char*  IPC_Client_Name_ADAS;
    extern const char*  IPC_Client_Name_GVM;

    // IPC service ID
    extern const int32_t IPC_SERVICE_ID;

    // IPC instance ID
    extern const int32_t IPC_SERVICE_INSTANCE_COMMON;
    extern const int32_t IPC_SERVICE_INSTANCE_GAUGE;
    extern const int32_t IPC_SERVICE_INSTANCE_TELLTALE;
    extern const int32_t IPC_SERVICE_INSTANCE_CHIME;
    extern const int32_t IPC_SERVICE_INSTANCE_TRIPCOMPUTER;
    extern const int32_t IPC_SERVICE_INSTANCE_WARNING;
    extern const int32_t IPC_SERVICE_INSTANCE_KEYBOARD;
    extern const int32_t IPC_SERVICE_INSTANCE_TRIPSTATUS;
    extern const int32_t IPC_SERVICE_INSTANCE_ADAS;

    // sound service
    extern const int32_t IPC_SOUND_SERVICE_ID;
    extern const int32_t IPC_SERVICE_INSTANCE_SOUND;

    // gvm service
    extern const int32_t IPC_SERVICE_GVM_ID;
    extern const int32_t IPC_SERVICE_INSTANCE_GVM;


    template <typename type>
    bool checkValue(shared_ptr<AutoSOCM::Client> client,
                SharedMemory::AtomicNumber<type>& sm, type& value, const char* name)
    {
        bool ok = false;
        if (client == NULL)
        {
            logWarning("[%s] Client is Null", name);
        }
        else
        {
            const type tmp = static_cast<type>(client->getAttribute(name).toInt());
            if (tmp != value)
            {
                logInfo("[%s] Value Change %02u --> %02u", name, value, tmp);
                value = tmp;
                sm.setValue(value);

                ok = true;
            }
        }
        return ok;
    }
}
