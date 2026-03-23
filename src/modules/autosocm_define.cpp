#include "autosocm_define.hpp"

namespace AutoLogicController
{
    const char*  IPC_Client_Name_Common       = "CommonClient";
    const char*  IPC_Client_Name_Gauge        = "GaugeClient";
    const char*  IPC_Client_Name_Telltale     = "TelltaleClient";
    const char*  IPC_Client_Name_Chime        = "ChimeClient";
    const char*  IPC_Client_Name_TripComputer = "TripComputerClient";
    const char*  IPC_Client_Name_Warning      = "WarningClient";
    const char*  IPC_Client_Name_Keyboard     = "KeyboardClient";
    const char*  IPC_Client_Name_ADAS         = "ADASClient";
    const char*  IPC_Client_Name_GVM          = "GVMClient";

    const int32_t IPC_SERVICE_ID                    = 0x2022;

    

    const int32_t IPC_SERVICE_INSTANCE_COMMON       = 0x0001;
    const int32_t IPC_SERVICE_INSTANCE_GAUGE        = 0x0002;
    const int32_t IPC_SERVICE_INSTANCE_TELLTALE     = 0x0003;
    const int32_t IPC_SERVICE_INSTANCE_CHIME        = 0x0004;
    const int32_t IPC_SERVICE_INSTANCE_TRIPCOMPUTER = 0x0005;
    const int32_t IPC_SERVICE_INSTANCE_WARNING      = 0x0006;
    const int32_t IPC_SERVICE_INSTANCE_KEYBOARD     = 0x0007;
    const int32_t IPC_SERVICE_INSTANCE_TRIPSTATUS   = 0x0008;
    const int32_t IPC_SERVICE_INSTANCE_ADAS         = 0x000A;

    // sound service
    const int32_t IPC_SOUND_SERVICE_ID              = 0x1005;
    const int32_t IPC_SERVICE_INSTANCE_SOUND        = 0x0001;

    // gvm service
    const int32_t IPC_SERVICE_GVM_ID                = 0x2023;
    const int32_t IPC_SERVICE_INSTANCE_GVM          = 0x000B;



} 
