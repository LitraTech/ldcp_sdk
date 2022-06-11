#include "ldcp/device.h"
#include "ldcp/session.h"
#include "ldcp/utility.h"

#include <asio.hpp>

namespace ldcp_sdk
{

Device::Device(const Location& location)
  : DeviceBase(location)
{
}

Device::Device(const DeviceInfo& device_info)
  : Device(device_info.location())
{
}

error_t Device::open()
{
  error_t result = DeviceBase::open();
  if (result == error_t::no_error) {
    in_addr_t target_address = INADDR_NONE;
    in_port_t target_port = 0;
    if ((result = readSettings(
           SETTINGS_ENTRY_TRANSPORT_ETHERNET_DATA_CHANNEL_TARGET_ADDRESS,
           &target_address)) == error_t::no_error &&
        (result = readSettings(
           SETTINGS_ENTRY_TRANSPORT_ETHERNET_DATA_CHANNEL_TARGET_PORT,
           &target_port)) == error_t::no_error)
      result = session_->openDataChannel(target_port);
    if (result != error_t::no_error)
      close();
  }
  return result;
}

error_t Device::readSettings(const std::string& entry_name, void* value)
{
  rapidjson::Document request = session_->createEmptyRequestObject();
  rapidjson::Document::AllocatorType& allocator = request.GetAllocator();
  request.AddMember("method", "settings/read", allocator);
  request.AddMember("params", rapidjson::Value().SetObject(), allocator);
  request["params"].AddMember("entry",
    rapidjson::Value().SetString(entry_name.c_str(), allocator), allocator);

  rapidjson::Document response;
  error_t result = session_->executeCommand(std::move(request), response);

  if (result == error_t::no_error) {
    if (entry_name == SETTINGS_ENTRY_TRANSPORT_ETHERNET_DATA_CHANNEL_TARGET_ADDRESS)
      *(in_addr_t*)value = inet_addr(response["result"].GetString());
    else if (entry_name == SETTINGS_ENTRY_TRANSPORT_ETHERNET_DATA_CHANNEL_TARGET_PORT)
      *(in_port_t*)value = htons(response["result"].GetInt());
    else
      return error_t::not_supported;
  }
  return result;
}

error_t Device::writeSettings(const std::string& entry_name, const void* value)
{
  rapidjson::Document request = session_->createEmptyRequestObject();
  rapidjson::Document::AllocatorType& allocator = request.GetAllocator();
  request.AddMember("method", "settings/write", allocator);
  request.AddMember("params", rapidjson::Value().SetObject(), allocator);
  request["params"].AddMember("entry",
    rapidjson::Value().SetString(entry_name.c_str(), allocator), allocator);
  if (entry_name == SETTINGS_ENTRY_TRANSPORT_ETHERNET_DATA_CHANNEL_TARGET_ADDRESS) {
    request["params"].AddMember(
        "value",
        rapidjson::Value().SetString(((const std::string*)value)->c_str(), allocator),
        allocator
    );
  }
  else if (entry_name == SETTINGS_ENTRY_TRANSPORT_ETHERNET_DATA_CHANNEL_TARGET_PORT)
    request["params"].AddMember("value", ntohs(*(in_port_t*)value), allocator);
  else
    return error_t::not_supported;

  rapidjson::Document response;
  error_t result = session_->executeCommand(std::move(request), response);

  return result;
}

error_t Device::persistSettings(const std::string& entry_name)
{
  rapidjson::Document request = session_->createEmptyRequestObject();
  rapidjson::Document::AllocatorType& allocator = request.GetAllocator();
  request.AddMember("method", "settings/persist", allocator);
  request.AddMember("params", rapidjson::Value().SetObject(), allocator);
  request["params"].AddMember("entry",
    rapidjson::Value().SetString(entry_name.c_str(), allocator), allocator);

  rapidjson::Document response;
  error_t result = session_->executeCommand(std::move(request), response);

  return result;
}

error_t Device::startStreaming()
{
  rapidjson::Document request = session_->createEmptyRequestObject(), response;
  request.AddMember("method", "scan/startStreaming", request.GetAllocator());
  error_t result = session_->executeCommand(std::move(request), response);
  return result;
}

error_t Device::startStreaming(int frame_count)
{
  rapidjson::Document request = session_->createEmptyRequestObject(), response;
  rapidjson::Document::AllocatorType& allocator = request.GetAllocator();
  request.AddMember("method", "scan/startStreaming", allocator);
  request.AddMember("params", rapidjson::Value().SetObject(), allocator);
  request["params"].AddMember("count", frame_count, allocator);
  error_t result = session_->executeCommand(std::move(request), response);
  return result;
}

error_t Device::stopStreaming()
{
  rapidjson::Document request = session_->createEmptyRequestObject(), response;
  request.AddMember("method", "scan/stopStreaming", request.GetAllocator());
  error_t result = session_->executeCommand(std::move(request), response);
  return result;
}

template<int Echos>
error_t Device::readScanFrame(ScanFrame<Echos>& scan_frame)
{
  std::vector<uint8_t> scan_packet;

  int block_index = 0, block_count = 1;
  while (block_index < block_count) {
    error_t result = session_->receiveScanPacket(scan_packet);
    if (result != error_t::no_error)
      return result;

    const ScanPacketHeader* packet_header =
      reinterpret_cast<const ScanPacketHeader*>(scan_packet.data());
    if (packet_header->block_index != block_index) {
      block_index = 0;
      continue;
    }
    else {
      int block_length = packet_header->block_length;

      if (block_index == 0) {
        block_count = packet_header->block_count;
        scan_frame.timestamp = packet_header->timestamp;
        scan_frame.layers.resize(1);
        scan_frame.layers[0].ranges.resize(block_count * block_length);
        scan_frame.layers[0].intensities.resize(block_count * block_length);
      }

      int echo_count = packet_header->flags.payload_layout.echo_count + 1;
      const uint16_t* range_pointer = reinterpret_cast<const uint16_t*>(
        scan_packet.data() + sizeof(ScanPacketHeader));
      const uint8_t* intensity_pointer = reinterpret_cast<const uint8_t*>(
        range_pointer + block_length * echo_count);
      for (int i = 0; i < block_length; i++) {
        int measurement_index = block_index * block_length + i;
        for (int j = 0; j < echo_count; j++) {
          if (j < Echos) {
            scan_frame.layers[0].ranges[measurement_index][j] = *range_pointer;
            scan_frame.layers[0].intensities[measurement_index][j] = *intensity_pointer;
          }
          range_pointer++;
          intensity_pointer++;
        }
        std::fill(scan_frame.layers[0].ranges[measurement_index].begin() + echo_count,
          scan_frame.layers[0].ranges[measurement_index].end(), 0);
        std::fill(scan_frame.layers[0].intensities[measurement_index].begin() + echo_count,
          scan_frame.layers[0].intensities[measurement_index].end(), 0);
      }

      block_index++;
    }
  }

  return error_t::no_error;
}

template error_t Device::readScanFrame<1>(ScanFrame<1>& scan_frame);
template error_t Device::readScanFrame<2>(ScanFrame<2>& scan_frame);

const std::string Device::SETTINGS_ENTRY_TRANSPORT_ETHERNET_DATA_CHANNEL_TARGET_ADDRESS =
    "transport.ethernet.dataChannel.targetAddress";
const std::string Device::SETTINGS_ENTRY_TRANSPORT_ETHERNET_DATA_CHANNEL_TARGET_PORT =
    "transport.ethernet.dataChannel.targetPort";

}
