
.. _program_listing_file_include_PDJE_EXPORT_SETTER.hpp:

Program Listing for File PDJE_EXPORT_SETTER.hpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_PDJE_EXPORT_SETTER.hpp>` (``include\PDJE_EXPORT_SETTER.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #pragma once
   
   #ifdef PDJE_WINDOWS_DLL
   #ifdef PDJE_BUILDING
   #define PDJE_API __declspec(dllexport)
   #else
   #define PDJE_API __declspec(dllimport)
   #endif
   #else
   #define PDJE_API
   #endif
