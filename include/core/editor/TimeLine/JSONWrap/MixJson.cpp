#include "jsonWrapper.hpp"

template <>
template <>
int
PDJE_JSONHandler<MIX_W>::deleteLine(const MixArgs &args) = delete;

template <>
template <>
int
PDJE_JSONHandler<MIX_W>::deleteLine(const MixArgs &args,
                                    bool           skipType,
                                    bool           skipDetail)
{
    std::vector<int> targetIDX;
    try {
        for (unsigned long long i = 0; i < ROOT[PDJEARR].size(); ++i) {
            auto Target = ROOT[PDJEARR].at(i);
            if (Target[PDJE_JSON_TYPE] != args.type && !skipType)
                continue;
            if (Target[PDJE_JSON_DETAILS] != args.details && !skipDetail)
                continue;
            if (Target[PDJE_JSON_ID] != args.ID && args.ID != -1)
                continue;
            if (Target[PDJE_JSON_FIRST] != args.first && args.first != "")
                continue;
            if (Target[PDJE_JSON_SECOND] != args.second && args.second != "")
                continue;
            if (Target[PDJE_JSON_THIRD] != args.third && args.third != "")
                continue;
            if (Target[PDJE_JSON_BEAT] != args.beat && args.beat != -1)
                continue;
            if (Target[PDJE_JSON_SUBBEAT] != args.subBeat && args.subBeat != -1)
                continue;
            if (Target[PDJE_JSON_SEPARATE] != args.separate &&
                args.separate != -1)
                continue;
            if (Target[PDJE_JSON_EBEAT] != args.Ebeat && args.Ebeat != -1)
                continue;
            if (Target[PDJE_JSON_ESUBBEAT] != args.EsubBeat &&
                args.EsubBeat != -1)
                continue;
            if (Target[PDJE_JSON_ESEPARATE] != args.Eseparate &&
                args.Eseparate != -1)
                continue;
            targetIDX.push_back(i);
        }
        for (auto i : targetIDX | vs::reverse) {
            ROOT[PDJEARR].erase(i);
        }
    } catch (...) {
        return 0;
    }
    return static_cast<int>(targetIDX.size());
}

template <>
template <>
bool
PDJE_JSONHandler<MIX_W>::add(const MixArgs &args)
{
    nj tempMix = { { PDJE_JSON_TYPE, static_cast<int>(args.type) },
                   { PDJE_JSON_DETAILS, static_cast<int>(args.details) },
                   { PDJE_JSON_ID, args.ID },
                   { PDJE_JSON_FIRST, args.first },
                   { PDJE_JSON_SECOND, args.second },
                   { PDJE_JSON_THIRD, args.third },
                   { PDJE_JSON_BEAT, args.beat },
                   { PDJE_JSON_SUBBEAT, args.subBeat },
                   { PDJE_JSON_SEPARATE, args.separate },
                   { PDJE_JSON_EBEAT, args.Ebeat },
                   { PDJE_JSON_ESUBBEAT, args.EsubBeat },
                   { PDJE_JSON_ESEPARATE, args.Eseparate } };
    if (!ROOT.contains(PDJEARR)) {
        critlog("mix json root not found. from PDJE_JSONHandler<MIX_W> add.");
        return false;
    }
    ROOT[PDJEARR].push_back(tempMix);
    return true;
}

template <>
template <>
void
PDJE_JSONHandler<MIX_W>::getAll(
    std::function<void(const MixArgs &args)> jsonCallback)
{
    if (!ROOT.contains(PDJEARR)) {
        critlog(
            "mix json root not found. from PDJE_JSONHandler<MIX_W> getAll.");
        return;
    }
    for (auto &i : ROOT[PDJEARR]) {
        MixArgs tempargs{ i[PDJE_JSON_TYPE],     i[PDJE_JSON_DETAILS],
                          i[PDJE_JSON_ID],       i[PDJE_JSON_FIRST],
                          i[PDJE_JSON_SECOND],   i[PDJE_JSON_THIRD],
                          i[PDJE_JSON_BEAT],     i[PDJE_JSON_SUBBEAT],
                          i[PDJE_JSON_SEPARATE], i[PDJE_JSON_EBEAT],
                          i[PDJE_JSON_ESUBBEAT], i[PDJE_JSON_ESEPARATE] };
        jsonCallback(tempargs);
    }
}

template <>
std::unique_ptr<MIX_W>
PDJE_JSONHandler<MIX_W>::render()
{
    try {
        auto tempMixBin = std::make_unique<MIX_W>();
        tempMixBin->makeNew();
        auto rootsz = ROOT[PDJEARR].size();
        auto filler = tempMixBin->Wp->initDatas(rootsz);
        for (std::size_t i = 0; i < rootsz; ++i) {
            auto target = ROOT[PDJEARR].at(i);
            filler[i].setType(target[PDJE_JSON_TYPE]);
            filler[i].setDetails(target[PDJE_JSON_DETAILS]);
            filler[i].setId(target[PDJE_JSON_ID]);
            filler[i].setFirst(target[PDJE_JSON_FIRST].get<SANITIZED_ORNOT>());
            filler[i].setSecond(
                target[PDJE_JSON_SECOND].get<SANITIZED_ORNOT>());
            filler[i].setThird(target[PDJE_JSON_THIRD].get<SANITIZED_ORNOT>());
            filler[i].setBeat(target[PDJE_JSON_BEAT]);
            filler[i].setSubBeat(target[PDJE_JSON_SUBBEAT]);
            filler[i].setSeparate(target[PDJE_JSON_SEPARATE]);
            filler[i].setEbeat(target[PDJE_JSON_EBEAT]);
            filler[i].setEsubBeat(target[PDJE_JSON_ESUBBEAT]);
            filler[i].setEseparate(target[PDJE_JSON_ESEPARATE]);
        }

        return tempMixBin;
    } catch (std::exception &e) {
        critlog("something wrong. from PDJE_JSONHandler<MIX_W> render. "
                "ErrException: ");
        critlog(e.what());
        return nullptr;
    }
}

template <>
bool
PDJE_JSONHandler<MIX_W>::load(const fs::path &filepath)
{
    if (fs::exists(filepath)) {
        if (fs::is_regular_file(filepath)) {
            std::ifstream jfile(filepath);

            if (!jfile.is_open()) {
                critlog("cannot open mix json data file. from "
                        "PDJE_JSONHandler<MIX_W> load. path: ");
                critlog(filepath.generic_string());
                return false;
            }

            try {
                jfile >> ROOT;
            } catch (std::exception &e) {
                critlog("cannot load mix json data from file. from "
                        "PDJE_JSONHandler<MIX_W> load. ErrException: ");
                critlog(e.what());
                return false;
            }

            jfile.close();
        } else {
            critlog("json data file is not regular file. from "
                    "PDJE_JSONHandler<MIX_W> load. path: ");
            critlog(filepath.generic_string());
            return false;
        }
    } else {
        fs::create_directories(filepath.parent_path());
        std::ofstream jfile(filepath);
        if (!jfile.is_open()) {
            critlog("failed to open or make new mix json file. from "
                    "PDJE_JSONHandler<MIX_W> load. path: ");
            critlog(filepath.generic_string());
            return false;
        }
        jfile << std::setw(4) << ROOT;
        jfile.close();
    }

    if (!ROOT.contains(PDJEARR)) {
        ROOT[PDJEARR] = nj::array();
    }

    return true;
}