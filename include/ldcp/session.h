#ifndef LDCP_SDK_SESSION_H_
#define LDCP_SDK_SESSION_H_

#include "ldcp/location.h"
#include "ldcp/transport.h"

#include <rapidjson/document.h>

#include <deque>
#include <condition_variable>

namespace ldcp_sdk
{

class Session
{
public:
  Session();

  void setTimeout(int timeout);

  error_t open(const Location& location);
  void close();
  bool isOpened() const;

  rapidjson::Document createEmptyRequestObject();
  void executeCommand(rapidjson::Document request);
  error_t executeCommand(rapidjson::Document request, rapidjson::Document& response);

  error_t openDataChannel(const in_port_t local_port);

  error_t receiveScanPacket(std::vector<uint8_t>& scan_packet_buffer);

private:
  void onMessageReceived(rapidjson::Document message);
  void onScanPacketReceived(std::vector<uint8_t> scan_packet);

private:
  static const int DEFAULT_TIMEOUT = 3000;
  static const int SCAN_BLOCK_BUFFERING_COUNT = 32;

private:
  int timeout_ms_;

  std::unique_ptr<Transport> transport_;

  int id_;
  std::mutex command_mutex_;

  std::deque<rapidjson::Document> response_queue_;
  std::mutex response_queue_mutex_;
  std::condition_variable response_queue_cv_;

  std::deque<std::vector<uint8_t>> scan_packet_queue_;
  std::mutex scan_packet_queue_mutex_;
  std::condition_variable scan_packet_queue_cv_;
};

}

#endif
