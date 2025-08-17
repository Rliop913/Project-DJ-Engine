Core_Engine
=====================

Before Playback
=================


Before Playback Step-1: initialize PDJE core engine
---------------------------------------------

.. doxygenclass:: PDJE
    :project: Project_DJ_Engine

.. tab-set-code:: 

    .. code-block:: c++

        auto engine = new PDJE("database/path");

    .. code-block:: c#

        PDJE engine = new PDJE("database/path");

    .. code-block:: python

        import pdje_POLYGLOT as pypdje
        engine = pypdje.PDJE("database/path")

    .. code-block:: gdscript

        var engine:PDJE_Wrapper = PDJE_Wrapper.new()
        engine.InitEngine("res://database/path")

Before Playback Step-2: Search Music & Track
------------------------------------

.. doxygenfunction:: PDJE::SearchMusic
    :project: Project_DJ_Engine


.. doxygenfunction:: PDJE::SearchTrack
    :project: Project_DJ_Engine

.. tab-set-code:: 

    .. code-block:: c++

        trackdata td = engine->SearchTrack("Track name").front();
        musdata md = engine->SearchMusic("music title", "composer name", -1.0).front();
        //-1.0 means ignore bpm

    .. code-block:: c#

        TRACK_VEC tdvec = engine.SearchTrack("Track name");
        MUS_VEC mdvec = engine.SearchMusic("music title", "composer name", -1.0);
        //-1.0 means ignore bpm

    .. code-block:: python

        import pdje_POLYGLOT as pypdje
        from pdje_POLYGLOT import TRACK_VEC
        from pdje_POLYGLOT import MUS_VEC

        tdvec:TRACK_VEC = engine.SearchTrack("track name")
        mdvec:MUS_VEC = engine.SearchMusic("music title", "composer name", -1.0)
        #-1.0 means ignore bpm

    .. code-block:: gdscript

        var tdlist = engine.SearchTrack("track name")
        var mdlist = engine.SearchMusic("music name", "composer name", -1.0)
        #-1.0 means ignore bpm

Before Playback Step-3: Init, Activate & Deactivate Audio Player
--------------------------------------------------------

.. doxygenfunction:: PDJE::InitPlayer
    :project: Project_DJ_Engine

.. doxygenfunction:: audioPlayer::Activate
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE::ResetPlayer
    :project: Project_DJ_Engine

.. doxygenfunction:: audioPlayer::Deactivate
    :project: Project_DJ_Engine


.. tab-set-code:: 

    .. code-block:: c++

        bool player_OK = engine->InitPlayer(PLAY_MODE::HYBRID_RENDER, td, 48);
        //render mode, trackdata, sample buffer

        engine->ResetPlayer();
        //reset player

        bool activate_OK = engine->player->Activate();
        //start playback

        bool deactivate_OK = engine->player->Deactivate();
        //stop playback

    .. code-block:: c#

        bool player_OK = engine.InitPlayer(PLAY_MODE::HYBRID_RENDER, tdvec[0], 48);
        //render mode, trackdata, sample buffer

        var AudioP = engine.GetPlayer();
        //get player object

        engine.ResetPlayer();
        //reset player object.
        //WARNING: after reset, AudioP becomes unavailable.

        bool activate_OK = AudioP.Activate();
        //start playback

        bool deactivate_OK = AudioP.Deactivate();
        //stop playback

    .. code-block:: python

        from pdje_POLYGLOT import audioPlayer

        player_OK = engine.InitPlayer(PLAY_MODE::HYBRID_RENDER, tdvec.front(), 48)
        #render mode, trackdata, sample buffer

        AudioP:audioPlayer = engine.GetPlayer()
        #get player object

        engine.ResetPlayer();
        #reset player object.
        #WARNING: after reset, AudioP becomes unavailable.

        activate_OK = AudioP.Activate()
        #start playback

        deactivate_OK = AudioP.Deactivate()
        #stop playback

    .. code-block:: gdscript

        var player_OK = engine.InitPlayer(PDJE_Wrapper::HYBRID_RENDER, tdlist[0], 48)
        #render mode, trackdata, sample buffer

        var AudioP:PlayerWrapper = engine.GetPlayer()
        #get player object

        engine.ResetPlayer();
        #reset player object.
        #WARNING: after reset, AudioP becomes unavailable.

        var activate_OK = AudioP.Activate()
        #start playback

        var deactivate_OK = AudioP.Deactivate()
        #stop playback


On Playback
==============


On Playback Step-1: Get & Use FX Controller Pannel
-----------------------------------------------------------

.. doxygenfunction:: audioPlayer::GetFXControlPannel
    :project: Project_DJ_Engine

.. doxygenfunction:: FXControlPannel::FX_ON_OFF
    :project: Project_DJ_Engine

.. doxygenenum:: FXList
    :project: Project_DJ_Engine

.. doxygenfunction:: FXControlPannel::GetArgSetter
    :project: Project_DJ_Engine

.. doxygenfunction:: FXControlPannel::checkSomethingOn
    :project: Project_DJ_Engine

fxArgs_document_here


.. tab-set-code:: 

    .. code-block:: c++

        FXControlPannel* fx_pannel = engine->player->GetFXControlPannel("title");
        //get music's fx controller.

        fx_pannel = engine->player->GetFXControlPannel();
        //or get mixed track's fx controller like this.

        bool TurnON = true;
        fx_pannel->FX_ON_OFF(FXList::EQ, TurnON);
        //turn on EQ effect

        auto argsetter = fx_pannel->GetArgSetter(FXList::EQ);
        for(auto& i : argsetter){
            std::cout << "fx key: " << i.first << std::endl;
        }
        // get argument setter for EQ
        // you can check configurable (settable) arg keys like this

        argsetter["EQSelect"](1);
        argsetter["EQPower"](-32);
        // change FX args by key
        //for details, see FXArgs document

    .. code-block:: c#

        FXControlPannel fx_pannel = AudioP.GetFXControlPannel("title");
        //get music's fx controller

        fx_pannel = AudioP.GetFXControlPannel();
        //or get mixed track's fx controller like this.
        
        bool TurnON = true;
        fx_pannel.FX_ON_OFF(FXList.EQ, TurnON);
        //turn on EQ effect

        ARGSETTER_WRAPPER argsetter = new ARGSETTER_WRAPPER(fx_pannel);
        KEY_VEC keylist = argsetter.GetFXArgKeys(FXList.EQ);
        foreach(var keys in keylist){
            Console.WriteLine(keys);
        }
        //get argument setter wrapper and get configurable arg keys like this.

        argsetter.SetFXArg(FXList.EQ, "EQSelect", 1);
        argsetter.SetFXArg(FXList.EQ, "EQPower", -32);

        //change FX args by key
        //for details, see FXArgs document

    .. code-block:: python

        import pdje_POLYGLOT as pypdje
        from pdje_POLYGLOT import FXControlPannel
        from pdje_POLYGLOT import ARGSETTER_WRAPPER
        from pdje_POLYGLOT import KEY_VEC

        #...

        fx_pannel:FXControlPannel = AudioP.GetFXControlPannel("title")
        #get music's fx controller
        fx_pannel = AudioP.GetFXControlPannel()
        #or get mixed track's fx controller like this.
        TurnON:bool = True
        fx_pannel.FX_ON_OFF(pypdje.EQ, TurnON)
        #turn on EQ effect
        argsetter = ARGSETTER_WRAPPER(fx_pannel)
        keylist:KEY_VEC = argsetter.GetFXArgKeys(pypdje.EQ)

        for i in keylist:
            print("key: ", i)
        #get argument setter wrapper and get configurable arg keys like this.

        argsetter.SetFXArg(pypdje.EQ, "EQSelect", 1)
        argsetter.SetFXArg(pypdje.EQ, "EQPower", -32)
        #change FX args by key
        #for details, see FXArgs document


    .. code-block:: gdscript

        var fx_pannel:FXControlPannel = AudioP.GetFXControlPannel()
        #get mixed track's fx controller
        
        var TurnON:bool = True
        fx_pannel.FX_ON_OFF(EnumWrapper.EQ, TurnON)
        #turn on EQ effect

        var argsetter:FXArgWrapper = fx_pannel.GetArgSetter()
        print(argsetter.GetFXArgKeys(EnumWrapper.EQ))

        #get argument setter wrapper and get configurable arg keys like this.

        argsetter.SetFXArg(EnumWrapper.EQ, "EQSelect", 1)
        argsetter.SetFXArg(EnumWrapper.EQ, "EQPower", -32)
        #change FX args by key
        #for details, see FXArgs document


On Playback Step-2: Get & Use Music Controller Pannel
-----------------------------------------------------------

.. doxygenclass:: MusicControlPannel
    :project: Project_DJ_Engine

.. doxygenfunction:: audioPlayer::GetMusicControlPannel
    :project: Project_DJ_Engine

.. doxygenfunction:: MusicControlPannel::LoadMusic
    :project: Project_DJ_Engine

.. doxygenfunction:: MusicControlPannel::SetMusic
    :project: Project_DJ_Engine

.. doxygenfunction:: MusicControlPannel::CueMusic
    :project: Project_DJ_Engine

.. doxygenfunction:: MusicControlPannel::GetLoadedMusicList
    :project: Project_DJ_Engine

.. doxygenfunction:: MusicControlPannel::getFXHandle
    :project: Project_DJ_Engine

.. tab-set-code:: 

    .. code-block:: c++

        auto musPannel = engine->player->GetMusicControlPannel();
        
        bool musicFound = engine->SearchMusic("title", "composer");
        //find music to playback manually

        bool load_OK = musPannel->LoadMusic(*(engine->DBROOT), musicFound.front());
        // load found music to deck. music won't playback in here

        bool unload_OK = musPannel->UnloadMusic("title");
        // unload music from deck. don't forget for the memory space.
        // the deck always contains loaded music before calling musPannel.UnloadMusic or engine.ResetPlayer

        bool onoff_OK = musPannel->SetMusic("title", true);
        // turn on the music. now music playbacks

        unsigned long long second = 15;
        unsigned long long PCMFrame_position = 15 * 48000;
        //PCMFrame_position = second X SampleRate

        bool cue_OK = musPannel->CueMusic("title", PCMFrame_position);
        //set playback position of the music.

        std::vector<std::string> loaded_list = musPannel->GetLoadedMusicList();
        //get loaded music list.

        FXControlPannel* Fxhandle = musPannel->getFXHandle("title");
        //get music's fx handle

    .. code-block:: c#

        PDJE engine = new PDJE("database/path");

    .. code-block:: python

        import pdje_POLYGLOT as pypdje
        engine = pypdje.PDJE("database/path")

    .. code-block:: gdscript

        var engine:PDJE_Wrapper = PDJE_Wrapper.new()
        engine.InitEngine("res://database/path")