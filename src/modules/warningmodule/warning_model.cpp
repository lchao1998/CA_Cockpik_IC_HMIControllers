#include "warning_model.hpp"
#include "warning_id.hpp"


#define INIT_WARNING(id, priority, showTime, flashflag, modeflag) tmp_model[id]=(new WarningInfo(id, priority, showTime, flashflag, #id, modeflag))

bool WarningInfoModel::m_bInitModel = false;
map<WarningID, WarningInfo*> WarningInfoModel::m_model = initModel();

map<WarningID, WarningInfo*>& WarningInfoModel::initModel()
{
    if(m_bInitModel)
    {
        return m_model;
    }
    m_bInitModel = true;

    static map<WarningID, WarningInfo*> tmp_model;

    INIT_WARNING(WID_SRS_DriverBuckleSwitchStatus, WP_SEVERITY, 2000, FLASH_OFF, NORMAL_MODE); 
    INIT_WARNING(WID_SRS_PassengerBuckleSwitchStatus, WP_SEVERITY, 2000, FLASH_OFF, NORMAL_MODE); 
    INIT_WARNING(WID_EMS_MIL, WP_SEVERITY, 2000, FLASH_OFF, false); 
    INIT_WARNING(WID_SRS_WarningLampStatus, WP_SEVERITY, 2000, FLASH_OFF, NORMAL_MODE); 

    INIT_WARNING(WID_ACC_ACCTargetLngRange_30, WP_SEVERITY, 2000, FLASH_OFF, ADAS_MODE); 
    INIT_WARNING(WID_ACC_ACCTargetLngRange_80, WP_SEVERITY, 2000, FLASH_OFF, ADAS_MODE); 
    INIT_WARNING(WID_ACC_ACCTargetLngRange_180, WP_SEVERITY, 2000, FLASH_OFF, ADAS_MODE); 

    return tmp_model;
}

void WarningInfoModel::uninitModel()
{
    for(auto itor = m_model.begin(); itor != m_model.end();)
    {
        if(itor->second != NULL)
        {
            delete itor->second;
            itor->second = NULL; 
        }
        m_model.erase(itor++);
    }
    m_bInitModel = false;
}
map<WarningID, WarningInfo*>& WarningInfoModel::getModel()
{
    return m_model;
}

WarningInfoModel::~WarningInfoModel()
{
    uninitModel();
}