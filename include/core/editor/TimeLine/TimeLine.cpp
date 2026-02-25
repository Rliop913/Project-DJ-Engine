#include "DiffMachine/TimeLineDiffMachine.hpp"
#include "PDJE_LOG_SETTER.hpp"

#include <exception>

namespace PDJE_TIMELINE {

std::optional<TimeLineSemanticDiffResult>
BuildTimeLineSemanticDiff(git_repository    *repo,
                          const std::string &target_file,
                          const OID         &origin,
                          const OID         &compare,
                          TimeLineDiffKind   kind)
{
    try {
        switch (kind) {
        case TimeLineDiffKind::KV: {
            TimeLineDiffMachine<KV_W> machine(repo, target_file, origin, compare);
            return machine.Run();
        }
        case TimeLineDiffKind::MIX: {
            TimeLineDiffMachine<MIX_W> machine(repo, target_file, origin, compare);
            return machine.Run();
        }
        case TimeLineDiffKind::NOTE: {
            TimeLineDiffMachine<NOTE_W> machine(repo, target_file, origin, compare);
            return machine.Run();
        }
        case TimeLineDiffKind::MUSIC: {
            TimeLineDiffMachine<MUSIC_W> machine(repo, target_file, origin, compare);
            return machine.Run();
        }
        }
    } catch (const std::exception &e) {
        critlog("BuildTimeLineSemanticDiff failed. What: ");
        critlog(e.what());
        return std::nullopt;
    } catch (...) {
        critlog("BuildTimeLineSemanticDiff failed. unknown exception.");
        return std::nullopt;
    }
    return std::nullopt;
}

}; // namespace PDJE_TIMELINE
