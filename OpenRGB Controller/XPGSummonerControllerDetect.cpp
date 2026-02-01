/*---------------------------------------------------------*\
| XPGSummonerControllerDetect.cpp                           |
|                                                           |
|   Detector for XPG Summoner keyboard                      |
|                                                           |
|   Erick Granados (eriosgamer)                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "XPGSummonerController.h"
#include "RGBController_XPGSummoner.h"
#include "LogManager.h"
#include <hidapi.h>

/******************************************************************************************\
*                                                                                          *
*   DetectXPGSummonerControllers                                                           *
*                                                                                          *
*       Tests the USB address to see if a XPG Summoner Keyboard controller exists there.   *
*                                                                                          *
\******************************************************************************************/

void DetectXPGSummonerControllers(hid_device_info *info, const std::string &name)
{
    hid_device *dev = hid_open_path(info->path);

    if(dev)
    {
        LOG_INFO("[%s] RGB Interface opened successfully", name.c_str());
        LOG_INFO("[%s] Interface: %d, Usage Page: 0x%04X, Usage: 0x%04X", 
                  name.c_str(), info->interface_number, info->usage_page, info->usage);
        
        XPGSummonerController *controller = new XPGSummonerController(dev, info->path, info->product_id, name);
        RGBController_XPGSummoner *rgb_controller = new RGBController_XPGSummoner(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
    else
    {
        LOG_ERROR("[%s] Failed to open RGB interface", name.c_str());
    }
} /* DetectXPGSummonerControllers() */

/*-----------------------------------------------------*\
| Register only the RGB control interface (IF2)        |
| Usage Page: 0xFF01, Usage: 0x0001                    |
\*-----------------------------------------------------*/

REGISTER_HID_DETECTOR_IPU("XPG Summoner Gaming Keyboard", DetectXPGSummonerControllers, XPG_VID, XPG_SUMMONER_PID, 2, 0xFF01, 0x0001);