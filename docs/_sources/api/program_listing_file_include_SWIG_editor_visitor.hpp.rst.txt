
.. _program_listing_file_include_SWIG_editor_visitor.hpp:

Program Listing for File SWIG_editor_visitor.hpp
================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_SWIG_editor_visitor.hpp>` (``include/SWIG_editor_visitor.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "editorObject.hpp"
   
   struct NoteVisitor {
       virtual ~NoteVisitor()
       {
       }
       virtual void
       on_item(const EDIT_ARG_NOTE &o) = 0;
   };
   
   struct MixVisitor {
       virtual ~MixVisitor()
       {
       }
       virtual void
       on_item(const EDIT_ARG_MIX &o) = 0;
   };
   
   struct KVVisitor {
       virtual ~KVVisitor()
       {
       }
       virtual void
       on_item(const EDIT_ARG_KEY_VALUE &o) = 0;
   };
   
   struct MusicVisitor {
       virtual ~MusicVisitor()
       {
       }
       virtual void
       on_item(const EDIT_ARG_MUSIC &o) = 0;
   };
