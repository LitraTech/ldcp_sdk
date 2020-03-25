#include "ldcp/device.h"
#include "ldcp/session.h"
#include "ldcp/utility.h"

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

error_t Device::getIdentityInfo(std::map<std::string, std::string>& identity_info)
{
  rapidjson::Document request = session_->createEmptyRequestObject(), response;
  rapidjson::Document::AllocatorType& allocator = request.GetAllocator();
  request["method"].SetString("device/queryInfo");
  request.AddMember("params",
                    rapidjson::Value().SetObject()
                      .AddMember("category", "identity", allocator), allocator);

  error_t result = session_->executeCommand(std::move(request), response);

  if (result == error_t::no_error) {
    identity_info.clear();
    for (const auto& member : response["result"].GetObject())
      identity_info[member.name.GetString()] = member.value.GetString();
  }

  return result;
}

error_t Device::getVersionInfo(std::map<std::string, std::string>& version_info)
{
  rapidjson::Document request = session_->createEmptyRequestObject(), response;
  rapidjson::Document::AllocatorType& allocator = request.GetAllocator();
  request["method"].SetString("device/queryInfo");
  request.AddMember("params",
                    rapidjson::Value().SetObject()
                      .AddMember("category", "version", allocator), allocator);

  error_t result = session_->executeCommand(std::move(request), response);

  if (result == error_t::no_error) {
    version_info.clear();
    for (const auto& member : response["result"].GetObject())
      version_info[member.name.GetString()] = member.value.GetString();
  }

  return result;
}

error_t Device::getStatusInfo(std::map<std::string, std::string>& status_info)
{
  rapidjson::Document request = session_->createEmptyRequestObject(), response;
  rapidjson::Document::AllocatorType& allocator = request.GetAllocator();
  request["method"].SetString("device/queryInfo");
  request.AddMember("params",
                    rapidjson::Value().SetObject()
                      .AddMember("category", "status", allocator), allocator);

  error_t result = session_->executeCommand(std::move(request), response);

  if (result == error_t::no_error) {
    status_info.clear();
    for (const auto& member : response["result"].GetObject())
      status_info[member.name.GetString()] = member.value.GetString();
  }

  return result;
}

error_t Device::enterLowPower()
{
  rapidjson::Document request = session_->createEmptyRequestObject(), response;
  request["method"].SetString("device/enterLowPower");
  error_t result = session_->executeCommand(std::move(request), response);
  return result;
}

error_t Device::exitLowPower()
{
  rapidjson::Document request = session_->createEmptyRequestObject(), response;
  request["method"].SetString("device/exitLowPower");
  error_t result = session_->executeCommand(std::move(request), response);
  return result;
}

error_t Device::readTimestamp(uint32_t& timestamp)
{
  rapidjson::Document request = session_->createEmptyRequestObject(), response;
  request["method"].SetString("device/readTimestamp");
  error_t result = session_->executeCommand(std::move(request), response);
  if (result == error_t::no_error)
    timestamp = (uint32_t)response["result"].GetInt64();
  return result;
}

error_t Device::resetTimestamp()
{
  rapidjson::Document request = session_->createEmptyRequestObject(), response;
  request["method"].SetString("device/resetTimestamp");
  error_t result = session_->executeCommand(std::move(request), response);
  return result;
}

void Device::reboot()
{
  rapidjson::Document request = session_->createEmptyRequestObject();
  request["method"].SetString("device/reboot");
  session_->executeCommand(std::move(request));
}

void Device::rebootToBootloader()
{
  rapidjson::Document request = session_->createEmptyRequestObject();
  request["method"].SetString("device/rebootToBootloader");
  session_->executeCommand(std::move(request));
}

error_t Device::startStreaming()
{
  rapidjson::Document request = session_->createEmptyRequestObject(), response;
  request["method"].SetString("scan/startStreaming");
  error_t result = session_->executeCommand(std::move(request), response);
  return result;
}

error_t Device::readScanBlock(ScanBlock& scan_block)
{
  rapidjson::Document notification;
  error_t result = session_->pollForScanData(notification);

  if (result == error_t::no_error) {
    scan_block.block_id = notification["params"]["block"].GetInt();
    scan_block.timestamp = (uint32_t)notification["params"]["timestamp"].GetInt64();
    scan_block.layers.resize(notification["params"]["layers"].Size());
    for (size_t i = 0; i < scan_block.layers.size(); i++) {
      const rapidjson::Value& layer = notification["params"]["layers"][i];
      if (layer.IsNull())
        continue;

      const rapidjson::Value& ranges = layer["ranges"];
      if (!ranges.IsNull()) {
        int decoded_length = Utility::CalculateBase64DecodedLength(ranges.GetString(), ranges.GetStringLength());
        scan_block.layers[i].ranges.resize(decoded_length / sizeof(uint16_t));
        Utility::Base64Decode(ranges.GetString(), ranges.GetStringLength(), (uint8_t*)&scan_block.layers[i].ranges[0]);
      }
      const rapidjson::Value& intensities = layer["intensities"];
      if (!intensities.IsNull()) {
        int decoded_length = Utility::CalculateBase64DecodedLength(intensities.GetString(), intensities.GetStringLength());
        scan_block.layers[i].intensities.resize(decoded_length);
        Utility::Base64Decode(intensities.GetString(), intensities.GetStringLength(), &scan_block.layers[i].intensities[0]);
      }
    }
  }

  return result;
}

error_t Device::stopStreaming()
{
  rapidjson::Document request = session_->createEmptyRequestObject(), response;
  request["method"].SetString("scan/stopStreaming");
  error_t result = session_->executeCommand(std::move(request), response);
  return result;
}

}
