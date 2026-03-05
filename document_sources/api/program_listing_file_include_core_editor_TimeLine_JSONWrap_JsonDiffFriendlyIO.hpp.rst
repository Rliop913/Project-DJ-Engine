
.. _program_listing_file_include_core_editor_TimeLine_JSONWrap_JsonDiffFriendlyIO.hpp:

Program Listing for File JsonDiffFriendlyIO.hpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_editor_TimeLine_JSONWrap_JsonDiffFriendlyIO.hpp>` (``include\core\editor\TimeLine\JSONWrap\JsonDiffFriendlyIO.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <filesystem>
   #include <fstream>
   #include <ostream>
   #include <sstream>
   #include <string>
   
   #include <nlohmann/json.hpp>
   
   namespace PDJE_JSON_IO_DETAIL {
   
   inline void
   DumpDiffFriendlyArray(std::ostream &os, const nlohmann::json &arr, int indent);
   
   inline void
   DumpDiffFriendlyObject(std::ostream      &os,
                          const nlohmann::json &obj,
                          int                indent)
   {
       os << "{";
       if (!obj.is_object() || obj.empty()) {
           os << "}";
           return;
       }
   
       os << "\n";
       bool first = true;
       for (auto it = obj.begin(); it != obj.end(); ++it) {
           os << std::string(static_cast<std::size_t>(indent + 2), ' ');
           if (!first) {
               os << ", ";
           }
           os << nlohmann::json(it.key()).dump() << ": ";
           if (it.value().is_array()) {
               DumpDiffFriendlyArray(os, it.value(), indent + 2);
           } else {
               os << it.value().dump();
           }
           os << "\n";
           first = false;
       }
       os << std::string(static_cast<std::size_t>(indent), ' ') << "}";
   }
   
   inline void
   DumpDiffFriendlyArray(std::ostream      &os,
                         const nlohmann::json &arr,
                         int                indent)
   {
       os << "[";
       if (!arr.is_array() || arr.empty()) {
           os << "]";
           return;
       }
   
       os << "\n";
       bool first = true;
       for (const auto &elem : arr) {
           os << std::string(static_cast<std::size_t>(indent + 2), ' ');
           if (!first) {
               os << ", ";
           }
           os << elem.dump();
           os << "\n";
           first = false;
       }
       os << std::string(static_cast<std::size_t>(indent), ' ') << "]";
   }
   
   inline std::string
   DumpDiffFriendlyJson(const nlohmann::json &root)
   {
       std::ostringstream oss;
       if (root.is_object()) {
           DumpDiffFriendlyObject(oss, root, 0);
       } else if (root.is_array()) {
           DumpDiffFriendlyArray(oss, root, 0);
       } else {
           oss << root.dump();
       }
       oss << "\n";
       return oss.str();
   }
   
   inline bool
   WriteDiffFriendlyJsonToFile(const std::filesystem::path &path,
                               const nlohmann::json        &root)
   {
       try {
           std::ofstream jfile(path);
           if (!jfile.is_open()) {
               return false;
           }
           jfile << DumpDiffFriendlyJson(root);
           return jfile.good();
       } catch (...) {
           return false;
       }
   }
   
   } // namespace PDJE_JSON_IO_DETAIL
