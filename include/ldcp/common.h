#ifndef LDCP_SDK_COMMON_H_
#define LDCP_SDK_COMMON_H_

#ifdef _WIN32
#ifdef LDCP_SDK_EXPORTS
#define LDCP_SDK_API __declspec(dllexport)
#else
#define LDCP_SDK_API
#endif
#endif

#endif
