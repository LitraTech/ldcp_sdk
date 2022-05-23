#include "ldcp/device.h"
#include "ldcp/session.h"
#include "ldcp/utility.h"

#include <asio.hpp>

namespace ldcp_sdk
{

Device::Device(const DeviceInfo& device_info)
  : DeviceBase(device_info)
{
}

Device::Device(const Location& location)
  : DeviceBase(location)
{
}

Device::Device(DeviceBase&& other)
  : DeviceBase(std::move(other))
{
}

error_t Device::open()
{
  error_t result = DeviceBase::open();
  return result;
}

}
