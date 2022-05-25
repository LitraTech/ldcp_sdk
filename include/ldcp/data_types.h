#ifndef LDCP_SDK_DATA_TYPES_H_
#define LDCP_SDK_DATA_TYPES_H_

#include <cstdint>
#include <vector>
#include <array>

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
  union {
    struct {
      uint16_t intensity_width : 1;
      uint16_t range_width : 1;
      uint16_t echo_count : 2;
    } payload_layout;
  } flags;
};
#pragma pack(pop)

template<int Echos = 1>
struct ScanFrame
{
  struct LayerData
  {
    std::vector<std::array<int, Echos>> ranges;
    std::vector<std::array<int, Echos>> intensities;
  };

  unsigned int timestamp;
  std::vector<LayerData> layers;
};

typedef ScanFrame<1> SingleEchoScanFrame;
typedef ScanFrame<2> DualEchoScanFrame;

}

#endif
