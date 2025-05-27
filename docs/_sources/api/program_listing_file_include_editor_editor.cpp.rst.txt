
.. _program_listing_file_include_editor_editor.cpp:

Program Listing for File editor.cpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_editor_editor.cpp>` (``include/editor/editor.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "editor.hpp"
   #include <filesystem>
   
   namespace fs = std::filesystem;
   
   bool
   PDJE_Editor::openProject(const std::string& projectPath)
   {
       pt = fs::path(projectPath);
       mixp = pt / "Mixes";
       notep = pt / "Notes";
       musicp = pt / "Musics";
       kvp = pt / "KeyValues";
       
       if(!fs::exists(pt) || !fs::is_directory(pt)){
           fs::create_directory(pt);
           fs::create_directory(mixp);
           fs::create_directory(notep);
           fs::create_directory(musicp);
           fs::create_directory(kvp);
           if(
               !fs::exists(pt)     || !fs::is_directory(pt)    ||
               !fs::exists(mixp)   || !fs::is_directory(mixp)  ||
               !fs::exists(notep)  || !fs::is_directory(notep) ||
               !fs::exists(kvp)  || !fs::is_directory(kvp) ||
               !fs::exists(musicp) || !fs::is_directory(musicp)
           ){ return false; }
       }
       if( !mixHandle.first.Open(mixp.string())|| !mixHandle.second.load(mixp.string()) ||
           !KVHandler.first.Open(mixp.string())|| !KVHandler.second.load(mixp.string()) ||
           !noteHandle.first.Open(notep.string()) || !noteHandle.second.load(notep.string()))
           { return false; }
   
       for(const auto& musicSubpath : fs::directory_iterator(musicp)){
           if(fs::is_directory(musicSubpath)){
               
   
               musicHandle.emplace_back(name, email);
               musicHandle.back().musicName = musicSubpath.path().filename().string();
               if( !musicHandle.back().gith.Open(musicSubpath.path().string()) ||
                   !musicHandle.back().jsonh.load(musicSubpath.path().string())){
                       return false;
                   }
           }
       }
       return true;
       
   }
   
   bool
   PDJE_Editor::AddMusicConfig(const std::string& NewMusicName)
   {
       auto newpath = musicp / NewMusicName;
       if(fs::exists(newpath)){
           return false;
       }
       else{
           fs::create_directory(newpath);
           if(fs::exists(newpath)){
               musicHandle.emplace_back(name, email);
               musicHandle.back().musicName = NewMusicName;
               if( !musicHandle.back().gith.Open(newpath.string()) ||
                   !musicHandle.back().jsonh.load(newpath.string())){
                       return false;
                   }
               else return true;
           }
       }
       return false;
   }
