
.. _program_listing_file_include_global_DataLines_fileNameSanitizer.hpp:

Program Listing for File fileNameSanitizer.hpp
==============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_DataLines_fileNameSanitizer.hpp>` (``include\global\DataLines\fileNameSanitizer.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #pragma once
   #include "PDJE_EXPORT_SETTER.hpp"
   #include <cppcodec/base64_default_url.hpp>
   #include <filesystem>
   #include <optional>
   #include <string>
   namespace fs = std::filesystem;
   
   using SANITIZED = std::string;
   
   using UNSANITIZED = std::string;
   
   using DONT_SANITIZE = std::string;
   
   using SANITIZED_ORNOT = std::string;
   
   using cbase = cppcodec::base64_url;
   
   class PDJE_API PDJE_Name_Sanitizer {
     public:
       static std::optional<SANITIZED>
       sanitizeFileName(const std::string &fileName);
   
       static std::string
       getFileName(const SANITIZED &sanitized);
   
       PDJE_Name_Sanitizer()  = delete;
       ~PDJE_Name_Sanitizer() = delete;
   };
