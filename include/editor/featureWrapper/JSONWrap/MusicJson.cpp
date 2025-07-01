#include "jsonWrapper.hpp"

#include "editorObject.hpp"

template<>
template<>
int 
PDJE_JSONHandler<MUSIC_W>::deleteLine(
        const NoteArgs& args,
        bool skipType, 
        bool skipDetail) = delete;

template<>
template<>
int
PDJE_JSONHandler<MUSIC_W>::deleteLine(const MusicArgs& args)
{
    std::vector<int> targetIDX;
    try{
        for(unsigned long long i=0; i < ROOT[PDJEMUSICBPM].size(); ++i){
            auto Target = ROOT[PDJEMUSICBPM].at(i);
            if(Target["bar"]        != args.bar         && args.bar         != -1   )   continue;
            if(Target["beat"]       != args.beat        && args.beat        != -1   )   continue;
            if(Target["separate"]   != args.separate    && args.separate    != -1   )   continue;
            if(Target["bpm"]        != args.bpm         && args.bpm         != ""   )   continue;
            
            targetIDX.push_back(i);
        }
        for(auto i : targetIDX | vs::reverse){
            ROOT[PDJEMUSICBPM].erase(i);
        }
    }
    catch(...){
        return 0;
    }
    return static_cast<int>(targetIDX.size());
}

template<>
template<>
bool
PDJE_JSONHandler<MUSIC_W>::add(const MusicArgs& args)
{
    nj tempMus = {
        {"bpm"      ,   args.bpm        },
        {"bar"      ,   args.bar        },
        {"beat"     ,   args.beat       },
        {"separate" ,   args.separate   }
    };
    if(!ROOT.contains(PDJEMUSICBPM)){
        return false;
    }
    ROOT[PDJEMUSICBPM].push_back(tempMus);
    return true;
}

template<>
std::unique_ptr<MUSIC_W> 
PDJE_JSONHandler<MUSIC_W>::render()
{
    try{
        auto tempMusBin = std::make_unique<MUSIC_W>();
        tempMusBin->makeNew();
        auto rootsz = ROOT[PDJEMUSICBPM].size();
        auto filler = tempMusBin->Wp->initDatas(rootsz);
        for(std::size_t i=0; i<rootsz; ++i){
            auto target = ROOT[PDJEMUSICBPM].at(i);
            filler[i].setBar        (target["bar"       ]);
            filler[i].setBeat       (target["beat"      ]);
            filler[i].setBpm        (target["bpm"       ].get<DONT_SANITIZE>());
            filler[i].setSeparate   (target["separate"  ]);
        }
        return tempMusBin;
    }
    catch(...){
        return nullptr;
    }
}

template<>
template<>
void
PDJE_JSONHandler<MUSIC_W>::getAll(
    std::function<void(const EDIT_ARG_MUSIC& args)> jsonCallback
)
{
    if(!ROOT.contains(PDJEMUSICBPM)){
        return;
    }
    for(auto& i : ROOT[PDJEMUSICBPM]){
        EDIT_ARG_MUSIC tempargs;
        
        
        tempargs.musicName = PDJE_Name_Sanitizer::getFileName(ROOT["TITLE"].get<SANITIZED>());
        
        auto tempBpm = i["bpm"].get<DONT_SANITIZE>();
        tempargs.arg = {
            tempBpm         ,
            i["bar"         ],
            i["beat"        ],
            i["separate"    ]
        };
        jsonCallback(tempargs);
    }
}
template<>
bool
PDJE_JSONHandler<MUSIC_W>::load(const fs::path& path)
{
    auto filepath = path / "musicmetadata.PDJE";
    if(fs::exists(filepath)){
        if(fs::is_regular_file(filepath)){
            std::ifstream jfile(filepath);
            
            if(!jfile.is_open()) return false;

            try{ jfile >> ROOT; }
            catch(...){ return false; }

            jfile.close();
        }
        else{
            return false;
        }
    }
    else{
        fs::create_directories(filepath.parent_path());
        std::ofstream jfile(filepath);
        if(!jfile.is_open()) return false;
        jfile << std::setw(4) << ROOT;
        jfile.close();
    }

    if(!ROOT.contains(PDJEMUSICBPM)){
        ROOT[PDJEMUSICBPM] = nj::array();
    }

    return true;

}