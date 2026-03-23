/**
* @file         warning_info.hpp
* @brief        warning infomation
* @details      warning class about warning id, priority,falshflag and so on.
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

#include "warning_id.hpp"

/**
* @struct WarningPriority
* @brief  warning priority
* @details define warning priority,which has a priority to show.
           Smaller values are displayed first.
*/
enum WaringPriority{
    WP_SEVERITY = 0U,
    WP_NORMAL = 1U,
    WP_LOW = 2U
};

/**
* @struct WarningFlashFlag
* @brief  flash flag
* @details flash on or off
*/
enum WarningFlashFlag{
    FLASH_OFF    = 0U,
    FLASH_ON     = 1U     
};

enum WarningMode{
    NORMAL_MODE = 0U,
    ADAS_MODE   = 1U,
};

class WarningInfo{
    public:
        /**
        * @fn WarningInfo
        * @brief construct function
        * @param const WarningID& wID : warning id
        * @param const WaringPriority& wPriority: priority
        * @param const uint32_t& showTime : unit is millisecond
        * @param const uint8_t& flashFlag : WarningFlashFlag
        * @param const char* wName ： the warning name
        * @return priority value
        */
        WarningInfo(const WarningID& wID, 
                    const WaringPriority& wPriority, 
                    const uint32_t& showTime, 
                    const uint8_t& flashFlag,
                    const char* wName = NULL,
                    const uint8_t&   modeFlag = NORMAL_MODE);
        ~WarningInfo();

        friend bool operator<(WarningInfo a, WarningInfo b){
            return a.m_priority > b.m_priority;
        }
    public:
        /**
        * @fn getPriority
        * @brief get warning priority
        * @return priority value
        */
        WaringPriority getPriority() const;

        /**
        * @fn getID
        * @brief get warning ID
        * @return ID value
        */
        WarningID getID() const;

        /**
        * @fn getShowTime
        * @brief get warning showtime
        * @return showtime (ms)
        */
        uint32_t getShowTime() const;

        /**
        * @fn setShowTime
        * @brief set warning showtime
        * @param const uint32_t showTime : unit is millisecond.
        * @return 
        */
        void setShowTime(const uint32_t showTime);

        /**
        * @fn resetShowTime
        * @brief reset warning showtime
        * @return 
        */
        void resetShowTime();

        /**
        * @fn getFlashFlag
        * @brief get warning flash flag
        * @return flash flag : FLASH_OFF / FLASH_ON
        */
        uint8_t getFlashFlag() const;

        /**
        * @fn getName
        * @brief get warning name
        * @return name
        */
        const char* getName() const;

        /**
        * @fn printInfo
        * @brief printf warning info
        * @return name
        */
        void printInfo(const char* info);
        bool getmodeFlag() const;
    private:
        WarningInfo(const WarningInfo& warninginfo) = delete;
        WarningInfo& operator=(const WarningInfo& warninginfo) = delete;
    private:
        WaringPriority m_priority;
        WarningID m_id;

        uint32_t    m_defaultShowTime;
        uint32_t    m_showTime;
        uint8_t     m_flashFlag;
        const char* m_name;
        uint8_t     m_modeFlag;
};
