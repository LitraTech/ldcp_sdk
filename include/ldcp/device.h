#ifndef LDCP_SDK_DEVICE_H_
#define LDCP_SDK_DEVICE_H_

#include "ldcp/device_base.h"

namespace ldcp_sdk
{

class Session;

class Device : public DeviceBase
{
public:
  Device(const DeviceInfo& device_info);
  Device(const Location& location);
  Device(DeviceBase&& other);

  error_t getIdentityInfo(std::map<std::string, std::string>& identity_info);
  error_t getVersionInfo(std::map<std::string, std::string>& version_info);
  error_t getStatusInfo(std::map<std::string, std::string>& status_info);
  error_t enterLowPower();
  error_t exitLowPower();
  error_t readTimestamp(uint32_t& timestamp);
  error_t resetTimestamp();
  void reboot();
  void rebootToBootloader();

  error_t startStreaming();
  error_t readScanBlock(ScanBlock& scan_block);
  error_t stopStreaming();
};

}

#endif
