#ifndef LDCP_SDK_ERROR_H_
#define LDCP_SDK_ERROR_H_

namespace ldcp_sdk
{

enum error_t {
  no_error = 0,
  address_in_use,
  invalid_address,
  connection_refused,
  timed_out,
  link_down,
  connection_lost,
  protocol_error,
  not_supported,
  invalid_params,
  device_error,
  unknown
};

}

#endif
