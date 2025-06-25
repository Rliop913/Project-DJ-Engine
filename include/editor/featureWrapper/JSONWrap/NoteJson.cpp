#include "jsonWrapper.hpp"




template<>
template<>
bool
PDJE_JSONHandler<NOTE_W>::add(const NoteArgs& args)
{
    nj tempMix = {
        {"Note_Type"    ,   std::string(args.Note_Type.begin()  , args.Note_Type.end())     },
        {"Note_Detail"  ,   std::string(args.Note_Detail.begin(), args.Note_Detail.end())   },
        {"first"        ,   std::string(args.first.begin()      , args.first.end())         },
        {"second"       ,   std::string(args.second.begin()     , args.second.end())        },
        {"third"        ,   std::string(args.third.begin()      , args.third.end())         },
        {"bar"          ,   args.bar        },
        {"beat"         ,   args.beat       },
        {"separate"     ,   args.separate   },
        {"Ebar"         ,   args.Ebar       },
        {"Ebeat"        ,   args.Ebeat      },
        {"Eseparate"    ,   args.Eseparate  }
    };
    if(!ROOT.contains(PDJENOTE)){
        return false;
    }
    ROOT[PDJENOTE].push_back(tempMix);
    return true;
}


template<>
template<>
int
PDJE_JSONHandler<NOTE_W>::deleteLine(const NoteArgs& args)
{
    std::vector<int> targetIDX;
    try{
        for(unsigned long long i=0; i < ROOT[PDJENOTE].size(); ++i){
            auto Target = ROOT[PDJENOTE].at(i);
            if(Target["Note_Type"]  != args.Note_Type   && args.Note_Type   != ""   )   continue;
            if(Target["Note_Detail"]!= args.Note_Detail && args.Note_Detail != ""   )   continue;
            if(Target["first"]      != args.first       && args.first       != ""   )   continue;
            if(Target["second"]     != args.second      && args.second      != ""   )   continue;
            if(Target["third"]      != args.third       && args.third       != ""   )   continue;
            if(Target["bar"]        != args.bar         && args.bar         != -1   )   continue;
            if(Target["beat"]       != args.beat        && args.beat        != -1   )   continue;
            if(Target["separate"]   != args.separate    && args.separate    != -1   )   continue;
            if(Target["Ebar"]       != args.Ebar        && args.Ebar        != -1   )   continue;
            if(Target["Ebeat"]      != args.Ebeat       && args.Ebeat       != -1   )   continue;
            if(Target["Eseparate"]  != args.Eseparate   && args.Eseparate   != -1   )   continue;
            targetIDX.push_back(i);
        }
        for(auto i : targetIDX | vs::reverse){
            ROOT[PDJENOTE].erase(i);
        }
    }
    catch(...){
        return 0;
    }
    return static_cast<int>(targetIDX.size());
}



template<>
template<>
void
PDJE_JSONHandler<NOTE_W>::getAll(
    std::function<void(const NoteArgs& args)> jsonCallback
)
{
    if(!ROOT.contains(PDJENOTE)){
        return;
    }
    for(auto& i : ROOT[PDJENOTE]){

        NoteArgs tempargs{
            i["Note_Type"   ].get<std::string>(),
            i["Note_Detail" ].get<std::string>(),
            i["first"       ].get<std::string>(),
            i["second"      ].get<std::string>(),
            i["third"       ].get<std::string>(),
            i["bar"         ],
            i["beat"        ],
            i["separate"    ],
            i["Ebar"        ],
            i["Ebeat"       ],
            i["Eseparate"   ]
        };
        jsonCallback(tempargs);
    }
}

template<>
std::unique_ptr<NOTE_W> 
PDJE_JSONHandler<NOTE_W>::render()
{
    try{
        auto tempMixBin = std::make_unique<NOTE_W>();
        tempMixBin->makeNew();
        auto rootsz = ROOT[PDJENOTE].size();
        auto filler = tempMixBin->Wp->initDatas(rootsz);
        for(std::size_t i=0; i<rootsz; ++i){
            auto target = ROOT[PDJENOTE].at(i);
            filler[i].setNoteType       (target["Note_Type"     ].get<std::string>());
            filler[i].setNoteDetail     (target["Note_Detail"   ].get<std::string>());
            filler[i].setFirst          (target["first"         ].get<std::string>());
            filler[i].setSecond         (target["second"        ].get<std::string>());
            filler[i].setThird          (target["third"         ].get<std::string>());
            filler[i].setBar            (target["bar"           ]);
            filler[i].setBeat           (target["beat"          ]);
            filler[i].setSeparate       (target["separate"      ]);
            filler[i].setEBar           (target["Ebar"          ]);
            filler[i].setEBeat          (target["Ebeat"         ]);
            filler[i].setESeparate      (target["Eseparate"     ]);
        }
        return tempMixBin;
    }
    catch(...){
        return nullptr;
    }
}



template<>
bool
PDJE_JSONHandler<NOTE_W>::load(const fs::path& path)
{
    auto filepath = path / "notemetadata.PDJE";
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

    if(!ROOT.contains(PDJENOTE)){
        ROOT[PDJENOTE] = nj::array();
    }

    return true;

}
