
.. _program_listing_file_include_tests_editorTest.cpp:

Program Listing for File editorTest.cpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_tests_editorTest.cpp>` (``include/tests/editorTest.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_interface.hpp"
   #include "editor.hpp"
   
   #include "PDJE_Benchmark.hpp"
   #include <filesystem>
   #include <fstream>
   #include <iostream>
   #include <nlohmann/json.hpp>
   #include <optional>
   #include <sstream>
   #include <set>
   #include <string>
   // #include <NanoLog.hpp>
   
   namespace {
   
   using test_json = nlohmann::json;
   
   std::set<std::string>
   CollectLogOids(const std::string &logs_json)
   {
       std::set<std::string> oids;
       auto                  root = test_json::parse(logs_json);
       if (!root.contains("LOGS") || !root["LOGS"].is_array()) {
           return oids;
       }
       for (const auto &entry : root["LOGS"]) {
           if (entry.contains("OID") && entry["OID"].is_string()) {
               oids.insert(entry["OID"].get<std::string>());
           }
       }
       return oids;
   }
   
   std::optional<std::string>
   FindExactlyOneNewOid(const std::set<std::string> &before,
                        const std::set<std::string> &after)
   {
       std::optional<std::string> found;
       for (const auto &oid : after) {
           if (!before.contains(oid)) {
               if (found.has_value()) {
                   return std::nullopt;
               }
               found = oid;
           }
       }
       return found;
   }
   
   bool
   RecordNewCommitOID(auto &timeline,
                      std::set<std::string> &oid_cache,
                      std::optional<std::string> &new_oid)
   {
       timeline->UpdateLogs();
       auto next_oids = CollectLogOids(timeline->GetLogs());
       new_oid        = FindExactlyOneNewOid(oid_cache, next_oids);
       oid_cache      = std::move(next_oids);
       return new_oid.has_value();
   }
   
   std::optional<std::string>
   ReadTextFile(const std::filesystem::path &path)
   {
       std::ifstream file(path, std::ios::binary);
       if (!file.is_open()) {
           return std::nullopt;
       }
       std::ostringstream oss;
       oss << file.rdbuf();
       return oss.str();
   }
   
   std::optional<test_json>
   ParseJsonNoThrow(const std::string &text)
   {
       try {
           return test_json::parse(text);
       } catch (...) {
           return std::nullopt;
       }
   }
   
   std::optional<std::filesystem::path>
   FindFirstFileNamed(const std::filesystem::path &root, const std::string &filename)
   {
       std::error_code ec;
       if (!std::filesystem::exists(root, ec)) {
           return std::nullopt;
       }
       for (const auto &entry : std::filesystem::recursive_directory_iterator(root, ec)) {
           if (ec) {
               return std::nullopt;
           }
           if (!entry.is_regular_file(ec)) {
               continue;
           }
           if (entry.path().filename() == filename) {
               return entry.path();
           }
       }
       return std::nullopt;
   }
   
   int
   RunTimeLineDiffFailureSmoke()
   {
       namespace fs = std::filesystem;
   
       bool           ok = true;
       std::error_code ec;
       auto           root = fs::temp_directory_path() / "pdje_timeline_diff_failure_smoke";
       fs::remove_all(root, ec);
       ec.clear();
   
       std::cout << "[diff-failure-smoke] root: " << root.string() << std::endl;
       PDJE_Editor editor(root, "diff-failure-smoke", "diff-failure-smoke@test");
       if (!editor.mixHandle || !editor.KVHandle) {
           std::cout << "[diff-failure-smoke] FAIL: timeline handles not initialized"
                     << std::endl;
           return 1;
       }
   
       auto print_check = [&ok](bool cond, const std::string &label) {
           std::cout << "[diff-failure-smoke] " << (cond ? "PASS: " : "FAIL: ")
                     << label << std::endl;
           ok = ok && cond;
       };
   
       auto bad_mix = editor.mixHandle->Diff("not-an-oid", "still-not-an-oid");
       print_check(!bad_mix.has_value(), "mix diff invalid oid returns nullopt");
   
       auto bad_kv = editor.KVHandle->Diff("not-an-oid", "still-not-an-oid");
       print_check(!bad_kv.has_value(), "kv diff invalid oid returns nullopt");
   
       fs::remove_all(root, ec);
       if (!ok) {
           std::cout << "[diff-failure-smoke] RESULT: FAIL" << std::endl;
           return 2;
       }
       std::cout << "[diff-failure-smoke] RESULT: PASS" << std::endl;
       return 0;
   }
   
   int
   RunTimeLineJsonFormatSmoke()
   {
       namespace fs = std::filesystem;
   
       bool           ok = true;
       std::error_code ec;
       auto           root = fs::temp_directory_path() / "pdje_timeline_json_format_smoke";
       fs::remove_all(root, ec);
       ec.clear();
   
       std::cout << "[json-format-smoke] root: " << root.string() << std::endl;
       PDJE_Editor editor(root, "json-format-smoke", "json-format-smoke@test");
       if (!editor.mixHandle || !editor.noteHandle || !editor.KVHandle) {
           std::cout << "[json-format-smoke] FAIL: timeline handles not initialized"
                     << std::endl;
           return 1;
       }
   
       auto print_check = [&ok](bool cond, const std::string &label) {
           std::cout << "[json-format-smoke] " << (cond ? "PASS: " : "FAIL: ")
                     << label << std::endl;
           ok = ok && cond;
       };
   
       auto read_json = [&print_check](const fs::path &path, const std::string &label)
           -> std::optional<test_json> {
           auto text = ReadTextFile(path);
           print_check(text.has_value(), label + " readable");
           if (!text) {
               return std::nullopt;
           }
           auto parsed = ParseJsonNoThrow(*text);
           print_check(parsed.has_value(), label + " valid json");
           return parsed;
       };
   
       auto read_text = [&print_check](const fs::path &path, const std::string &label)
           -> std::optional<std::string> {
           auto text = ReadTextFile(path);
           print_check(text.has_value(), label + " readable");
           return text;
       };
   
       const auto mix_path  = root / "Mixes" / "mixmetadata.PDJE";
       const auto note_path = root / "Notes" / "notemetadata.PDJE";
       const auto kv_path   = root / "KeyValues" / "keyvaluemetadata.PDJE";
   
       if (auto mix_json = read_json(mix_path, "mix skeleton file")) {
           print_check(mix_json->contains(PDJEARR), "mix skeleton has array key");
           print_check((*mix_json)[PDJEARR].is_array(), "mix skeleton array type");
           print_check((*mix_json)[PDJEARR].empty(), "mix skeleton array empty");
       }
       if (auto note_json = read_json(note_path, "note skeleton file")) {
           print_check(note_json->contains(PDJENOTE), "note skeleton has array key");
           print_check((*note_json)[PDJENOTE].is_array(), "note skeleton array type");
           print_check((*note_json)[PDJENOTE].empty(), "note skeleton array empty");
       }
       if (auto kv_json = read_json(kv_path, "kv skeleton file")) {
           print_check(kv_json->is_object(), "kv skeleton object type");
           print_check(kv_json->empty(), "kv skeleton object empty");
       }
   
       print_check(editor.AddMusicConfig(
                       "json_smoke_music",
                       "json_smoke_composer",
                       "0",
                       root / "dummy.wav"),
                   "music config create");
       auto music_path = FindFirstFileNamed(root / "Musics", "musicmetadata.PDJE");
       print_check(music_path.has_value(), "music metadata file exists");
       if (music_path) {
           if (auto music_json = read_json(*music_path, "music skeleton file")) {
               print_check(music_json->contains(PDJEMUSICBPM),
                           "music skeleton has bpm array key");
               print_check((*music_json)[PDJEMUSICBPM].is_array(),
                           "music skeleton array type");
               print_check((*music_json)[PDJEMUSICBPM].empty(),
                           "music skeleton array empty");
           }
       }
   
       MixArgs mix_a;
       mix_a.type     = TypeEnum::LOAD;
       mix_a.details  = DetailEnum::HIGH;
       mix_a.ID       = 100;
       mix_a.first    = "fmt_a";
       mix_a.beat     = 1;
       mix_a.subBeat  = 0;
       mix_a.separate = 4;
       MixArgs mix_b  = mix_a;
       mix_b.ID       = 101;
       mix_b.first    = "fmt_b";
       mix_b.beat     = 2;
   
       NoteArgs note_a;
       note_a.Note_Type   = "tap";
       note_a.Note_Detail = 1;
       note_a.beat        = 1;
       note_a.subBeat     = 0;
       note_a.separate    = 4;
       note_a.railID      = 1;
   
       MusicArgs mus_a;
       mus_a.bpm      = "120";
       mus_a.beat     = 0;
       mus_a.subBeat  = 0;
       mus_a.separate = 4;
       MusicArgs mus_b = mus_a;
       mus_b.bpm       = "140";
       mus_b.beat      = 4;
   
       print_check(editor.mixHandle->WriteData(mix_a), "mix format write #1");
       print_check(editor.mixHandle->WriteData(mix_b), "mix format write #2");
       print_check(editor.noteHandle->WriteData(note_a), "note format write #1");
       print_check(editor.KVHandle->WriteData(KEY_VALUE{ "fmt_key_a", "v1" }),
                   "kv format write #1");
       print_check(editor.KVHandle->WriteData(KEY_VALUE{ "fmt_key_b", "v2" }),
                   "kv format write #2");
   
       bool music_write_ok = false;
       if (!editor.musicHandle.empty() && editor.musicHandle.back().handle) {
           music_write_ok =
               editor.musicHandle.back().handle->WriteData(mus_a) &&
               editor.musicHandle.back().handle->WriteData(mus_b);
       }
       print_check(music_write_ok, "music format writes");
   
       if (auto mix_text = read_text(mix_path, "mix format file")) {
           print_check(ParseJsonNoThrow(*mix_text).has_value(),
                       "mix format file valid json");
           print_check(mix_text->find("\n    , {") != std::string::npos,
                       "mix array uses leading comma on appended row");
           print_check(mix_text->find("\n      \"") == std::string::npos,
                       "mix array object is single-line");
       }
       if (auto note_text = read_text(note_path, "note format file")) {
           print_check(ParseJsonNoThrow(*note_text).has_value(),
                       "note format file valid json");
           print_check(note_text->find("\n    {") != std::string::npos,
                       "note array row is line-based");
           print_check(note_text->find("\n      \"") == std::string::npos,
                       "note array object is single-line");
       }
       if (auto kv_text = read_text(kv_path, "kv format file")) {
           print_check(ParseJsonNoThrow(*kv_text).has_value(),
                       "kv format file valid json");
           print_check(kv_text->find("\n  , \"") != std::string::npos,
                       "kv object uses leading comma for later fields");
       }
       if (music_path) {
           if (auto music_text = read_text(*music_path, "music format file")) {
               print_check(ParseJsonNoThrow(*music_text).has_value(),
                           "music format file valid json");
               print_check(music_text->find("\n    , {") != std::string::npos,
                           "music bpm array uses leading comma on appended row");
               print_check(music_text->find("\n      \"") == std::string::npos,
                           "music bpm array object is single-line");
           }
       }
   
       fs::remove_all(root, ec);
       if (!ok) {
           std::cout << "[json-format-smoke] RESULT: FAIL" << std::endl;
           return 2;
       }
       std::cout << "[json-format-smoke] RESULT: PASS" << std::endl;
       return 0;
   }
   
   int
   RunTimeLineDiffSmoke()
   {
       namespace fs = std::filesystem;
   
       bool           ok = true;
       std::error_code ec;
       auto           root = fs::temp_directory_path() / "pdje_timeline_diff_smoke";
       fs::remove_all(root, ec);
       ec.clear();
   
       std::cout << "[diff-smoke] root: " << root.string() << std::endl;
       PDJE_Editor editor(root, "diff-smoke", "diff-smoke@test");
       if (!editor.mixHandle || !editor.KVHandle) {
           std::cout << "[diff-smoke] FAIL: timeline handles not initialized"
                     << std::endl;
           return 1;
       }
   
       auto print_check = [&ok](bool cond, const std::string &label) {
           std::cout << "[diff-smoke] " << (cond ? "PASS: " : "FAIL: ") << label
                     << std::endl;
           ok = ok && cond;
       };
   
       // MIX add -> add diff (object recovery path)
       editor.mixHandle->UpdateLogs();
       auto mix_oids = CollectLogOids(editor.mixHandle->GetLogs());
       MixArgs mix_a;
       mix_a.type     = TypeEnum::LOAD;
       mix_a.details  = DetailEnum::HIGH;
       mix_a.ID       = 10;
       mix_a.first    = "smoke_a";
       mix_a.beat     = 1;
       mix_a.subBeat  = 0;
       mix_a.separate = 4;
   
       MixArgs mix_b = mix_a;
       mix_b.ID      = 11;
       mix_b.first   = "smoke_b";
       mix_b.beat    = 2;
   
       std::optional<std::string> mix_commit_1;
       std::optional<std::string> mix_commit_2;
       std::optional<std::string> mix_commit_3;
       print_check(editor.mixHandle->WriteData(mix_a), "mix write #1");
       print_check(RecordNewCommitOID(editor.mixHandle, mix_oids, mix_commit_1),
                   "capture mix commit #1 oid");
       print_check(editor.mixHandle->WriteData(mix_b), "mix write #2");
       print_check(RecordNewCommitOID(editor.mixHandle, mix_oids, mix_commit_2),
                   "capture mix commit #2 oid");
   
       if (mix_commit_1 && mix_commit_2) {
           auto mix_diff = editor.mixHandle->Diff(*mix_commit_1, *mix_commit_2);
           print_check(mix_diff.has_value(), "mix diff returns value");
           if (mix_diff) {
               print_check(mix_diff->mixRemoved.empty(),
                           "mix diff removed count == 0 for append");
               print_check(mix_diff->mixAdded.size() == 1,
                           "mix diff added count == 1 for append");
               if (!mix_diff->mixAdded.empty()) {
                   print_check(mix_diff->mixAdded.front().ID == mix_b.ID,
                               "mix diff added row ID matches");
               }
           }
       }
   
       print_check(editor.mixHandle->DeleteData(mix_b, false, false) == 1,
                   "mix delete #1 removes 1 row");
       print_check(RecordNewCommitOID(editor.mixHandle, mix_oids, mix_commit_3),
                   "capture mix commit #3 oid (delete)");
   
       if (mix_commit_2 && mix_commit_3) {
           auto mix_delete_diff = editor.mixHandle->Diff(*mix_commit_2, *mix_commit_3);
           print_check(mix_delete_diff.has_value(), "mix delete diff returns value");
           if (mix_delete_diff) {
               print_check(mix_delete_diff->mixRemoved.size() == 1,
                           "mix delete diff removed count == 1");
               print_check(mix_delete_diff->mixAdded.empty(),
                           "mix delete diff added count == 0");
               if (!mix_delete_diff->mixRemoved.empty()) {
                   print_check(mix_delete_diff->mixRemoved.front().ID == mix_b.ID,
                               "mix delete diff removed row ID matches");
               }
           }
       }
   
       // KV overwrite -> removed+added diff (field recovery path)
       editor.KVHandle->UpdateLogs();
       auto kv_oids = CollectLogOids(editor.KVHandle->GetLogs());
       std::optional<std::string> kv_commit_1;
       std::optional<std::string> kv_commit_2;
   
       print_check(editor.KVHandle->WriteData(KEY_VALUE{ "diff_smoke_key", "v1" }),
                   "kv write #1");
       print_check(RecordNewCommitOID(editor.KVHandle, kv_oids, kv_commit_1),
                   "capture kv commit #1 oid");
       print_check(editor.KVHandle->WriteData(KEY_VALUE{ "diff_smoke_key", "v2" }),
                   "kv write #2 (overwrite)");
       print_check(RecordNewCommitOID(editor.KVHandle, kv_oids, kv_commit_2),
                   "capture kv commit #2 oid");
   
       if (kv_commit_1 && kv_commit_2) {
           auto kv_diff = editor.KVHandle->Diff(*kv_commit_1, *kv_commit_2);
           print_check(kv_diff.has_value(), "kv diff returns value");
           if (kv_diff) {
               print_check(kv_diff->kvRemoved.size() == 1,
                           "kv diff removed count == 1 for overwrite");
               print_check(kv_diff->kvAdded.size() == 1,
                           "kv diff added count == 1 for overwrite");
               if (!kv_diff->kvRemoved.empty() && !kv_diff->kvAdded.empty()) {
                   print_check(kv_diff->kvRemoved.front().first == "diff_smoke_key",
                               "kv removed key matches");
                   print_check(kv_diff->kvAdded.front().first == "diff_smoke_key",
                               "kv added key matches");
               }
           }
       }
   
       fs::remove_all(root, ec);
       if (!ok) {
           std::cout << "[diff-smoke] RESULT: FAIL" << std::endl;
           return 2;
       }
       std::cout << "[diff-smoke] RESULT: PASS" << std::endl;
       return 0;
   }
   
   } // namespace
   
   int
   main(int argc, char **argv)
   {
       if (argc > 1 && std::string(argv[1]) == "--timeline-diff-smoke") {
           return RunTimeLineDiffSmoke();
       }
       if (argc > 1 && std::string(argv[1]) == "--timeline-diff-failure-smoke") {
           return RunTimeLineDiffFailureSmoke();
       }
       if (argc > 1 && std::string(argv[1]) == "--timeline-json-format-smoke") {
           return RunTimeLineJsonFormatSmoke();
       }
   
       std::cout << "editor tester" << std::endl;
   
       auto engine = new PDJE(std::string("testRoot.db"));
   
       if (engine->InitEditor("test", "test", "testEditorProject")) {
           std::cout << "init ok" << std::endl;
           bool Flag_Already_has_music = false;
           engine->editor->getAll<EDIT_ARG_MUSIC>(
               [&Flag_Already_has_music](const EDIT_ARG_MUSIC &margs) {
                   if (margs.musicName == "testMiku") {
                       Flag_Already_has_music = true;
                   }
               });
           if (!Flag_Already_has_music) {
               if (engine->editor->ConfigNewMusic(
                       "testMiku",
                       "Camellia",
                       "../../DMCA_FREE_DEMO_MUSIC/miku_temp.wav",
                       "41280")) {
   
                   EDIT_ARG_MUSIC temp;
                   temp.musicName    = "testMiku";
                   temp.arg.beat     = 0;
                   temp.arg.subBeat  = 0;
                   temp.arg.separate = 4;
                   temp.arg.bpm      = "138";
                   engine->editor->AddLine<EDIT_ARG_MUSIC>(temp);
   
                   EDIT_ARG_MIX bpmSet;
                   bpmSet.beat     = 0;
                   bpmSet.subBeat  = 0;
                   bpmSet.type     = TypeEnum::BPM_CONTROL;
                   bpmSet.details  = DetailEnum::TIME_STRETCH;
                   bpmSet.separate = 4;
                   bpmSet.ID       = 0;
                   bpmSet.first    = "138";
                   engine->editor->AddLine<EDIT_ARG_MIX>(bpmSet);
   
                   EDIT_ARG_MIX loadMusic;
                   loadMusic.beat    = 0;
                   loadMusic.subBeat = 0;
                   loadMusic.type    = TypeEnum::LOAD;
   
                   loadMusic.separate = 4;
                   loadMusic.first    = "testMiku";
                   loadMusic.second   = "Camellia";
                   loadMusic.third    = "138";
                   loadMusic.ID       = 0;
                   engine->editor->AddLine<EDIT_ARG_MIX>(loadMusic);
   
                   EDIT_ARG_MIX changeBpm;
                   changeBpm.beat     = 40;
                   changeBpm.subBeat  = 0;
                   changeBpm.type     = TypeEnum::BPM_CONTROL;
                   changeBpm.details  = DetailEnum::TIME_STRETCH;
                   changeBpm.separate = 4;
                   changeBpm.ID       = 0;
                   changeBpm.first    = "170";
                   engine->editor->AddLine<EDIT_ARG_MIX>(changeBpm);
   
                   EDIT_ARG_MIX unloadMusic;
                   unloadMusic.beat     = 200;
                   unloadMusic.subBeat  = 0;
                   unloadMusic.type     = TypeEnum::UNLOAD;
                   unloadMusic.ID       = 0;
                   unloadMusic.separate = 4;
                   engine->editor->AddLine<EDIT_ARG_MIX>(unloadMusic);
   
                   std::cout << "config init ok" << std::endl;
               } else {
                   std::cout << "config init failed" << std::endl;
               }
               if (engine->editor->ConfigNewMusic(
                       "ヒアソビ",
                       "Camellia",
                       "../../DMCA_FREE_DEMO_MUSIC/miku_temp.wav",
                       "41280")) {
                   EDIT_ARG_MUSIC temp;
                   temp.musicName    = "ヒアソビ";
                   temp.arg.beat     = 0;
                   temp.arg.subBeat  = 0;
                   temp.arg.separate = 4;
                   temp.arg.bpm      = "138";
                   engine->editor->AddLine<EDIT_ARG_MUSIC>(temp);
               }
               EDIT_ARG_NOTE notetemp;
               notetemp.railID = 1;
               for (int i = 0; i < 50; ++i) {
                   notetemp.beat = i;
                   engine->editor->AddLine<EDIT_ARG_NOTE>(notetemp);
               }
               engine->editor->Undo<EDIT_ARG_NOTE>();
               engine->editor->Redo<EDIT_ARG_NOTE>();
           }
           if (engine->SearchMusic("testMiku", "Camellia").empty()) {
               std::string linter_msg;
               bool        renderRes = engine->editor->render(
                   "testTrack", *(engine->DBROOT), linter_msg);
               bool pushRes = engine->editor->pushToRootDB(
                   *(engine->DBROOT), "testMiku", "Camellia");
               bool pushResSecond = engine->editor->pushToRootDB(
                   *(engine->DBROOT), "ヒアソビ", "Camellia");
               bool trackPushRes =
                   engine->editor->pushToRootDB(*(engine->DBROOT), "testTrack");
               if (pushRes)
                   std::cout << "pushRes ok" << std::endl;
               if (renderRes)
                   std::cout << "renderRes ok" << std::endl;
               if (trackPushRes)
                   std::cout << "trackPushRes ok" << std::endl;
               if (pushResSecond)
                   std::cout << "pushResSecond ok" << std::endl;
               if (pushRes && renderRes && trackPushRes && pushResSecond)
                   std::cout << "push ok" << std::endl;
               else
                   std::cout << "push failed" << std::endl;
   
               // std::shared_ptr<audioPlayer> ap;
               // engine->editor->demoPlayInit(ap, 48, "testTrack");
               // if (!ap) {
               //     std::cout << "failed to init demo player. " << std::endl;
               // }
               // if (ap->Activate()) {
               //     std::cout << "Activated demo" << std::endl;
               // }
               // getchar();
               // if (ap->Deactivate()) {
               //     std::cout << "DeActivated demo" << std::endl;
               // }
           }
           trackdata td;
           td             = engine->SearchTrack("testTrack").front();
           auto mode      = PLAY_MODE::HYBRID_RENDER;
           auto initres   = engine->InitPlayer(mode, td, 480);
           auto activeres = engine->player->Activate();
           if (mode == PLAY_MODE::FULL_PRE_RENDER) {
               getchar();
               engine->player->Deactivate();
               delete engine;
               return 0;
           }
           WBCH("FLAG GetMusicControlPanel()")
           auto musPanel = engine->player->GetMusicControlPanel();
           WBCH("FLAG SearchMusic()")
           auto muses = engine->SearchMusic("ヒアソビ", "Camellia");
           WBCH("FLAG LoadMusic()")
           musPanel->LoadMusic(*(engine->DBROOT), muses.front());
           getchar();
           WBCH("FLAG SetMusic()")
           musPanel->SetMusic("ヒアソビ", true);
   
           // musPanel->
           getchar();
           WBCH("FLAG getFXHandle()")
           auto Fxhandle = musPanel->getFXHandle("ヒアソビ");
           WBCH("FLAG FX_ON_OFF1()")
           Fxhandle->FX_ON_OFF(FXList::OCSFILTER, true);
           WBCH("FLAG FX_ON_OFF2()")
           Fxhandle->FX_ON_OFF(FXList::EQ, true);
           WBCH("FLAG GetArgSetter()")
           auto ocshandle = Fxhandle->GetArgSetter(FXList::OCSFILTER);
           WBCH("FLAG SetArgSetter1()")
           ocshandle["OCSFilterHighLowSW"](1);
           WBCH("FLAG SetArgSetter2()")
           ocshandle["RangeFreqHalf"](2500);
           WBCH("FLAG SetArgSetter3()")
           ocshandle["MiddleFreq"](5000);
           WBCH("FLAG SetArgSetter4()")
           ocshandle["Bps"](2.2333333);
           WBCH("FLAG SetArgSetter5()")
           ocshandle["OCSFilterDryWet"](0.7);
           getchar();
           WBCH("FLAG ChangeBpm()")
           musPanel->ChangeBpm("ヒアソビ", 120, 60);
           WBCH("FLAG GetArgSetter2()")
           auto eqhandle = Fxhandle->GetArgSetter(FXList::EQ);
           WBCH("FLAG SetArgSetter6()")
           eqhandle["EQHigh"](-20);
           WBCH("FLAG SetArgSetter7()")
           eqhandle["EQMid"](-20);
           WBCH("FLAG SetArgSetter8()")
           eqhandle["EQLow"](20);
   
           getchar();
           WBCH("FLAG Deactivate()")
           auto deactres = engine->player->Deactivate();
   
           // auto editor = engine->GetEditorObject();
           // editor->UpdateLog<EDIT_ARG_MIX>();
           // editor->UpdateLog<EDIT_ARG_KEY_VALUE>();
           // editor->UpdateLog<EDIT_ARG_NOTE>();
           // editor->UpdateLog<EDIT_ARG_MUSIC>();
   
           // editor->GetLogWithJSONGraph<EDIT_ARG_MIX>();
           // editor->GetLogWithJSONGraph<EDIT_ARG_KEY_VALUE>();
           // editor->GetLogWithJSONGraph<EDIT_ARG_NOTE>();
           // editor->GetLogWithJSONGraph<EDIT_ARG_MUSIC>("music name");
           // auto core_line = engine->PullOutDataLine();
           // core_line.preRenderedData;
           // core_line.maxCursor;
           // core_line.nowCursor;
           // core_line.used_frame;
   
       } else {
           std::cout << "init failed " << std::endl;
           delete engine;
           return 1;
       }
       delete engine;
       // std::cout<<engine.InitEditor("test", "test", "./testEditorProject") <<
       // std::endl; engine.editor->ConfigNewMusic("testMiku", "Camellia", "")
       return 0;
   }
