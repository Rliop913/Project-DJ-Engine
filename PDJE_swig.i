%module pdje_POLYGLOT
%{
    #include <vector>
    #include "PDJE_EXPORT_SETTER.hpp"
    #include "PDJE_interface.hpp"
    #include "dbRoot.hpp"
    #include "musicDB.hpp"
    #include "trackDB.hpp"
    #include <string>
    #include "ManualMix.hpp"
    #include "audioPlayer.hpp"
    #include "fileNameSanitizer.hpp"
    #include "editorObject.hpp"
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
%include "ManualMix.hpp"
%apply std::string {std::filesystem::path};

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
%include "editorObject.hpp"
%include "PDJE_EXPORT_SETTER.hpp"
namespace fs = std::filesystem;

// %include "rocksdb/db.h"
// %include "rocksdb/options.h"
// %include "rocksdb/table.h"
// %include "rocksdb/filter_policy.h"
%template(MUS_VEC) std::vector<musdata>;
%template(TRACK_VEC) std::vector<trackdata>;
%template(KEY_VEC) std::vector<std::string>;
