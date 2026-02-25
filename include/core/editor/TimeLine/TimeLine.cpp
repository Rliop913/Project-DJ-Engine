#include "DiffMachine/TimeLineDiffMachine.hpp"

namespace PDJE_TIMELINE {

std::optional<TimeLineSemanticDiffResult>
BuildTimeLineSemanticDiff(git_repository    *repo,
                          const std::string &target_file,
                          const OID         &origin,
                          const OID         &compare,
                          TimeLineDiffKind   kind)
{
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
    return std::nullopt;
}

}; // namespace PDJE_TIMELINE
