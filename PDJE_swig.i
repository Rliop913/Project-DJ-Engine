%module(directors="1") pdje_POLYGLOT
%{
    #include <vector>
    #include <memory>
    #include "MixBinary.capnp.h"
    #include "PDJE_EXPORT_SETTER.hpp"
    #include <optional>
    #include "PDJE_interface.hpp"
    #include "dbRoot.hpp"
    #include "musicDB.hpp"
    #include "trackDB.hpp"
    #include <string>
    #include "ManualMix.hpp"
    #include "audioPlayer.hpp"
    #include "MusicControlPannel.hpp"
    #include "fileNameSanitizer.hpp"
    #include "editorObject.hpp"
    #include "editorCommit.hpp"
    #include "SWIG_editor_visitor.hpp"
    #include "EditorArgs.hpp"
    #include "audioRender.hpp"
    
    // #include "editorObject.hpp"
    #include "rocksdb/rocksdb_namespace.h"
    #include <filesystem>
    #include "rocksdb/db.h"
    // #include <rocksdb/options.h>
    // #include <rocksdb/table.h>
    // #include <rocksdb/filter_policy.h>
%}

%include "PDJE_EXPORT_SETTER.hpp"

%include <std_vector.i>
%include <std_string.i>
// %include <std_unique_ptr.i>

%rename(TypeEnum) capnp::schemas::TypeEnum_f4ee4873bc65f8f0;
%rename(DetailEnum) capnp::schemas::DetailEnum_c6c88c32e11afb23;
%include "FAKE_CAPNP_ENUMS_FOR_SWIG.hpp"
%include <std_shared_ptr.i>
%shared_ptr(audioPlayer)
%shared_ptr(editorObject)
%shared_ptr(litedb)
// %unique_ptr(litedb)

%include "ManualMix.hpp"

namespace std {
    namespace filesystem {class path;}
}
%include "PDJE_interface.hpp"
namespace rocksdb {

}
namespace ROCKSDB_NAMESPACE = rocksdb;


%include "dbRoot.hpp"
%include "musicDB.hpp"
%include "trackDB.hpp"
%include "audioPlayer.hpp"
%include "fileNameSanitizer.hpp"
%include "MusicControlPannel.hpp"
%include "editorObject.hpp"
%include "editorCommit.hpp"
%include "audioRender.hpp"
%include <std_pair.i>  
namespace fs = std::filesystem;

%template(EDIT_ARG_KEY_VALUE) std::pair<std::string, std::string>;

%rename(EDIT_ARG_NOTE) NoteArgs;
%rename(EDIT_ARG_MIX)  MixArgs;
%include "EditorArgs.hpp"
%include "PDJE_EXPORT_SETTER.hpp"

%feature("director") NoteVisitor;

%feature("director") MixVisitor;

%feature("director") KVVisitor;

%feature("director") MusicVisitor;

%include "SWIG_editor_visitor.hpp"
// %include "jsonWrapper.hpp"


%template(MUS_VEC) std::vector<musdata>;
%template(TRACK_VEC) std::vector<trackdata>;
%template(KEY_VEC) std::vector<std::string>;


// %inline %{
// namespace PDJE_ENUM{
//   enum ITPL_ENUM {
//     ITPL_LINEAR = 0,
//     ITPL_COSINE,
//     ITPL_CUBIC,
//     ITPL_FLAT
//   };
// }

// %}



%inline %{
struct git_oid;
%}

%ignore editorObject::Undo<EDIT_ARG_NOTE>;
%ignore editorObject::Undo<EDIT_ARG_MIX>;
%ignore editorObject::Undo<EDIT_ARG_KEY_VALUE>;
%ignore editorObject::Undo<EDIT_ARG_MUSIC>;

%ignore editorObject::Redo<EDIT_ARG_NOTE>;
%ignore editorObject::Redo<EDIT_ARG_MIX>;
%ignore editorObject::Redo<EDIT_ARG_KEY_VALUE>;
%ignore editorObject::Redo<EDIT_ARG_MUSIC>;

%ignore editorObject::AddLine<EDIT_ARG_NOTE>;
%ignore editorObject::AddLine<EDIT_ARG_MIX>;
%ignore editorObject::AddLine<EDIT_ARG_KEY_VALUE>;
%ignore editorObject::AddLine<EDIT_ARG_MUSIC>;

%ignore editorObject::deleteLine<EDIT_ARG_NOTE>;
%ignore editorObject::deleteLine<EDIT_ARG_KEY_VALUE>;
%ignore editorObject::deleteLine<EDIT_ARG_MUSIC>;

%ignore editorObject::getAll<EDIT_ARG_NOTE>;
%ignore editorObject::getAll<EDIT_ARG_MIX>;
%ignore editorObject::getAll<EDIT_ARG_KEY_VALUE>;
%ignore editorObject::getAll<EDIT_ARG_MUSIC>;

%ignore editorObject::GetDiff<EDIT_ARG_NOTE>;
%ignore editorObject::GetDiff<EDIT_ARG_MIX>;
%ignore editorObject::GetDiff<EDIT_ARG_KEY_VALUE>;
%ignore editorObject::GetDiff<EDIT_ARG_MUSIC>;

%ignore editorObject::GetLogWithJSONGraph<EDIT_ARG_NOTE>;
%ignore editorObject::GetLogWithJSONGraph<EDIT_ARG_MIX>;
%ignore editorObject::GetLogWithJSONGraph<EDIT_ARG_KEY_VALUE>;
%ignore editorObject::GetLogWithJSONGraph<EDIT_ARG_MUSIC>;

%ignore editorObject::Go<EDIT_ARG_NOTE>;
%ignore editorObject::Go<EDIT_ARG_MIX>;
%ignore editorObject::Go<EDIT_ARG_KEY_VALUE>;
%ignore editorObject::Go<EDIT_ARG_MUSIC>;

%ignore editorObject::UpdateLog<EDIT_ARG_NOTE>;
%ignore editorObject::UpdateLog<EDIT_ARG_MIX>;
%ignore editorObject::UpdateLog<EDIT_ARG_KEY_VALUE>;
%ignore editorObject::UpdateLog<EDIT_ARG_MUSIC>;

%extend editorObject {

  // ========== AddLine ==========
  bool AddLineNote(const EDIT_ARG_NOTE& obj) {
    return $self->AddLine<EDIT_ARG_NOTE>(obj);
  }
  bool AddLineMix(const EDIT_ARG_MIX& obj) {
    return $self->AddLine<EDIT_ARG_MIX>(obj);
  }
  bool AddLineKV(const EDIT_ARG_KEY_VALUE& obj) {
    return $self->AddLine<EDIT_ARG_KEY_VALUE>(obj);
  }
  bool AddLineMusic(const EDIT_ARG_MUSIC& obj) {
    return $self->AddLine<EDIT_ARG_MUSIC>(obj);
  }

  // ========== deleteLine ==========
  int deleteLineNote(const EDIT_ARG_NOTE& obj) {
    return $self->deleteLine<EDIT_ARG_NOTE>(obj);
  }
  int deleteLineKV(const EDIT_ARG_KEY_VALUE& obj) {
    return $self->deleteLine<EDIT_ARG_KEY_VALUE>(obj);
  }
  int deleteLineMusic(const EDIT_ARG_MUSIC& obj) {
    return $self->deleteLine<EDIT_ARG_MUSIC>(obj);
  }

  void GetAllNotes(NoteVisitor* v) {
    $self->getAll<EDIT_ARG_NOTE>([&](const EDIT_ARG_NOTE& o){ v->on_item(o); });
  }
  void GetAllMixes(MixVisitor* v) {
    
    $self->getAll<EDIT_ARG_MIX>([&](const EDIT_ARG_MIX& o){ v->on_item(o); });
    
  }
  void GetAllKeyValues(KVVisitor* v) {
    $self->getAll<EDIT_ARG_KEY_VALUE>([&](const EDIT_ARG_KEY_VALUE& o){ v->on_item(o); });
  }
  void GetAllMusics(MusicVisitor* v) {
    $self->getAll<EDIT_ARG_MUSIC>([&](const EDIT_ARG_MUSIC& o){ v->on_item(o); });
  }

  // ========== Undo ==========
  bool UndoNote() { return $self->Undo<EDIT_ARG_NOTE>(); }
  bool UndoMix() { return $self->Undo<EDIT_ARG_MIX>(); }
  bool UndoKV() { return $self->Undo<EDIT_ARG_KEY_VALUE>(); }
  bool UndoMusic(const std::string& musicName) {
    return $self->Undo<EDIT_ARG_MUSIC>(musicName);
  }

  // ========== Redo ==========
  bool RedoNote() { return $self->Redo<EDIT_ARG_NOTE>(); }
  bool RedoMix() { return $self->Redo<EDIT_ARG_MIX>(); }
  bool RedoKV() { return $self->Redo<EDIT_ARG_KEY_VALUE>(); }
  bool RedoMusic(const std::string& musicName) {
    return $self->Redo<EDIT_ARG_MUSIC>(musicName);
  }

  // ========== Go (브랜치 스위치) ==========
  // 편의 오버로드: commitID 생략
  bool GoNote(const std::string& branchName, const std::string& commitID) {
    return $self->Go<EDIT_ARG_NOTE>(branchName, commitID);
  }
  bool GoMix(const std::string& branchName, const std::string& commitID) {
    return $self->Go<EDIT_ARG_MIX>(branchName, commitID);
  }
  bool GoKV(const std::string& branchName, const std::string& commitID) {
    return $self->Go<EDIT_ARG_KEY_VALUE>(branchName, commitID);
  }
  bool GoMusic(const std::string& branchName, const std::string& commitID) {
    return $self->Go<EDIT_ARG_MUSIC>(branchName, commitID);
  }

  // ========== GetLogWithJSONGraph ==========
  // 문자열로 받는 편이 타겟 언어에서 편함
  std::string GetLogNoteJSON() {
    return $self->GetLogWithJSONGraph<EDIT_ARG_NOTE>();
  }
  std::string GetLogMixJSON() {
    return $self->GetLogWithJSONGraph<EDIT_ARG_MIX>();
  }
  std::string GetLogKVJSON() {
    return $self->GetLogWithJSONGraph<EDIT_ARG_KEY_VALUE>();
  }
  std::string GetLogMusicJSON(const std::string& musicName) {
    return $self->GetLogWithJSONGraph<EDIT_ARG_MUSIC>(musicName);
  }

  // ========== GetDiff ==========
  DiffResult GetDiffNote(const gitwrap::commit& oldC, const gitwrap::commit& newC) {
    return $self->GetDiff<EDIT_ARG_NOTE>(oldC, newC);
  }
  DiffResult GetDiffMix(const gitwrap::commit& oldC, const gitwrap::commit& newC) {
    return $self->GetDiff<EDIT_ARG_MIX>(oldC, newC);
  }
  DiffResult GetDiffKV(const gitwrap::commit& oldC, const gitwrap::commit& newC) {
    return $self->GetDiff<EDIT_ARG_KEY_VALUE>(oldC, newC);
  }
  DiffResult GetDiffMusic(const gitwrap::commit& oldC, const gitwrap::commit& newC) {
    return $self->GetDiff<EDIT_ARG_MUSIC>(oldC, newC);
  }

  // ========== UpdateLog ==========
  bool UpdateLogNote() { return $self->UpdateLog<EDIT_ARG_NOTE>(); }
  bool UpdateLogMix() { return $self->UpdateLog<EDIT_ARG_MIX>(); }
  bool UpdateLogKV() { return $self->UpdateLog<EDIT_ARG_KEY_VALUE>(); }
  bool UpdateLogMusic() { return $self->UpdateLog<EDIT_ARG_MUSIC>(); }

  // 브랜치/뮤직 이름으로 지정하는 오버로드
  bool UpdateLogNoteOn(const std::string& branchName) {
    return $self->UpdateLog<EDIT_ARG_NOTE>(branchName);
  }
  bool UpdateLogMixOn(const std::string& branchName) {
    return $self->UpdateLog<EDIT_ARG_MIX>(branchName);
  }
  bool UpdateLogKVOn(const std::string& branchName) {
    return $self->UpdateLog<EDIT_ARG_KEY_VALUE>(branchName);
  }
  bool UpdateLogMusicOn(const std::string& musicName) {
    return $self->UpdateLog<EDIT_ARG_MUSIC>(musicName);
  }
}