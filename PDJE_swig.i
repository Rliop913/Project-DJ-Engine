%module pdje_POLYGLOT
%{
    #include <vector>
    #include "PDJE_interface.hpp"
    #include "dbRoot.hpp"
    #include "musicDB.hpp"
    #include "trackDB.hpp"
    #include <string>
    #include "ManualMix.hpp"
    #include "audioPlayer.hpp"
%}
%include <std_vector.i>
%include <std_string.i>
%include "ManualMix.hpp"
%include "PDJE_interface.hpp"
%include "dbRoot.hpp"
%include "musicDB.hpp"
%include "trackDB.hpp"
%include "audioPlayer.hpp"

%template(MUS_VEC) std::vector<musdata>;
%template(TRACK_VEC) std::vector<trackdata>;
%template(KEY_VEC) std::vector<std::string>;