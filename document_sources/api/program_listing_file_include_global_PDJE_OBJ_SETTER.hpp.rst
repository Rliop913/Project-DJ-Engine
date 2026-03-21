
.. _program_listing_file_include_global_PDJE_OBJ_SETTER.hpp:

Program Listing for File PDJE_OBJ_SETTER.hpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_PDJE_OBJ_SETTER.hpp>` (``include/global/PDJE_OBJ_SETTER.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include <cstdint>
   #include <functional>
   #include <string>
   using OBJ_SETTER_CALLBACK = std::function<void(
       const std::string,
       const uint16_t,
       const std::string,
       const std::string,
       const std::string,
       const unsigned long long,
       const unsigned long long,
       const uint64_t)>;
