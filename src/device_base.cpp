#include "ldcp/device_base.h"
#include "ldcp/session.h"

namespace ldcp_sdk
{

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

void DeviceBase::setTimeout(int timeout_ms)
{
  session_->setTimeout(timeout_ms);
}

error_t DeviceBase::open()
{
  return session_->open(*location_);
}

bool DeviceBase::isOpened() const
{
  return session_->isOpened();
}

void DeviceBase::close()
{
  session_->close();
}

error_t DeviceBase::queryBootMode(std::string& mode)
{
  rapidjson::Document request = session_->createEmptyRequestObject(), response;
  rapidjson::Document::AllocatorType& allocator = request.GetAllocator();
  request.AddMember("method", "device/queryInfo", allocator);
  request.AddMember("params",
                    rapidjson::Value().SetObject()
                      .AddMember("entry", "device.bootMode", allocator), allocator);

  error_t result = session_->executeCommand(std::move(request), response);

  if (result == error_t::no_error)
    mode = response["result"].GetString();

  return result;
}

void DeviceBase::reboot()
{
  rapidjson::Document request = session_->createEmptyRequestObject();
  request.AddMember("method", "device/reboot", request.GetAllocator());
  session_->executeCommand(std::move(request));
}

}
