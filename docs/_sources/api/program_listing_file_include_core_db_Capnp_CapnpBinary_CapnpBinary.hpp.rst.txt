
.. _program_listing_file_include_core_db_Capnp_CapnpBinary_CapnpBinary.hpp:

Program Listing for File CapnpBinary.hpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_db_Capnp_CapnpBinary_CapnpBinary.hpp>` (``include/core/db/Capnp/CapnpBinary/CapnpBinary.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <optional>
   #include <vector>
   
   #include <capnp/message.h>
   #include <capnp/serialize.h>
   
   #include "MixBinary.capnp.h"
   #include "MusicBinary.capnp.h"
   #include "NoteBinary.capnp.h"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   
   template <typename DType> class CapReader {
     private:
       std::vector<kj::byte>                        Origin;
       std::optional<capnp::FlatArrayMessageReader> capreader;
   
     public:
       CapReader()  = default;
       ~CapReader() = default;
       std::optional<typename DType::Reader> Rp;
       bool
       open(const std::vector<kj::byte> &capnpBinary)
       {
           try {
               Origin.resize(capnpBinary.size());
               memcpy(Origin.data(), capnpBinary.data(), capnpBinary.size());
               kj::ArrayPtr<const kj::byte> arr(Origin.data(), Origin.size());
               capreader.emplace(::kj::arrayPtr(
                   reinterpret_cast<const capnp::word *>(arr.begin()),
                   arr.size() / sizeof(capnp::word)));
               Rp = capreader->getRoot<DType>();
               return true;
           } catch (std::exception &e) {
               critlog("failed to open capnpBinary. from CapReader open. "
                       "ExceptionLog: ");
               critlog(e.what());
               return false;
           }
       }
       std::vector<kj::byte>
       out()
       {
           return Origin;
       }
   };
   
   template <typename DType> class CapWriter {
     private:
       std::optional<capnp::MallocMessageBuilder> capwriter;
   
     public:
       CapWriter()  = default;
       ~CapWriter() = default;
       std::optional<typename DType::Builder> Wp;
   
       bool
       open(const std::vector<kj::byte> &capnpBinary)
       {
           try {
               kj::ArrayPtr<const kj::byte> arr(capnpBinary.data(),
                                                capnpBinary.size());
               capwriter.emplace();
               Wp = capwriter->initRoot<DType>();
               capnp::FlatArrayMessageReader readed(::kj::arrayPtr(
                   reinterpret_cast<const capnp::word *>(arr.begin()),
                   arr.size() / sizeof(capnp::word)));
               auto                          readroot = readed.getRoot<DType>();
               Wp->initDatas(readroot.getDatas().size());
               Wp->setDatas(readroot.getDatas());
               return true;
           } catch (std::exception &e) {
               critlog("failed to open capnpBinary. from CapWriter open. "
                       "ExceptionLog: ");
               critlog(e.what());
               return false;
           }
       }
   
       bool
       makeNew()
       {
           try {
   
               capwriter.emplace();
               Wp = capwriter->initRoot<DType>();
               return true;
           } catch (std::exception &e) {
               critlog("failed to make new capnpWriter. from CapWriter makeNew. "
                       "ExceptionLog: ");
               critlog(e.what());
               return false;
           }
       }
   
       std::vector<kj::byte>
       out()
       {
           try {
               auto farr  = capnp::messageToFlatArray(capwriter.value());
               auto fbyte = farr.asBytes();
               std::vector<kj::byte> buffer(fbyte.begin(), fbyte.end());
               return buffer;
           } catch (std::exception &e) {
               critlog("failed to return capnp binary datas. from CapWriter out. "
                       "ExceptionLog: ");
               critlog(e.what());
               return std::vector<kj::byte>();
           }
       }
   };
