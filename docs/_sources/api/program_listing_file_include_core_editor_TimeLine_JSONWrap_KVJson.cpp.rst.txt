
.. _program_listing_file_include_core_editor_TimeLine_JSONWrap_KVJson.cpp:

Program Listing for File KVJson.cpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_editor_TimeLine_JSONWrap_KVJson.cpp>` (``include\core\editor\TimeLine\JSONWrap\KVJson.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "jsonWrapper.hpp"
   #include <exception>
   
   template <>
   template <>
   bool
   PDJE_JSONHandler<KV_W>::add(const KEY_VALUE &args)
   {
       ROOT[args.first] = args.second;
       return true;
   }
   
   template <>
   template <>
   int
   PDJE_JSONHandler<KV_W>::deleteLine(const KEY &args)
   {
       try {
           if (ROOT.contains(args)) {
               ROOT.erase(args);
               return 1;
           } else {
               return 0;
           }
       } catch (const std::exception &e) {
           critlog("failed on json. What:");
           critlog(e.what());
           return -1;
       }
   }
   
   template <>
   template <>
   void
   PDJE_JSONHandler<KV_W>::getAll(
       std::function<void(const KEY_VALUE &args)> jsonCallback)
   {
       for (auto &[key, value] : ROOT.items()) {
           jsonCallback(KEY_VALUE(key, value.dump()));
       }
   }
   
   template <>
   bool
   PDJE_JSONHandler<KV_W>::load(const fs::path &filepath)
   {
   
       if (fs::exists(filepath)) {
           if (fs::is_regular_file(filepath)) {
               std::ifstream jfile(filepath);
   
               if (!jfile.is_open()) {
                   critlog("failed to open KVJson file. from "
                           "PDJE_JSONHandler<KW_W> load. path: ");
                   critlog(filepath.generic_string());
                   return false;
               }
   
               try {
                   jfile >> ROOT;
               } catch (std::exception &e) {
                   critlog("failed to load data from file. from "
                           "PDJE_JSONHandler<KW_W> load. ErrException: ");
                   critlog(e.what());
                   return false;
               }
   
               jfile.close();
           } else {
               critlog("path is not regular file. from PDJE_JSONHandler<KW_W> "
                       "load. path: ");
               critlog(filepath.generic_string());
               return false;
           }
       } else {
           fs::create_directories(filepath.parent_path());
           std::ofstream jfile(filepath);
           if (!jfile.is_open()) {
               critlog("failed to make or open new json file. from "
                       "PDJE_JSONHandler<KW_W> load. path: ");
               critlog(filepath.generic_string());
               return false;
           }
           jfile << std::setw(4) << ROOT;
           jfile.close();
       }
       return true;
   }
   
   template <>
   template <>
   int
   PDJE_JSONHandler<KV_W>::deleteLine(const DONT_SANITIZE &args,
                                      bool                 skipType,
                                      bool                 skipDetail) = delete;
