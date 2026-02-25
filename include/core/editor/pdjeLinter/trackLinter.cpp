#include "CapnpBinary.hpp"
#include "MixBinary.capnp.h"
#include "TrackLinterRules.hpp"
#include "fileNameSanitizer.hpp"
#include "pdjeLinter.hpp"
#include <cstdint>
#include <string>

template <>
bool
PDJE_Linter<trackdata>::Lint(const trackdata &target, UNSANITIZED &lint_msg)
{
    auto trackReader = CapReader<MixBinaryCapnpData>();
    if (!trackReader.open(target.mixBinary)) {
        lint_msg = "failed to open mix data.";
        return false;
    }
    auto td = trackReader.Rp->getDatas();

    PDJE_TRACK_LINTER_RULES::TrackLinterIdCount id_has_load;
    PDJE_TRACK_LINTER_RULES::TrackLinterIdCount id_has_unload;
    for (size_t i = 0; i < td.size(); ++i) {
        PDJE_TRACK_LINTER_RULES::AccumulateIf(id_has_load,
                                              td[i].getId(),
                                              td[i].getType() == TypeEnum::LOAD);
        PDJE_TRACK_LINTER_RULES::AccumulateIf(
            id_has_unload,
            td[i].getId(),
            td[i].getType() == TypeEnum::UNLOAD);
    }
    bool FLAG_RESULT = true;
    if (!PDJE_TRACK_LINTER_RULES::ValidateExactlyOne(
            id_has_load, "load", lint_msg)) {
        FLAG_RESULT = false;
    }
    if (!PDJE_TRACK_LINTER_RULES::ValidateExactlyOne(
            id_has_unload, "unload", lint_msg)) {
        FLAG_RESULT = false;
    }

    return FLAG_RESULT;
}
