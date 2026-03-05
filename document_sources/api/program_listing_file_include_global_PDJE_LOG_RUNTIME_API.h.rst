
.. _program_listing_file_include_global_PDJE_LOG_RUNTIME_API.h:

Program Listing for File PDJE_LOG_RUNTIME_API.h
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_PDJE_LOG_RUNTIME_API.h>` (``include\global\PDJE_LOG_RUNTIME_API.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "PDJE_EXPORT_SETTER.hpp"
   
   #include <stddef.h>
   #include <stdint.h>
   
   #ifdef __cplusplus
   extern "C" {
   #endif
   
   typedef enum PDJE_LogLevelV1 {
       PDJE_LOG_LEVEL_TRACE_V1    = 0,
       PDJE_LOG_LEVEL_DEBUG_V1    = 1,
       PDJE_LOG_LEVEL_INFO_V1     = 2,
       PDJE_LOG_LEVEL_WARN_V1     = 3,
       PDJE_LOG_LEVEL_ERROR_V1    = 4,
       PDJE_LOG_LEVEL_CRITICAL_V1 = 5,
       PDJE_LOG_LEVEL_OFF_V1      = 6
   } PDJE_LogLevelV1;
   
   typedef enum PDJE_LogBackendV1 {
       PDJE_LOG_BACKEND_INTERNAL_SPDLOG_V1 = 0,
       PDJE_LOG_BACKEND_HOST_CALLBACK_V1   = 1
   } PDJE_LogBackendV1;
   
   typedef enum PDJE_LogInitResultV1 {
       PDJE_LOG_INIT_OK_V1                = 0,
       PDJE_LOG_INIT_ALREADY_INITIALIZED_V1 = 1,
       PDJE_LOG_INIT_INVALID_ARGUMENT_V1  = 2,
       PDJE_LOG_INIT_FAILED_V1            = 3
   } PDJE_LogInitResultV1;
   
   typedef enum PDJE_LogShutdownResultV1 {
       PDJE_LOG_SHUTDOWN_OK_V1             = 0,
       PDJE_LOG_SHUTDOWN_NOT_INITIALIZED_V1 = 1,
       PDJE_LOG_SHUTDOWN_FAILED_V1         = 2
   } PDJE_LogShutdownResultV1;
   
   typedef enum PDJE_LogConfigFlagsV1 {
       PDJE_LOG_CFG_FLAG_DISABLE_AUTOINIT_V1 = 1u << 0
   } PDJE_LogConfigFlagsV1;
   
   typedef void(PDJE_CALL *PDJE_LogHostWriteFnV1)(
       int         level,
       const char *message,
       size_t      message_len,
       void       *user_data);
   
   typedef struct PDJE_LogHostSinkV1 {
       uint32_t             struct_size;
       PDJE_LogHostWriteFnV1 write;
       void                *user_data;
   } PDJE_LogHostSinkV1;
   
   typedef struct PDJE_LogConfigV1 {
       uint32_t         struct_size;
       int              backend;
       int              min_level;
       uint32_t         flags;
       const char      *file_path;
       PDJE_LogHostSinkV1 host_sink;
   } PDJE_LogConfigV1;
   
   PDJE_API int PDJE_CALL
   pdje_logging_init_v1(const PDJE_LogConfigV1 *cfg);
   
   PDJE_API int PDJE_CALL
   pdje_logging_shutdown_v1(void);
   
   PDJE_API int PDJE_CALL
   pdje_logging_is_initialized_v1(void);
   
   PDJE_API void PDJE_CALL
   pdje_log_write_v1(int level, const char *message, size_t message_len);
   
   #ifdef __cplusplus
   }
   #endif
