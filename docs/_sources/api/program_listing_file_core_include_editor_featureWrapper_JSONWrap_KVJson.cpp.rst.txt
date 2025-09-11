
.. _program_listing_file_core_include_editor_featureWrapper_JSONWrap_KVJson.cpp:

Program Listing for File KVJson.cpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_core_include_editor_featureWrapper_JSONWrap_KVJson.cpp>` (``core_include\editor\featureWrapper\JSONWrap\KVJson.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "jsonWrapper.hpp"
   
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
       ROOT.erase(args);
       return 1;
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
   PDJE_JSONHandler<KV_W>::load(const fs::path &path)
   {
   
       auto filepath = path / "keyvaluemetadata.PDJE";
       if (fs::exists(filepath)) {
           if (fs::is_regular_file(filepath)) {
               std::ifstream jfile(filepath);
   
               if (!jfile.is_open()) {
                   critlog("failed to open KVJson file. from "
                           "PDJE_JSONHandler<KW_W> load. path: ");
                   critlog(path.generic_string());
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
               critlog(path.generic_string());
               return false;
           }
       } else {
           fs::create_directories(filepath.parent_path());
           std::ofstream jfile(filepath);
           if (!jfile.is_open()) {
               critlog("failed to make or open new json file. from "
                       "PDJE_JSONHandler<KW_W> load. path: ");
               critlog(path.generic_string());
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
