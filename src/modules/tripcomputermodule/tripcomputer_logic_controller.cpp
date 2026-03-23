#include "tripcomputer_logic_controller.hpp"
#include <time.h>

TripComputerLogicController::TripComputerLogicController() : AppFw::Module(), m_TripComputerInfo(),
        m_TripComputerClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_TRIPCOMPUTER)),
        m_GaugeClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_GAUGE)),
        m_TripComputerClientAvailability(false),
        m_GaugeClientAvailability(false)
{
    memset(&m_TripComputerInfo, 0, sizeof(SM_TripComputer));

    CHECKCLIENTAVAILABILITY(TripComputer);
    CHECKCLIENTAVAILABILITY(Gauge);


    m_TripComputerClient->setPollable(true);
    m_GaugeClient->setPollable(true);

    gettimeofday(&m_start, NULL);
}

TripComputerLogicController::~TripComputerLogicController()
{
    // TODO Auto-generated destructor stub
}

void TripComputerLogicController::onProcess()
{
    static uint8_t accMode = 0U;

    m_TripComputerClient->poll();
    m_GaugeClient->poll();

    if (m_TripComputerClientAvailability)
    {
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TripComputerClient, tripcomputer_, m_TripComputerInfo, Odo);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TripComputerClient, tripcomputer_, m_TripComputerInfo, IP_FuelLevelPercent);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TripComputerClient, tripcomputer_, m_TripComputerInfo, AC_EnvironmentalTempVD);
        CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(m_TripComputerClient, tripcomputer_, m_TripComputerInfo, AC_EnvironmentalTemp);

        // calculate date.
        uint8_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        getLocalTime(year, month, day, hour, minute, second);
        tripcomputer_Vehicle_SYS_time_H.setValue(hour);
        tripcomputer_Vehicle_SYS_time_M.setValue(minute);
        tripcomputer_Vehicle_SYS_time_S.setValue(second);

        //logDebug("System time [%d:%d:%d]", hour, minute, second);
        // calculate drive odo
        uint32_t tmp_Odo = static_cast<uint32_t>(m_TripComputerClient->getAttribute("Odo").toInt());
        if(!m_bInitOdo)
        {
            if(tmp_Odo >0)
            {
                m_defaultDriveOdo = tmp_Odo;
                m_bInitOdo = true;
            }
        }
        else
        {    
            if(tmp_Odo > 0)
            {
                m_driveOdo = tmp_Odo - m_defaultDriveOdo;
                tripcomputer_trip.setValue(m_driveOdo);
                //logDebug("m_driveOdo : [%d]", m_driveOdo);
            }
        }
        
    }

    if(m_GaugeClientAvailability)
    {
        uint16_t tmp_vehicleSpeed = static_cast<uint16_t>(m_GaugeClient->getAttribute("ESP_VehicleSpeed").toUInt());
        m_averageSpeed = (m_averageSpeed + tmp_vehicleSpeed) / 2;
        tripcomputer_trip_Average_Speed.setValue(m_averageSpeed);

        //logDebug("m_averageSpeed : [%d]", tmp_vehicleSpeed);
    }

    setDriveTime();
}

void TripComputerLogicController::setDriveTime()
{
    struct timeval endTime;
    gettimeofday(&endTime, NULL);
    uint64_t driveTime = static_cast<uint64_t>(endTime.tv_sec - m_start.tv_sec);
    //logDebug("-------------drive time [%d : %d]-----------", driveTime / 3600, driveTime % 3600 / 60);
    tripcomputer_Vehicle_Running_time_H.setValue(static_cast<uint8_t>(driveTime / 3600));
    tripcomputer_Vehicle_Running_time_M.setValue(static_cast<uint8_t>(driveTime % 3600 / 60));
}

void TripComputerLogicController::getLocalTime(uint8_t& year, uint8_t& month, uint8_t& day, uint8_t& hour, uint8_t& minute, uint8_t& second)
{
    time_t now;
    struct tm *timenow;
    time(&now);
    timenow = localtime(&now);
    // logDebug("---------[Y: %d] [M:%d] [DD:%d] [H: %d] [m:%d] [s:%d] ----------", 
    // timenow->tm_year + 1900, timenow->tm_mon + 1, timenow->tm_mday,
    // timenow->tm_hour, timenow->tm_min, timenow->tm_sec);
    year = timenow->tm_year + 1900;
    month = timenow->tm_mon + 1;
    day = timenow->tm_mday;
    hour = timenow->tm_hour;
    minute = timenow->tm_min;
    second = timenow->tm_sec;
}


