#include "Structs.hpp"

namespace FrameCalc{
    unsigned long CountFrame(
        unsigned long Sbar,
        unsigned long Sbeat,
        unsigned long Sseparate,
        unsigned long Ebar,
        unsigned long Ebeat,
        unsigned long Eseparate,
        double bpm
    )
    {
        Sseparate = Sseparate > 0 ? Sseparate : 1;
        Eseparate = Eseparate > 0 ? Eseparate : 1;
        bpm = bpm > 0 ? bpm : 1;
        auto Sapprx = APPRX(double, Sbar, Sbeat, Sseparate);
        auto Eapprx = APPRX(double, Ebar, Ebeat, Eseparate);
        return static_cast<unsigned long>(
            std::round(
                (Eapprx - Sapprx) * (DMINUTE / bpm) * DSAMPLERATE
            )
        );
    }
}