Path_To_Core_Engine
=====================



Player Step-1: initialize PDJE core engine
---------------------------------------------

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

Player Step-2: Search Music & Track
------------------------------------

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

Player Step-3: Init, Activate & Deactivate Audio Player
--------------------------------------------------------


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

Player Step-4: Get & Use Music Controller Pannel
-----------------------------------------------------------


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

    .. code-block:: c#

        PDJE engine = new PDJE("database/path");

    .. code-block:: python

        import pdje_POLYGLOT as pypdje
        engine = pypdje.PDJE("database/path")

    .. code-block:: gdscript

        var engine:PDJE_Wrapper = PDJE_Wrapper.new()
        engine.InitEngine("res://database/path")