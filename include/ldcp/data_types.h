#ifndef LDCP_SDK_DATA_TYPES_H_
#define LDCP_SDK_DATA_TYPES_H_

#include <vector>
#include <cstdint>
#include <memory>

namespace ldcp_sdk
{

#pragma pack(push, 1)
struct ScanPacketHeader
{
  uint16_t signature;
  uint16_t frame_index;
  uint8_t block_index;
  uint8_t block_count;
  uint16_t block_length;
  uint32_t timestamp;
  uint16_t checksum;
  struct {
    uint16_t payload_layout : 4;
    uint16_t reserved : 12;
  } flags;
};
#pragma pack(pop)

class ScanBlock
{
public:
  class BlockData
  {
  public:
    std::vector<uint16_t> ranges;
    std::vector<uint8_t> intensities;
  };

  uint8_t block_id;
  uint32_t timestamp;
  std::vector<BlockData> layers;
};

class ScanFrame
{
public:
  class FrameData
  {
  public:
    std::vector<uint16_t> ranges;
    std::vector<uint8_t> intensities;
  };

  uint32_t timestamp;
  std::vector<FrameData> layers;
};

}

#endif
