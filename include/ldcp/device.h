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
  class Properties;
  class Settings;

public:
  Device(const Location& location);
  Device(const DeviceInfo& device_info);

  error_t open() override;

  error_t startMeasurement();
  error_t stopMeasurement();

  error_t startStreaming();
  error_t startStreaming(int frame_count);
  error_t stopStreaming();

  template<int Echos>
  error_t readScanFrame(ScanFrame<Echos>& scan_frame);

  Properties& properties();
  Settings& settings();

private:
  std::unique_ptr<Properties> properties_;
  std::unique_ptr<Settings> settings_;
};

class Device::Properties
{
  friend Device;

public:
  static const std::string IDENTITY_MODEL_NAME;
  static const std::string IDENTITY_SERIAL_NUMBER;
  static const std::string VERSION_FIRMWARE;

public:
  error_t get(const std::string& entry_name, void* value);

private:
  Properties(Session& session);

private:
  Session& session_;
};

class Device::Settings
{
  friend Device;

public:
  static const std::string ENTRY_RANGEFINDER_ECHO_MODE;
  static const std::string ENTRY_SCAN_RESOLUTION;
  static const std::string ENTRY_SCAN_FREQUENCY;
  static const std::string ENTRY_FILTERS_SHADOW_FILTER_ENABLED;
  static const std::string ENTRY_FILTERS_SHADOW_FILTER_STRENGTH;
  static const std::string ENTRY_CONNECTIVITY_ETHERNET_IPV4_ADDRESS;
  static const std::string ENTRY_CONNECTIVITY_ETHERNET_IPV4_SUBNET;
  static const std::string ENTRY_TRANSPORT_ETHERNET_DATA_CHANNEL_TARGET_ADDRESS;
  static const std::string ENTRY_TRANSPORT_ETHERNET_DATA_CHANNEL_TARGET_PORT;

public:
  enum echo_mode_t {
    ECHO_MODE_SINGLE_FIRST,
    ECHO_MODE_SINGLE_STRONGEST,
    ECHO_MODE_SINGLE_LAST,
    ECHO_MODE_DUAL
  };

  enum scan_resolution_t {
    SCAN_RESOLUTION_90K,
    SCAN_RESOLUTION_60K,
    SCAN_RESOLUTION_30K,
    SCAN_RESOLUTION_15K
  };

public:
  error_t read(const std::string& entry_name, void* value);
  error_t write(const std::string& entry_name, const void* value);
  error_t persist(const std::string& entry_name);

private:
  Settings(Session& session);

private:
  Session& session_;
};

}

#endif
