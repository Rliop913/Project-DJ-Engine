
.. _program_listing_file_include_util_common_Status.hpp:

Program Listing for File Status.hpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_common_Status.hpp>` (``include\util\common\Status.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "util/common/StatusCode.hpp"
   
   #include <string>
   
   namespace PDJE_UTIL::common {
   
   struct Status {
       StatusCode  code    = StatusCode::ok;
       std::string message = {};
   
       constexpr bool
       ok() const noexcept
       {
           return code == StatusCode::ok;
       }
   };
   
   } // namespace PDJE_UTIL::common
