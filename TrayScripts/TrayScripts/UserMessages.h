#pragma once
#include "WinUser.h"

//between the ranges of 0x0400 and 0x7FFF  we have custom user messages to use in the Window Call backs
//But what isn't made clear is that actually 0x0400 - 0x407 are reserved for NIN_SELECT(0x0400) to NIN_POPUPCLOSE(0x407)
#define APPWM_MSGSTART 0x0408

//The Icon notify defined message
#define APPWM_ICONNOTIFY (WM_APP + 1)

//CONTEXT MENU RESERVED ID'S 
#define NID_ExitTrayCommand 0

