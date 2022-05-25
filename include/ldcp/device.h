#ifndef LDCP_SDK_DEVICE_H_
#define LDCP_SDK_DEVICE_H_

#include "ldcp/device_base.h"
#include "ldcp/device_info.h"
#include "ldcp/data_types.h"

namespace ldcp_sdk
{

class Device : public DeviceBase
{
public:
  Device(const Location& location);
  Device(const DeviceInfo& device_info);

  error_t open() override;

  error_t readSettings(const std::string& entry_name, void* value);
  error_t writeSettings(const std::string& entry_name, const void* value);
  error_t persistSettings(const std::string& entry_name);

  error_t startStreaming();
  error_t startStreaming(int frame_count);
  error_t stopStreaming();

  template<int Echos>
  error_t readScanFrame(ScanFrame<Echos>& scan_frame);

public:
  static const std::string SETTINGS_ENTRY_TRANSPORT_ETHERNET_DATA_CHANNEL_TARGET_PORT;
};

}

#endif
