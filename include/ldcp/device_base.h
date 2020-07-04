#ifndef LDCP_SDK_DEVICE_BASE_H_
#define LDCP_SDK_DEVICE_BASE_H_

#include "ldcp/common.h"
#include "ldcp/device_info.h"
#include "ldcp/error.h"
#include "ldcp/data_types.h"

namespace ldcp_sdk
{

class Session;

class LDCP_SDK_API DeviceBase
{
public:
  DeviceBase(const DeviceInfo& device_info);
  DeviceBase(const Location& location);
  DeviceBase(DeviceBase&& other);
  virtual ~DeviceBase();

  const Location& location() const;

  void setTimeout(int timeout);

  error_t open();
  void close();

  error_t queryOperationMode(std::string& mode);
  void reboot();

protected:
  std::unique_ptr<Location> location_;
  std::unique_ptr<Session> session_;
};

}

#endif
