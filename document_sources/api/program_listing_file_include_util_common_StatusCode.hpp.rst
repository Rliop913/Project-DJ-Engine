
.. _program_listing_file_include_util_common_StatusCode.hpp:

Program Listing for File StatusCode.hpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_common_StatusCode.hpp>` (``include\util\common\StatusCode.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   namespace PDJE_UTIL::common {
   
   enum class StatusCode {
       ok,
       invalid_argument,
       not_found,
       type_mismatch,
       unsupported,
       io_error,
       closed,
       backend_error,
       out_of_range,
       internal_error
   };
   
   } // namespace PDJE_UTIL::common
