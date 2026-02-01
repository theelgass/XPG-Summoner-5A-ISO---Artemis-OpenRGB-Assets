/*---------------------------------------------------------*\
| XPGSummonerController.cpp                                 |
|                                                           |
|   Driver for XPG Summoner keyboard - Alternative version |
|                                                           |
|   Erick Granados (eriosgamer)                             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "XPGSummonerController.h"
#include "StringUtils.h"
#include "LogManager.h"

XPGSummonerController::XPGSummonerController(hid_device *dev_handle, const char *path, const unsigned short pid, std::string dev_name)
{
    dev      = dev_handle;
    location = path;
    name     = dev_name;
    usb_pid  = pid;

    LOG_DEBUG("[%s] Controller initialized", name.c_str());
    SendInitialize();
}

XPGSummonerController::~XPGSummonerController()
{
    hid_close(dev);
}

void XPGSummonerController::SendInitialize()
{
    // Method 1: With Report ID
    unsigned char init_buf1[265];
    memset(init_buf1, 0x00, 265);
    init_buf1[0] = 0x07;
    init_buf1[1] = 0xEA;
    init_buf1[2] = 0x00;
    init_buf1[3] = 0x00;
    
    int result1 = hid_write(dev, init_buf1, 265);
    LOG_DEBUG("[%s] SendInitialize method 1 (with ReportID): %d", name.c_str(), result1);
    
    if(result1 < 0)
    {
        // Method 2: Without Report ID (264 bytes)
        unsigned char init_buf2[264];
        memset(init_buf2, 0x00, 264);
        init_buf2[0] = 0xEA;
        init_buf2[1] = 0x00;
        init_buf2[2] = 0x00;
        
        int result2 = hid_write(dev, init_buf2, 264);
        LOG_DEBUG("[%s] SendInitialize method 2 (no ReportID): %d", name.c_str(), result2);
        
        if(result2 < 0)
        {
            LOG_ERROR("[%s] Both init methods failed: %ls", name.c_str(), hid_error(dev));
        }
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

std::string XPGSummonerController::GetLocationString()
{
    return("HID: " + location);
}

std::string XPGSummonerController::GetNameString()
{
    return(name);
}

std::string XPGSummonerController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

unsigned short XPGSummonerController::GetUSBPID()
{
    return(usb_pid);
}

void XPGSummonerController::SendColors(unsigned char *color_data, unsigned int color_data_size)
{
    LOG_TRACE("[%s] SendColors called with %d bytes", name.c_str(), color_data_size);
    
    const int total_bytes = 126 * 4;
    const int packet_size = 256;
    
    SendColorDataPacket(0, color_data, packet_size);
    
    int remaining = total_bytes - packet_size;
    if(remaining > 0)
    {
        SendColorDataPacket(1, &color_data[packet_size], remaining);
    }
}

unsigned int XPGSummonerController::SendColorDataPacket(
    unsigned char packet_id,
    unsigned char *color_data,
    unsigned int color_size)
{
    unsigned int copy_size = (color_size > 256) ? 256 : color_size;
    int result = -1;
    
    // Method 1: Standard packet with Report ID (265 bytes)
    {
        unsigned char packet[265];
        memset(packet, 0x00, 265);
        
        packet[0] = 0x07;
        packet[1] = 0xA3;
        packet[2] = 0x08;
        packet[3] = 0x00;
        packet[4] = packet_id;
        packet[5] = 0x00;
        
        memcpy(&packet[6], color_data, copy_size);
        
        result = hid_write(dev, packet, 265);
        LOG_TRACE("[%s] Zone %d method 1 (265 bytes): result=%d", 
                  name.c_str(), packet_id, result);
    }
    
    // Method 2: If method 1 failed, try without Report ID (264 bytes)
    if(result < 0)
    {
        unsigned char packet[264];
        memset(packet, 0x00, 264);
        
        packet[0] = 0xA3;
        packet[1] = 0x08;
        packet[2] = 0x00;
        packet[3] = packet_id;
        packet[4] = 0x00;
        
        memcpy(&packet[5], color_data, copy_size);
        
        result = hid_write(dev, packet, 264);
        LOG_TRACE("[%s] Zone %d method 2 (264 bytes): result=%d", 
                  name.c_str(), packet_id, result);
    }
    
    // Method 3: Try as output report
    if(result < 0)
    {
        unsigned char packet[265];
        memset(packet, 0x00, 265);
        
        packet[0] = 0x07;
        packet[1] = 0xA3;
        packet[2] = 0x08;
        packet[3] = 0x00;
        packet[4] = packet_id;
        packet[5] = 0x00;
        
        memcpy(&packet[6], color_data, copy_size);
        
        result = hid_send_feature_report(dev, packet, 265);
        LOG_TRACE("[%s] Zone %d method 3 (feature report): result=%d", 
                  name.c_str(), packet_id, result);
    }
    
    if(result < 0)
    {
        LOG_ERROR("[%s] Zone %d: All methods failed: %ls", 
                  name.c_str(), packet_id, hid_error(dev));
    }
    else
    {
        LOG_DEBUG("[%s] Zone %d: Success! Sent %d bytes", 
                  name.c_str(), packet_id, result);
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    return copy_size;
}

void XPGSummonerController::SendTerminateColorPacket()
{
    unsigned char terminate_buf[265];
    memset(terminate_buf, 0x00, 265);
    
    terminate_buf[0] = 0x07;
    terminate_buf[1] = 0xEA;
    terminate_buf[2] = 0x00;
    terminate_buf[3] = 0x00;
    
    int result = hid_write(dev, terminate_buf, 265);
    LOG_DEBUG("[%s] SendTerminate: result = %d", name.c_str(), result);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}