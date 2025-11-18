#include "MusicJsonHelper.hpp"
#include "editorObject.hpp"
#include "fileNameSanitizer.hpp"
#include "pdjeLinter.hpp"
#include "trackDB.hpp"

bool
editorObject::render(const UNSANITIZED &trackTitle,
                     litedb            &ROOTDB,
                     UNSANITIZED       &lint_msg)
{
    std::unordered_map<SANITIZED, SANITIZED> titles;
    auto td = makeTrackData(trackTitle, titles);
    if (!PDJE_Linter<trackdata>::Lint(td, lint_msg)) {
        return false;
    }

    std::vector<musdata> mds;
    for (auto &i : edit_core->musicHandle) {
        if (GetTitle(*i.handle->GetJson()) == "" ||
            !fs::exists(GetMusicABSLocation(*i.handle->GetJson()))) {
            continue;
        }
        mds.emplace_back();

        auto rendered = i.handle->GetJson()->render();

        mds.back().title = GetTitle(*i.handle->GetJson());
        auto rdout       = rendered->out();
        mds.back().bpmBinary.assign(rdout.begin(), rdout.end());
        auto tempCOMPOSER   = GetComposer(*i.handle->GetJson());
        auto tempPATH       = GetMusicABSLocation(*i.handle->GetJson());
        auto tempFIRST_BEAT = GetFirstBeat(*i.handle->GetJson());

        mds.back().composer  = (tempCOMPOSER);
        mds.back().musicPath = (tempPATH);
        mds.back().firstBeat = (tempFIRST_BEAT);
        try {
            mds.back().bpm =
                std::stod(rendered->Wp->getDatas()[0].getBpm().cStr());
        } catch (std::exception &e) {
            critlog("failed to convert bpm to double. from editorObject "
                    "render. ErrException: ");
            critlog(e.what());
            critlog("music name: ");
            critlog(mds.back().title);
            continue;
        }
        titles[mds.back().title] = "";
    }

    for (auto &i : titles) {
        if (i.second != "") {
            auto findFromRoot     = musdata();
            findFromRoot.title    = i.first;
            findFromRoot.composer = i.second;
            auto mus              = ROOTDB << findFromRoot;
            if (mus.has_value()) {
                if (mus->empty())
                    continue;
                musdata fromRoot = mus->front();
                // try {
                //     fromRoot.musicPath =
                //         fs::relative(
                //             fs::absolute(ROOTDB.getRoot().parent_path() /
                //                          fromRoot.musicPath),
                //             projectRoot)
                //             .string();
                // } catch (std::exception &e) {
                //     critlog("failed to convert relative path. from "
                //             "editorObject render. ErrException: ");
                //     critlog(e.what());
                //     critlog("music path: ");
                //     critlog(fromRoot.musicPath);
                //     critlog("project root: ");
                //     critlog(projectRoot.generic_string());
                //     return false;
                // }
                mds.push_back(fromRoot);
            }
        }
    }

    return projectLocalDB->BuildProject(td, mds);
}