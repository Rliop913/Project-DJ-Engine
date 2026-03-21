
.. _program_listing_file_include_global_DataLines_fileNameSanitizer.cpp:

Program Listing for File fileNameSanitizer.cpp
==============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_DataLines_fileNameSanitizer.cpp>` (``include/global/DataLines/fileNameSanitizer.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "fileNameSanitizer.hpp"
   #include <cppcodec/base64_default_url.hpp>
   
   using cbase = cppcodec::base64_url;
   std::optional<SANITIZED>
   PDJE_Name_Sanitizer::sanitizeFileName(const std::string &fileName)
   {
       if (cbase::encoded_size(fileName.size()) >= 255) {
           return std::nullopt;
       }
       return cbase::encode(fileName);
   }
   
   std::string
   PDJE_Name_Sanitizer::getFileName(const SANITIZED &sanitized)
   {
       auto decoded = cbase::decode(sanitized);
       return std::string(decoded.begin(), decoded.end());
   }
