#include "ldcp/device_base.h"
#include "ldcp/session.h"

namespace ldcp_sdk
{

DeviceBase::DeviceBase(const ldcp_sdk::DeviceInfo& device_info)
  : DeviceBase(device_info.location())
{
}

DeviceBase::DeviceBase(const Location& location)
  : session_(new Session())
{
  if (typeid(location) == typeid(NetworkLocation))
    location_ = std::unique_ptr<NetworkLocation>(new NetworkLocation((const NetworkLocation&)location));
}

DeviceBase::DeviceBase(DeviceBase&& other)
  : location_(std::move(other.location_))
  , session_(std::move(other.session_))
{
}

DeviceBase::~DeviceBase()
{
  if (session_ && session_->isOpened())
    session_->close();
}

const Location& DeviceBase::location() const
{
  return *location_;
}

void DeviceBase::setTimeout(int timeout)
{
  session_->setTimeout(timeout);
}

error_t DeviceBase::open()
{
  return session_->open(*location_);
}

void DeviceBase::close()
{
  session_->close();
}

}