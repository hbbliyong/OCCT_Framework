// songyun_export.h
#pragma once

#if defined(_WIN32)
#ifdef SONGYUN_EXPORTS
#define SONGYUN_API __declspec(dllexport)
#else
#define SONGYUN_API __declspec(dllimport)
#endif
#else
// Linux: 仅编译库时标记导出，客户端空宏
#if defined(SONGYUN_EXPORTS)
#define SONGYUN_API __attribute__((visibility("default")))
#else
#define SONGYUN_API
#endif
#endif

// // 关键补充：禁用客户端的 dllexport/dllimport
// #ifdef __cplusplus
// extern "C" {
// #endif
//     // 所有导出函数必须用 extern "C" 包裹（避免 C++ 名称修饰）
//     SONGYUN_API void songyun_init();
// #ifdef __cplusplus
// }
// #endif