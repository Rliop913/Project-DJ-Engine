
.. _program_listing_file_include_editor_featureWrapper_GitWrap_Add_AddController.hpp:

Program Listing for File AddController.hpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_editor_featureWrapper_GitWrap_Add_AddController.hpp>` (``include/editor/featureWrapper/GitWrap/Add/AddController.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <git2.h>
   #include <string>
   
   class AddController {
   public:
       git_index* index = nullptr;
       AddController() = default;
       // 파일을 인덱스에 추가. 성공 시 true 반환
       bool open(git_repository* repo);
       bool addFile(const std::string& path);
       ~AddController();
   };
