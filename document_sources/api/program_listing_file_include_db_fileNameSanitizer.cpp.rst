
.. _program_listing_file_include_db_fileNameSanitizer.cpp:

Program Listing for File fileNameSanitizer.cpp
==============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_db_fileNameSanitizer.cpp>` (``include\db\fileNameSanitizer.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "fileNameSanitizer.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   
   std::optional<SANITIZED> 
   PDJE_Name_Sanitizer::sanitizeFileName(const std::string& fileName)
   {
       if(cbase::encoded_size(fileName.size()) >= 255){
           critlog("failed to sanitize filename. from PDJE_Name_Sanitizer sanitizeFileName. ErrfileName: ");
           critlog(fileName);
           return std::nullopt;
       }
       return cbase::encode(fileName);
   }
   
   
   std::string
   PDJE_Name_Sanitizer::getFileName(const SANITIZED& sanitized)
   {
       auto decoded = cbase::decode(sanitized);
       return std::string(decoded.begin(), decoded.end());
   }
