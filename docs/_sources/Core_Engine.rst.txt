Core_Engine
=====================

Before Playback
=================


Before Playback Step-1: initialize PDJE core engine
-----------------------------------------------------

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
-----------------------------------------------------

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
------------------------------------------------------------------------

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

        bool player_OK = engine.InitPlayer(PLAY_MODE.HYBRID_RENDER, tdvec[0], 48);
        //render mode, trackdata, sample buffer

        var AudioP = engine.GetPlayerObject();
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

        player_OK = engine.InitPlayer(pyPDJE.HYBRID_RENDER, tdvec[0], 48)
        #render mode, trackdata, sample buffer

        AudioP:audioPlayer = engine.GetPlayerObject()
        #get player object

        engine.ResetPlayer()
        #reset player object.
        #WARNING: after reset, AudioP becomes unavailable.

        activate_OK = AudioP.Activate()
        #start playback

        deactivate_OK = AudioP.Deactivate()
        #stop playback

    .. code-block:: gdscript

        var player_OK = engine.InitPlayer(PDJE_Wrapper.HYBRID_RENDER, tdlist[0], 48)
        #render mode, trackdata, sample buffer

        var AudioP:PlayerWrapper = engine.GetPlayer()
        #get player object

        engine.ResetPlayer()
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



to see Available args, See: :doc:`/FX_ARGS`

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
        
        var TurnON = true
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
        
        auto musicFound = engine->SearchMusic("title", "composer");
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

        
        MusicControlPannel musPannel = AudioP.GetMusicControlPannel();
        
        var musicFound = engine.SearchMusic("title", "composer");
        //find music to playback manually

        bool load_OK = musPannel.LoadMusic(engine.DBROOT, musicFound[0]);
        // load found music to deck. music won't playback in here

        bool unload_OK = musPannel.UnloadMusic("title");
        // unload music from deck. don't forget for the memory space.
        // the deck always contains loaded music before calling musPannel.UnloadMusic or engine.ResetPlayer

        bool onoff_OK = musPannel.SetMusic("title", true);
        // turn on the music. now music playbacks

        ulong second = 15;
        ulong PCMFrame_position = 15 * 48000;
        //PCMFrame_position = second X SampleRate

        bool cue_OK = musPannel.CueMusic("title", PCMFrame_position);
        //set playback position of the music.

        KEY_VEC loaded_list = musPannel.GetLoadedMusicList();
        //get loaded music list.

        FXControlPannel Fxhandle = musPannel.getFXHandle("title");
        //get music's fx handle

    .. code-block:: python

        import pdje_POLYGLOT as pyPDJE
        from pdje_POLYGLOT import MusicControlPannel
        from pdje_POLYGLOT import FXControlPannel
        from pdje_POLYGLOT import audioPlayer
        from pdje_POLYGLOT import MUS_VEC
        
        musPannel:MusicControlPannel = engine.player.GetMusicControlPannel()
        
        musicFound:MUS_VEC = engine.SearchMusic("title", "composer")
        #find music to playback manually

        load_OK = musPannel.LoadMusic(engine.DBROOT, musicFound[0])
        # load found music to deck. music won't playback in here

        unload_OK = musPannel.UnloadMusic("title")
        # unload music from deck. don't forget for the memory space.
        # the deck always contains loaded music before calling musPannel.UnloadMusic or engine.ResetPlayer

        onoff_OK = musPannel.SetMusic("title", True)
        # turn on the music. now music playbacks

        second:int = 15
        PCMFrame_position:int = 15 * 48000
        #PCMFrame_position = second X SampleRate

        cue_OK = musPannel.CueMusic("title", PCMFrame_position)
        #set playback position of the music.

        loaded_list = musPannel.GetLoadedMusicList()
        #get loaded music list.

        Fxhandle:FXControlPannel = musPannel.getFXHandle("title")
        #get music's fx handle

    .. code-block:: gdscript

        var musPannel:MusPannelWrapper = AudioP.GetMusicControlPannel()
        
        var musicFound = engine.SearchMusic("title", "composer", -1.0)
        #find music to playback manually

        var load_OK = musPannel.LoadMusic("title", "composer", -1.0)
        # load found music to deck. music won't playback in here

        var unload_OK = musPannel.UnloadMusic("title")
        # unload music from deck. don't forget for the memory space.
        # the deck always contains loaded music before calling musPannel.UnloadMusic or engine.ResetPlayer

        var onoff_OK = musPannel.SetMusic("title", true)
        # turn on the music. now music playbacks

        var second = 15
        var PCMFrame_position = 15 * 48000
        #PCMFrame_position = second X SampleRate

        var cue_OK = musPannel.CueMusic("title", PCMFrame_position)
        #set playback position of the music.

        var loaded_list = musPannel.GetLoadedMusicList()
        #get loaded music list.

        var Fxhandle:FXWrapper = musPannel.getFXHandle("title")
        #get music's fx handle

Editor API
=============

The PDJE editor provides only an API and does not include a built-in graphical user interface (GUI).

With the editor API, you can:

- Add audio files to the PDJE database (DB) and generate metadata
- Create and edit mix sets (combinations of multiple tracks) using the registered music in the DB
- Create and edit note data that can be used in rhythm games

Editor Step-1: Create & Manage DB
----------------------------------------


.. doxygenfunction:: PDJE::InitEditor
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE::CloseEditor
    :project: Project_DJ_Engine

.. doxygenfunction:: PDJE::GetEditorObject
    :project: Project_DJ_Engine

.. doxygenfunction:: editorObject::DESTROY_PROJECT
    :project: Project_DJ_Engine


.. tab-set-code:: 

    .. code-block:: c++

        bool initRes = engine->InitEditor("my name", "my@email, no need to fill", "ProjectRoot");
        auto editor = engine->GetEditorObject();
        std::string destroyRes = editor->DESTROY_PROJECT();
        engine->CloseEditor();

    .. code-block:: c#

        bool initRes = engine.InitEditor("my name", "my@email, no need to fill", "ProjectRoot");
        var editor = engine.GetEditorObject();
        var destroyRes = engine.DESTROY_PROJECT();
        engine.CloseEditor();

    .. code-block:: python

        init_res = engine.InitEditor("my name", "my@email, no need to fill", "ProjectRoot")
        editor:editorObject = engine.GetEditorObject()
        destroy_res = editor.DESTROY_PROJECT()
        engine.CloseEditor()

    .. code-block:: gdscript

        var init_res = engine.InitEditor("my name", "my@email, no need to fill", "ProjectRoot")
        var editor = engine.GetEditor()
        editor.DESTROY_PROJECT()
        engine.CloseEditor()


Editor Step-2: Editing Control & History view
-------------------------------------------------

To see all functions, check :doc:`/api/classeditorObject`

Undo
------

.. tab-set-code:: 

    .. code-block:: c++

        // Undo Mix
        bool undoRes = editor->Undo<EDIT_ARG_MIX>();
        
        // Undo Note
        undoRes = editor->Undo<EDIT_ARG_NOTE>();

        // Undo KV
        undoRes = editor->Undo<EDIT_ARG_KEY_VALUE>();
        
        // Undo Music
        undoRes = editor->Undo<EDIT_ARG_MUSIC>();
        
    .. code-block:: c#

        bool undoRes = editor->UndoNote();
        undoRes = editor->UndoMix();
        undoRes = editor->UndoKV();
        undoRes = editor->UndoMusic();

    .. code-block:: python

        import pdje_POLYGLOT as pypdje
        from pdje_POLYGLOT import editorObject
        undoRes = editor.UndoNote()
        undoRes = editor.UndoKV()
        undoRes = editor.UndoMix()
        undoRes = editor.UndoMusic()
        
    .. code-block:: gdscript

        var undoRes = editor.Undo(editor.NOTE, "")
        undoRes = editor.Undo(editor.MUSIC, "music name")
        undoRes = editor.Undo(editor.MIX, "")
        undoRes = editor.Undo(editor.KV, "")

Redo
-------

.. tab-set-code:: 

    .. code-block:: c++

        // Redo Mix
        bool RedoRes = editor->Redo<EDIT_ARG_MIX>();
        
        // Redo Note
        RedoRes = editor->Redo<EDIT_ARG_NOTE>();

        // Redo KV
        RedoRes = editor->Redo<EDIT_ARG_KEY_VALUE>();
        
        // Redo Music
        RedoRes = editor->Redo<EDIT_ARG_MUSIC>();
        
    .. code-block:: c#

        bool RedoRes = editor->RedoNote();
        RedoRes = editor->RedoMix();
        RedoRes = editor->RedoKV();
        RedoRes = editor->RedoMusic();

    .. code-block:: python

        import pdje_POLYGLOT as pypdje
        from pdje_POLYGLOT import editorObject
        RedoRes = editor.RedoNote()
        RedoRes = editor.RedoKV()
        RedoRes = editor.RedoMix()
        RedoRes = editor.RedoMusic()
        
    .. code-block:: gdscript

        var RedoRes = editor.Redo(editor.NOTE, "")
        RedoRes = editor.Redo(editor.MUSIC, "music name")
        RedoRes = editor.Redo(editor.MIX, "")
        RedoRes = editor.Redo(editor.KV, "")


Time travel
-------------

To get necessary args, See :ref:`get-edit-logs`

.. tab-set-code:: 

    .. code-block:: c++
        
        //get note edit logs
        std::string logs = editor->GetLogWithJSONGraph<EDIT_ARG_NOTE>();
        // std::string logs = editor->GetLogWithJSONGraph<EDIT_ARG_MIX>();
        // std::string logs = editor->GetLogWithJSONGraph<EDIT_ARG_KEY_VALUE>();
        // std::string logs = editor->GetLogWithJSONGraph<EDIT_ARG_MUSIC>("music name"); // this is the only difference
        
        //parse json. try print them.
        auto jj = nlohmann::json::parse(logs);
        
        //the "BRANCH" has branch lists. this is json array.
        std::string branchName = jj["BRANCH"].at(0)["NAME"];
        std::string branch_head_oid = jj["BRANCH"].at(0)["OID"];
        // "BRANCH" is an array of { "NAME": "...", "OID": "..." }
        //it has two keys. "NAME", "OID". the oid means the head commit oid of the branch.

        bool GoRes = editor->Go<EDIT_ARG_NOTE>(branchName, branch_head_oid);
        //bool GoRes = editor->Go<EDIT_ARG_MIX>(branchName, branch_head_oid);
        //bool GoRes = editor->Go<EDIT_ARG_KEY_VALUE>(branchName, branch_head_oid);
        //bool GoRes = editor->Go<EDIT_ARG_MUSIC>(branchName, branch_head_oid);

    .. code-block:: c#

        //get logs
        string logs = editor.GetLogMixJSON();
        //string logs = editor.GetLogNoteJSON();
        //string logs = editor.GetLogMusicJSON("music name");
        //string logs = editor.GetLogKVJSON();

        //get branch name, oid from logs

        // editor.GoNote(name, oid);
        editor.GoMix(name, oid);
        // editor.GoKV(name, oid);
        // editor.GoMusic(name, oid);
        
        

    .. code-block:: python
        
        import pdje_POLYGLOT as pypdje
        from pdje_POLYGLOT import editorObject

        #...

        logs = editor.GetLogMixJSON()
        # logs = editor.GetLogKVJSON()
        # logs = editor.GetLogMusicJSON("music name")
        # logs = editor.GetLogNoteJSON()

        # get branch name, oid from logs

        # editor.GoKV(name, oid)
        # editor.GoNote(name, oid)
        # editor.GoMusic(name, oid)
        editor.GoMix(name, oid)

    .. code-block:: gdscript

        var logs = editor.GetLogWithJSONGraph(editor.NOTE, "")
        # var logs = editor.GetLogWithJSONGraph(editor.MUSIC, "music name")
        # var logs = editor.GetLogWithJSONGraph(editor.KV, "")
        # var logs = editor.GetLogWithJSONGraph(editor.MIX, "")

        #get branch name, oid from logs

        editor.Go(editor.NOTE, name, oid)
        # editor.Go(editor.MUSIC, name, oid)
        # editor.Go(editor.MIX, name, oid)
        # editor.Go(editor.KV, name, oid)
        



Add line
-----------

See :ref:`about-mix-data` first.

.. tab-set-code:: 

    .. code-block:: c++

        EDIT_ARG_MIX mixs;
        
        mixs.type = TypeEnum::FILTER; //Filter
        mixs.details = DetailEnum::LOW; //Low pass

        mixs.ID = 1; //Deck number. access music with this.

        mixs.first = ITPL_ENUM::ITPL_COSINE; // first arg
        mixs.second = "5000,1000,2000,3000,4000,5000,5500,6000"; // second arg, eight point values
        // mixs.first = ITPL_ENUM::ITPL_FLAT; // if no need interpolation
        // mixs.second = "5000"; // just one value
        
        mixs.third = "NONE"; // third arg

        mixs.beat = 0;
        mixs.subBeat = 0;
        mixs.separate = 0;
        //"start_position" = beat + (subBeat / separate)

        mixs.Ebeat = 16;//end beat
        mixs.EsubBeat = 2;//end subBeat
        mixs.Eseparate = 4;//end separate
        //"end_position" = ebeat + (esubBeat / eseparate)

        //summation: add low pass filter from "start_position" to "end_position" with interpolation

        editor->AddLine(mixs);//add mix data

        // EDIT_ARG_NOTE data;
        // EDIT_ARG_KEY_VALUE data;
        // EDIT_ARG_MUSIC data;

        // editor->AddLine(data);

        // editor->AddLine("music name", "48000") // this changes the music's first beat position


    .. code-block:: c#

        EDIT_ARG_MIX mixs = new EDIT_ARG_MIX();

        mixs.type = TypeEnum.FILTER; //Filter
        mixs.details = DetailEnum.LOW; //Low pass

        mixs.ID = 1; //Deck number. access music with this.

        mixs.first = ITPL_ENUM.ITPL_COSINE.ToString(); // first arg
        mixs.second = "5000,1000,2000,3000,4000,5000,5500,6000"; // second arg, eight point values
        // mixs.first = ITPL_ENUM.ITPL_FLAT.ToString(); // if no need interpolation
        // mixs.second = "5000"; // just one value

        mixs.third = "NONE"; // third arg

        mixs.beat = 0;
        mixs.subBeat = 0;
        mixs.separate = 0;
        //"start_position" = beat + (subBeat / separate)

        mixs.Ebeat = 16;//end beat
        mixs.EsubBeat = 2;//end subBeat
        mixs.Eseparate = 4;//end separate
        //"end_position" = ebeat + (esubBeat / eseparate)

        //summation: add low pass filter from "start_position" to "end_position" with interpolation

        engine.editor.AddLineMix(mixs);//add mix data
        // EDIT_ARG_NOTE data;
        // editor.AddLineNote(data);

        // EDIT_ARG_KEY_VALUE data;
        // editor.AddLineKV(data);

        // EDIT_ARG_MUSIC data;
        // editor.AddLineMusic(data);

        // editor.AddLine("music name", "48000"); // this changes the music's first beat position.

    .. code-block:: python

        import pdje_POLYGLOT as pypdje

        from pdje_POLYGLOT import EDIT_ARG_MIX
        from pdje_POLYGLOT import EDIT_ARG_MUSIC
        from pdje_POLYGLOT import EDIT_ARG_KEY_VALUE

        from pdje_POLYGLOT import EDIT_ARG_NOTE
        from pdje_POLYGLOT import editorObject

        mixs = EDIT_ARG_MIX()

        mixs.type = pypdje.TypeEnum_FILTER #Filter
        mixs.details = pypdje.DetailEnum_LOW #Low pass

        mixs.ID = 1 #Deck number. access music with this.

        mixs.first = pypdje.ITPL_COSINE.ToString() # first arg
        mixs.second = "5000,1000,2000,3000,4000,5000,5500,6000" # second arg, eight point values
        # mixs.first = pypdje.ITPL_FLAT.ToString() # if no need interpolation
        # mixs.second = "5000" # just one value

        mixs.third = "NONE" # third arg

        mixs.beat = 0
        mixs.subBeat = 0
        mixs.separate = 0
        #"start_position" = beat + (subBeat / separate)

        mixs.Ebeat = 16#end beat
        mixs.EsubBeat = 2#end subBeat
        mixs.Eseparate = 4#end separate
        #"end_position" = ebeat + (esubBeat / eseparate)

        #summation: add low pass filter from "start_position" to "end_position" with interpolation

        editor.AddLineMix(mixs)#add mix data
        # data = EDIT_ARG_NOTE()
        # editor.AddLineNote(data)

        # data = EDIT_ARG_KEY_VALUE()
        # editor.AddLineKV(data)

        # data = EDIT_ARG_MUSIC()
        # editor.AddLineMusic(data)

        # editor.AddLine("music name", "48000") # this changes the music's first beat position.

    .. code-block:: gdscript
        
        var mixs = PDJE_EDITOR_ARG.new()
        mixs.InitMixArg(
            PDJE_EDITOR_ARG.EDITOR_TYPE_LIST.FILTER,
            PDJE_EDITOR_ARG.EDITOR_DETAIL_LIST.LOW,
            1,
            "1",
            "5000,1000,2000,3000,4000,5000,5500,6000",
            "NONE",
            16,
            0,
            0,
            32,
            0,
            0)
        #"start_position" = beat + (subBeat / separate)
        #"end_position" = ebeat + (esubBeat / eseparate)
        #summation: add low pass filter from "start_position" to "end_position" with interpolation
        editor.AddLine(mixs)#add mix data
        
        var data = PDJE_EDITOR_ARG.new()
        data.InitKeyValueArg("", "")
        editor.AddLine(data) #add key-value args
        
        data = PDJE_EDITOR_ARG.new()
        data.InitNoteArg(
            "something. ex- tap note", 
            "something. ex- color red", 
            "something. ex- line number", 
            "something. ex- tap sound",
            "something. ex- ??",
            4,0,0,
            4,0,0)
        editor.AddLine(data)#add custom simple red tap note data on music's 4th beat
        
        data = PDJE_EDITOR_ARG.new()
        data.InitMusicArg("temp music title", "120", 0, 0, 0)
        editor.AddLine(data)#music starts with 120 bpm
        var data2 = PDJE_EDITOR_ARG.new()
        data2.InitMusicArg("temp music title", "240", 64, 0, 0)
        editor.AddLine(data2)#music starts with 120 bpm and it changes into 240 bpm in 64th beat
        


Get all lines
--------------

.. tab-set-code:: 

    .. code-block:: c++

        std::vector<EDIT_ARG_MIX> mixFound;
        editor->getAll([&mixFound](const EDIT_ARG_MIX& mix_arg){//single threaded. will add multithreaded , faster getter soon.
            if(mix_arg.beat < 50){
                mixFound.push_back();
            }
        });

        // std::vector<EDIT_ARG_NOTE> Found;
        // editor->getAll([&Found](const EDIT_ARG_NOTE& arg);

        // std::vector<EDIT_ARG_KEY_VALUE> Found;
        // editor->getAll([&Found](const EDIT_ARG_KEY_VALUE& arg);

        // std::vector<EDIT_ARG_MUSIC> Found;
        // editor->getAll([&Found](const EDIT_ARG_MUSIC& arg);
        

    .. code-block:: c#

        MixCall caller = new MixCall();

        editor.GetAllMixes(caller);

        // editor.GetAllKeyValues(caller);
        // editor.GetAllMusics(caller);
        // editor.GetAllNotes(caller);

        public class MixCall : MixVisitor { // MusicVisitor, NoteVisitor, KVVisitor
            public List<EDIT_ARG_MIX> mixs = new List<EDIT_ARG_MIX>();
            public override void on_item(EDIT_ARG_MIX o)// override this function
            {
                if (o.beat > 50)
                {
                    mixs.Add(o);
                }
            }
        };// Just change MIX into NOTE, KEY_VALUE, MUSIC


    .. code-block:: python

        import pdje_POLYGLOT as pypdje
        
        class MixCall(pypdje.MixVisitor):# MusicVisitor, NoteVisitor, KVVisitor
            def __init__(self):
                super().__init__()
                self.mixs = []

            def on_item(self, o:EDIT_ARG_MIX):# def this function
                if o.beat > 50:
                    self.mixs.append(o)

        caller = MixCall()

        editor.GetAllMixes(caller)

        # editor.GetAllKeyValues(caller)
        # editor.GetAllMusics(caller)
        # editor.GetAllNotes(caller)

        for i in caller.mixs:
            print(i)
            

    .. code-block:: gdscript

        var edited_data:Dictionary = editor.getAll()

Delete line
--------------

.. tab-set-code:: 

    .. code-block:: c++

        std::vector<EDIT_ARG_NOTE> noteFound;

        //find delete targets with getAll function.

        for(const auto& delete_target : noteFound){
            editor->deleteLine(delete_target);
        }

        std::vector<EDIT_ARG_MIX> mixFound;

        //find delete targets with getAll function.

        bool skipType = true;
        bool skipDetail = false;
        for(const auto& delete_target : mixFound){
            editor->deleteLine(delete_target, skipType, skipDetail); //mix arg is special.
        }

    .. code-block:: c#

        MixCall caller = new MixCall();

        editor.GetAllMixes(caller);
        bool skipType = true;
        bool skipDetail = false;
        foreach (var target in caller.mixs)
        {
            editor.deleteLine(target, skipType, skipDetail);
            // editor.deleteLineKV(target);
            // editor.deleteLineMusic(target);
            // editor.deleteLineNote(target);
        }

    .. code-block:: python

        class MixCall(pypdje.MixVisitor):
            def __init__(self):
                super().__init__()
                self.mixs = []

            def on_item(self, o:EDIT_ARG_MIX):
                if o.beat > 50:
                    self.mixs.append(o)

        caller = MixCall()

        editor.GetAllMixes(caller)

        skip_type = False
        skip_detail = True

        for i in caller.mixs:
            editor.deleteLine(i, skip_type, skip_detail)
            # editor.deleteLineKV(i)
            # editor.deleteLineMusic(i)
            # editor.deleteLineNote(i)

    .. code-block:: gdscript

        var edited_data:Dictionary = editor.getAll()
        for delete_target in edited_data["noteDatas"]:
            var target = PDJE_EDITOR_ARG.new()
            target.InitNoteArg(
                delete_target["Note_Type"],
                delete_target["Note_Detail"],
                delete_target["first"],
                delete_target["second"],
                delete_target["third"],
                delete_target["beat"],
                delete_target["subBeat"],
                delete_target["separate"],
                delete_target["Ebeat"],
                delete_target["EsubBeat"],
                delete_target["Eseparate"],
                )
            editor.deleteLine(target, false, false)


.. Get diff
.. ------------

.. .. tab-set-code:: 

..     .. code-block:: c++

..         auto engine = new PDJE("database/path");

..     .. code-block:: c#

..         PDJE engine = new PDJE("database/path");

..     .. code-block:: python

..         import pdje_POLYGLOT as pypdje
..         engine = pypdje.PDJE("database/path")

..     .. code-block:: gdscript

..         var engine:PDJE_Wrapper = PDJE_Wrapper.new()
..         engine.InitEngine("res://database/path")

.. _get-edit-logs:

Get edit logs
---------------


.. tab-set-code:: 

    .. code-block:: c++

        /*
        JSON structure produced:

        {
        "BRANCH": [                          // branch head list
            {
            "NAME": string,                  // branch name (e.g., "main")
            "OID":  string                   // head commit oid (40-hex from git_oid_tostr_s)
            },
            ...
        ],
        "COMMIT": [                          // commit metadata list
            {
            "OID":      string,              // commit oid (40-hex)
            "EMAIL":    string,              // author email
            "NAME":     string,              // author name
            "PARENTID": string               // parent commit oid (may be empty/zero for initial)
            },
            ...
        ]
        }
        */
        std::string mix_json_graph = editor->GetLogWithJSONGraph<EDIT_ARG_MIX>();
        std::string key_value_json_graph = editor->GetLogWithJSONGraph<EDIT_ARG_KEY_VALUE>();
        std::string note_json_graph = editor->GetLogWithJSONGraph<EDIT_ARG_NOTE>();
        std::string music_json_graph = editor->GetLogWithJSONGraph<EDIT_ARG_MUSIC>("music name");

    .. code-block:: c#

        /*
        JSON structure produced:

        {
        "BRANCH": [                          // branch head list
            {
            "NAME": string,                  // branch name (e.g., "main")
            "OID":  string                   // head commit oid (40-hex from git_oid_tostr_s)
            },
            ...
        ],
        "COMMIT": [                          // commit metadata list
            {
            "OID":      string,              // commit oid (40-hex)
            "EMAIL":    string,              // author email
            "NAME":     string,              // author name
            "PARENTID": string               // parent commit oid (may be empty/zero for initial)
            },
            ...
        ]
        }
        */
        string KV_json_graph = editor.GetLogKVJSON();
        string Mix_json_graph = editor.GetLogMixJSON();
        string Music_json_graph = editor.GetLogMusicJSON("music name");
        string Note_json_graph = editor.GetLogNoteJSON();

    .. code-block:: python

        """
        JSON structure produced:

        {
        "BRANCH": [                          // branch head list
            {
            "NAME": string,                  // branch name (e.g., "main")
            "OID":  string                   // head commit oid (40-hex from git_oid_tostr_s)
            },
            ...
        ],
        "COMMIT": [                          // commit metadata list
            {
            "OID":      string,              // commit oid (40-hex)
            "EMAIL":    string,              // author email
            "NAME":     string,              // author name
            "PARENTID": string               // parent commit oid (may be empty/zero for initial)
            },
            ...
        ]
        }
        """
        KV_json_graph = editor.GetLogKVJSON()
        Mix_json_graph = editor.GetLogMixJSON()
        Music_json_graph = editor.GetLogMusicJSON("music name")
        Note_json_graph = editor.GetLogNoteJSON()

    .. code-block:: gdscript

        """
        JSON structure produced:

        {
        "BRANCH": [                          // branch head list
            {
            "NAME": string,                  // branch name (e.g., "main")
            "OID":  string                   // head commit oid (40-hex from git_oid_tostr_s)
            },
            ...
        ],
        "COMMIT": [                          // commit metadata list
            {
            "OID":      string,              // commit oid (40-hex)
            "EMAIL":    string,              // author email
            "NAME":     string,              // author name
            "PARENTID": string               // parent commit oid (may be empty/zero for initial)
            },
            ...
        ]
        }
        """
        editor.GetLogWithJSONGraph(EditorWrapper.MIX, "")

Update edit logs
-------------------

.. tab-set-code:: 

    .. code-block:: c++

        bool updateRes = false;
        updateRes = editor->UpdateLog<EDIT_ARG_MIX>();
        updateRes = editor->UpdateLog<EDIT_ARG_MIX>("branch name");
        // updateRes = editor->UpdateLog<EDIT_ARG_KEY_VALUE>();
        // updateRes = editor->UpdateLog<EDIT_ARG_KEY_VALUE>("branch name");
        // updateRes = editor->UpdateLog<EDIT_ARG_MUSIC>();
        // updateRes = editor->UpdateLog<EDIT_ARG_MUSIC>("music name");
        // updateRes = editor->UpdateLog<EDIT_ARG_NOTE>();
        // updateRes = editor->UpdateLog<EDIT_ARG_NOTE>("branch name");
        

    .. code-block:: c#

        bool updateRes = false;
        updateRes = editor.UpdateLogMix();
        updateRes = editor.UpdateLogMixOn("branch name");
        // updateRes = editor.UpdateLogKV();
        // updateRes = editor.UpdateLogKVOn("branch name");
        // updateRes = editor.UpdateLogMusic();
        // updateRes = editor.UpdateLogMusicOn("music name");
        // updateRes = editor.UpdateLogNote();
        // updateRes = editor.UpdateLogNoteOn("branch name");

    .. code-block:: python

        import pdje_POLYGLOT as pypdje
        
        updateRes = editor.UpdateLogMix();
        updateRes = editor.UpdateLogMixOn("branch name");
        # updateRes = editor.UpdateLogKV();
        # updateRes = editor.UpdateLogKVOn("branch name");
        # updateRes = editor.UpdateLogMusic();
        # updateRes = editor.UpdateLogMusicOn("music name");
        # updateRes = editor.UpdateLogNote();
        # updateRes = editor.UpdateLogNoteOn("branch name");

    .. code-block:: gdscript

        editor.UpdateLog(EditorWrapper.MIX,"")
        editor.UpdateLog(EditorWrapper.MUSIC,"music title")
        editor.UpdateLog(EditorWrapper.NOTE,"")
        editor.UpdateLog(EditorWrapper.KV,"")


Editor Step-3: Config new Music & Render & apply to root db
-------------------------------------------------------------

.. doxygenfunction:: editorObject::ConfigNewMusic
    :project: Project_DJ_Engine


.. doxygenfunction:: editorObject::render
    :project: Project_DJ_Engine

.. doxygenfunction:: editorObject::pushToRootDB(litedb &ROOTDB, const UNSANITIZED &trackTitleToPush)
    :project: Project_DJ_Engine


.. doxygenfunction:: editorObject::pushToRootDB(litedb &ROOTDB, const UNSANITIZED &musicTitle, const UNSANITIZED &musicComposer)
    :project: Project_DJ_Engine


.. tab-set-code:: 

    .. code-block:: c++

        editor->ConfigNewMusic("music name", "composer name", "C://path.wav", "4800");
        //configure new music meta data. the 4800 means the music's first beat is on (4800 / 48000)second after begin of the music.
        editor->render("track name", *(engine->DBROOT));
        //renders track. builds track and music binary data from editor data.
        //if you want to add just musics, you don't need to edit or add track data. doesn't need to care about track name
        //track or music data must contain bpm data on 0beat, 0subBeat position.

        editor->pushToRootDB(*engine->DBROOT, "track title");
        //push rendered binary track data to Root DB.
        //all related musics will be pushed too.
        
        editor->pushToRootDB(*engine->DBROOT, "music name", "composer name");
        //just push rendered binary music data to Root DB.
        
        //you should render before pushing music or track data.

    .. code-block:: c#

        editor.ConfigNewMusic("music name", "composer name", "C://path.wav", "4800");
        //configure new music meta data. the 4800 means the music's first beat is on (4800 / 48000)second after begin of the music.
        editor.render("track name", (engine.DBROOT));
        //renders track. builds track and music binary data from editor data.
        //if you want to add just musics, you don't need to edit or add track data. doesn't need to care about track name
        //track or music data must contain bpm data on 0beat, 0subBeat position.

        editor.pushToRootDB(engine.DBROOT, "track title");
        //push rendered binary track data to Root DB.
        //all related musics will be pushed too.
        
        editor.pushToRootDB(engine.DBROOT, "music name", "composer name");
        //just push rendered binary music data to Root DB.
        
        //you should render before pushing music or track data.

    .. code-block:: python

        import pdje_POLYGLOT as pypdje
        editor.ConfigNewMusic("music name", "composer name", "C://path.wav", "4800")
        #configure new music meta data. the 4800 means the music's first beat is on (4800 / 48000)second after begin of the music.
        editor.render("track name", (engine.DBROOT))
        #renders track. builds track and music binary data from editor data.
        #if you want to add just musics, you don't need to edit or add track data. doesn't need to care about track name
        #track or music data must contain bpm data on 0beat, 0subBeat position.

        editor.pushToRootDB(engine.DBROOT, "track title")
        #push rendered binary track data to Root DB.
        #all related musics will be pushed too.
        
        editor.pushToRootDB(engine.DBROOT, "music name", "composer name")
        #just push rendered binary music data to Root DB.
        
        #you should render before pushing music or track data.

    .. code-block:: gdscript

        editor.ConfigNewMusic("music name", "composer name", "C://path.wav", "4800");
        #configure new music meta data. the 4800 means the music's first beat is on (4800 / 48000)second after begin of the music.
        editor.render("track name");
        #renders track. builds track and music binary data from editor data.
        #if you want to add just musics, you don't need to edit or add track data. doesn't need to care about track name
        #track or music data must contain bpm data on 0beat, 0subBeat position.

        editor.pushTrackToRootDB("track title")
        #push rendered binary track data to Root DB.
        #all related musics will be pushed too.
        
        editor.pushToRootDB("music name","composer name")
        #just push rendered binary music data to Root DB.
        
        #you should render before pushing music or track data.

Editor Step-4: playback editing project
-------------------------------------------------------------

.. doxygenfunction:: editorObject::demoPlayInit
    :project: Project_DJ_Engine


.. tab-set-code:: 

    .. code-block:: c++

        std::shared_ptr<audioPlayer> demoplayer;
        editor->demoPlayInit(demoplayer, 48, "track title");
        demoplayer->Activate();

    .. code-block:: c#

        audioPlayer demoplayer = new audioPlayer(48);
        editor.demoPlayInit(demoplayer, 48, "track title");
        demoplayer.Activate();

    .. code-block:: python

        import pdje_POLYGLOT as pypdje
        from pdje_POLYGLOT import audioPlayer
        demoplayer:audioPlayer = audioPlayer(48)
        editor.demoPlayInit(demoplayer, 48, "track title")
        demoplayer.Activate()

    .. code-block:: gdscript

        editor.demoPlayInit(48, "track title")
        var demoplayer = engine.GetPlayer()
        demoplayer.Activate()