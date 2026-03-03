
.. _program_listing_file_include_global_PDJE_EXPORT_SETTER.hpp:

Program Listing for File PDJE_EXPORT_SETTER.hpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_PDJE_EXPORT_SETTER.hpp>` (``include/global/PDJE_EXPORT_SETTER.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #pragma once
   
   #if defined(_WIN32) || defined(__CYGWIN__)
   #define PDJE_CALL __cdecl
   #else
   #define PDJE_CALL
   #endif
   
   #ifdef PDJE_WINDOWS_DLL
   #ifdef PDJE_BUILDING
   #define PDJE_API __declspec(dllexport)
   #else
   #define PDJE_API __declspec(dllimport)
   #endif
   #else
   #if defined(__GNUC__) || defined(__clang__)
   #define PDJE_API __attribute__((visibility("default")))
   #else
   #define PDJE_API
   #endif
   #endif
