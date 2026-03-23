#include "warning_info.hpp"
#include "log/log.hpp"


WarningInfo::WarningInfo(const WarningID& wID, 
                    const WaringPriority& wPriority, 
                    const uint32_t& showTime, 
                    const uint8_t& flashFlag,
                    const char* wName,
                    const uint8_t& modeFlag):
                    m_id(wID),
                    m_priority(wPriority),
                    m_defaultShowTime(showTime),
                    m_showTime(showTime),
                    m_flashFlag(flashFlag),
                    m_name(wName),
                    m_modeFlag(modeFlag)
                    
{

}
WarningInfo::~WarningInfo()
{

}

WarningID WarningInfo::getID() const
{
    return m_id;
}

WaringPriority WarningInfo::getPriority() const
{
    return m_priority;
}

uint32_t WarningInfo::getShowTime() const
{
    return m_showTime;
}

void WarningInfo::setShowTime(const uint32_t showTime)
{
    m_showTime = showTime;
}

void WarningInfo::resetShowTime()
{
    m_showTime = m_defaultShowTime;
}

uint8_t WarningInfo::getFlashFlag() const
{
    return m_flashFlag;
}

const char* WarningInfo::getName() const
{
    return m_name;
}
void WarningInfo::printInfo(const char* info)
{
    logDebug("[%s]: name[%s] id[%d] priority[%d] showtime[%d] flashflag[%d]", info, m_name, m_id, m_priority, m_showTime, m_flashFlag);
}


bool WarningInfo::getmodeFlag() const
{
    return m_modeFlag;
}

