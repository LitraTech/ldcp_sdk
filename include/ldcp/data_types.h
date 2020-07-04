#ifndef LDCP_SDK_DATA_TYPES_H_
#define LDCP_SDK_DATA_TYPES_H_

#include "ldcp/common.h"

#include <vector>
#include <cstdint>
#include <memory>

namespace ldcp_sdk
{

class LDCP_SDK_API ScanBlock
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

}

#endif
