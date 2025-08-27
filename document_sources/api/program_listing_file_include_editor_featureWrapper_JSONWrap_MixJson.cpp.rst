
.. _program_listing_file_include_editor_featureWrapper_JSONWrap_MixJson.cpp:

Program Listing for File MixJson.cpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_editor_featureWrapper_JSONWrap_MixJson.cpp>` (``include\editor\featureWrapper\JSONWrap\MixJson.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "jsonWrapper.hpp"
   
   template<>
   template<> 
   int 
   PDJE_JSONHandler<MIX_W>::deleteLine(const MixArgs& args) = delete;
   
   
   
   template<>
   template<>
   int
   PDJE_JSONHandler<MIX_W>::deleteLine(
       const MixArgs& args,
       bool skipType, 
       bool skipDetail)
   {
       std::vector<int> targetIDX;
       try{
           for(unsigned long long i=0; i < ROOT[PDJEARR].size(); ++i){
               auto Target = ROOT[PDJEARR].at(i);
               if(Target["type"]       != args.type     && !skipType                   )   continue;
               if(Target["details"]    != args.details  && !skipDetail                 )   continue;
               if(Target["ID"]         != args.ID       && args.ID             != -1   )   continue;
               if(Target["first"]      != args.first    && args.first          != ""   )   continue;
               if(Target["second"]     != args.second   && args.second         != ""   )   continue;
               if(Target["third"]      != args.third    && args.third          != ""   )   continue;
               if(Target["bar"]        != args.bar      && args.bar            != -1   )   continue;
               if(Target["beat"]       != args.beat     && args.beat           != -1   )   continue;
               if(Target["separate"]   != args.separate && args.separate       != -1   )   continue;
               if(Target["Ebar"]       != args.Ebar     && args.Ebar           != -1   )   continue;
               if(Target["Ebeat"]      != args.Ebeat    && args.Ebeat          != -1   )   continue;
               if(Target["Eseparate"]  != args.Eseparate&& args.Eseparate      != -1   )   continue;
               targetIDX.push_back(i);
           }
           for(auto i : targetIDX | vs::reverse){
               ROOT[PDJEARR].erase(i);
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
   PDJE_JSONHandler<MIX_W>::add(const MixArgs& args)
   {
       nj tempMix = {
           {"type"     ,   static_cast<int>(args.type)                         },
           {"details"  ,   static_cast<int>(args.details)                      },
           {"ID"       ,   args.ID                                             },
           {"first"    ,   args.first                                          },
           {"second"   ,   args.second                                         },
           {"third"    ,   args.third                                          },
           {"bar"      ,   args.bar                                            },
           {"beat"     ,   args.beat                                           },
           {"separate" ,   args.separate                                       },
           {"Ebar"     ,   args.Ebar                                           },
           {"Ebeat"    ,   args.Ebeat                                          },
           {"Eseparate",   args.Eseparate                                      }
       };
       if(!ROOT.contains(PDJEARR)){
           critlog("mix json root not found. from PDJE_JSONHandler<MIX_W> add.");
           return false;
       }
       ROOT[PDJEARR].push_back(tempMix);
       return true;
   }
   
   
   template<>
   template<>
   void
   PDJE_JSONHandler<MIX_W>::getAll(
       std::function<void(const MixArgs& args)> jsonCallback
   )
   {
       if(!ROOT.contains(PDJEARR)){
           critlog("mix json root not found. from PDJE_JSONHandler<MIX_W> getAll.");
           return;
       }
       for(auto& i : ROOT[PDJEARR]){
           MixArgs tempargs{
               i["type"        ],
               i["details"     ],
               i["ID"          ],
               i["first"       ],
               i["second"      ],
               i["third"       ],
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
   std::unique_ptr<MIX_W>
   PDJE_JSONHandler<MIX_W>::render()
   {
       try{
           auto tempMixBin = std::make_unique<MIX_W>();
           tempMixBin->makeNew();
           auto rootsz = ROOT[PDJEARR].size();
           auto filler = tempMixBin->Wp->initDatas(rootsz);
           for(std::size_t i=0; i<rootsz; ++i){
               auto target = ROOT[PDJEARR].at(i);
               filler[i].setType       (target["type"      ]);
               filler[i].setDetails    (target["details"   ]);
               filler[i].setId         (target["ID"        ]);
               filler[i].setFirst      (target["first"     ].get<SANITIZED_ORNOT>());
               filler[i].setSecond     (target["second"    ].get<SANITIZED_ORNOT>());
               filler[i].setThird      (target["third"     ].get<SANITIZED_ORNOT>());
               filler[i].setBar        (target["bar"       ]);
               filler[i].setBeat       (target["beat"      ]);
               filler[i].setSeparate   (target["separate"  ]);
               filler[i].setEbar       (target["Ebar"      ]);
               filler[i].setEbeat      (target["Ebeat"     ]);
               filler[i].setEseparate  (target["Eseparate" ]);
           }
   
           return tempMixBin;
       }
       catch(std::exception& e){
           critlog("something wrong. from PDJE_JSONHandler<MIX_W> render. ErrException: ");
           critlog(e.what());
           return nullptr;
       }
   }
   
   
   
   template<>
   bool
   PDJE_JSONHandler<MIX_W>::load(const fs::path& path)
   {
       auto filepath = path / "mixmetadata.PDJE";
       if(fs::exists(filepath)){
           if(fs::is_regular_file(filepath)){
               std::ifstream jfile(filepath);
               
               if(!jfile.is_open()){
                   critlog("cannot open mix json data file. from PDJE_JSONHandler<MIX_W> load. path: ");
                   critlog(path.generic_string());
                   return false;
               } 
   
               try{ jfile >> ROOT; }
               catch(std::exception& e){ 
                   critlog("cannot load mix json data from file. from PDJE_JSONHandler<MIX_W> load. ErrException: ");
                   critlog(e.what());
                   return false; 
               }
   
               jfile.close();
           }
           else{
               critlog("json data file is not regular file. from PDJE_JSONHandler<MIX_W> load. path: ");
               critlog(path.generic_string());
               return false;
           }
       }
       else{
           fs::create_directories(filepath.parent_path());
           std::ofstream jfile(filepath);
           if(!jfile.is_open()){
               critlog("failed to open or make new mix json file. from PDJE_JSONHandler<MIX_W> load. path: ");
               critlog(path.generic_string());
               return false;
           } 
           jfile << std::setw(4) << ROOT;
           jfile.close();
       }
   
       if(!ROOT.contains(PDJEARR)){
           ROOT[PDJEARR] = nj::array();
       }
   
       return true;
   
   }
