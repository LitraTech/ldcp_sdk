#ifndef LDCP_SDK_DEVICE_BASE_H_
#define LDCP_SDK_DEVICE_BASE_H_

#include "ldcp/location.h"
#include "ldcp/error.h"

#include <string>
#include <memory>

namespace ldcp_sdk
{

class Session;

class DeviceBase
{
public:
  DeviceBase(const Location& location);
  virtual ~DeviceBase();

  const Location& location() const;

  void setTimeout(int timeout_ms);

  virtual error_t open();
  bool isOpened() const;
  void close();

  error_t queryBootMode(std::string& mode);
  void reboot();

protected:
  std::unique_ptr<Location> location_;
  std::unique_ptr<Session> session_;
};

}

#endif
