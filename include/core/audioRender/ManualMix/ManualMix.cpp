#include "ManualMix.hpp"

FXControlPanel::FXControlPanel(int sampleRate)
{
    compressorPanel.first = false;
    distortionPanel.first = false;
    echoPanel.first       = false;
    eqPanel.first         = false;
    filterPanel.first     = false;
    flangerPanel.first    = false;
    ocsFilterPanel.first  = false;
    pannerPanel.first     = false;
    phaserPanel.first     = false;
    robotPanel.first      = false;
    rollPanel.first       = false;
    trancePanel.first     = false;
    volPanel.first        = false;

    compressorPanel.second.init(sampleRate);
    distortionPanel.second.init(sampleRate);
    echoPanel.second.init(sampleRate);
    eqPanel.second.init(sampleRate);
    filterPanel.second.init(sampleRate);
    flangerPanel.second.init(sampleRate);
    ocsFilterPanel.second.init(sampleRate);
    pannerPanel.second.init(sampleRate);
    phaserPanel.second.init(sampleRate);
    robotPanel.second.init(sampleRate);
    rollPanel.second.init(sampleRate);
    trancePanel.second.init(sampleRate);
    volPanel.second.init(sampleRate);
}

void
FXControlPanel::FX_ON_OFF(FXList fx, bool onoff)
{
    switch (fx) {

    case FXList::COMPRESSOR:
        compressorPanel.first = onoff;
        break;

    case FXList::DISTORTION:
        distortionPanel.first = onoff;
        break;

    case FXList::ECHO:
        echoPanel.first = onoff;
        break;

    case FXList::EQ:
        eqPanel.first = onoff;
        break;

    case FXList::FILTER:
        filterPanel.first = onoff;
        break;

    case FXList::FLANGER:
        flangerPanel.first = onoff;
        break;

    case FXList::OCSFILTER:
        ocsFilterPanel.first = onoff;
        break;

    case FXList::PANNER:
        pannerPanel.first = onoff;
        break;

    case FXList::PHASER:
        phaserPanel.first = onoff;
        break;

    case FXList::ROBOT:
        robotPanel.first = onoff;
        break;

    case FXList::ROLL:
        rollPanel.first = onoff;
        break;

    case FXList::TRANCE:
        trancePanel.first = onoff;
        break;

    case FXList::VOL:
        volPanel.first = onoff;
        break;

    default:
        break;
    }
}

ARGSETTER
FXControlPanel::GetArgSetter(FXList fx)
{
    switch (fx) {
    case FXList::COMPRESSOR:
        return compressorPanel.second.makeArgSetter();
        break;

    case FXList::DISTORTION:
        return distortionPanel.second.makeArgSetter();
        break;

    case FXList::ECHO:
        return echoPanel.second.makeArgSetter();
        break;

    case FXList::EQ:
        return eqPanel.second.makeArgSetter();
        break;

    case FXList::FILTER:
        return filterPanel.second.makeArgSetter();
        break;

    case FXList::FLANGER:
        return flangerPanel.second.makeArgSetter();
        break;

    case FXList::OCSFILTER:
        return ocsFilterPanel.second.makeArgSetter();
        break;

    case FXList::PANNER:
        return pannerPanel.second.makeArgSetter();
        break;

    case FXList::PHASER:
        return phaserPanel.second.makeArgSetter();
        break;

    case FXList::ROBOT:
        return robotPanel.second.makeArgSetter();
        break;

    case FXList::ROLL:
        return rollPanel.second.makeArgSetter();
        break;

    case FXList::TRANCE:
        return trancePanel.second.makeArgSetter();
        break;

    case FXList::VOL:
        return volPanel.second.makeArgSetter();
        break;

    default:
        return ARGSETTER();
        break;
    }
}

void
FXControlPanel::addFX(float **pcm, int samples)
{
    checkAndUse(pcm, samples, compressorPanel);
    checkAndUse(pcm, samples, distortionPanel);
    checkAndUse(pcm, samples, echoPanel);
    checkAndUse(pcm, samples, eqPanel);
    checkAndUse(pcm, samples, filterPanel);
    checkAndUse(pcm, samples, flangerPanel);
    checkAndUse(pcm, samples, ocsFilterPanel);
    checkAndUse(pcm, samples, pannerPanel);
    checkAndUse(pcm, samples, phaserPanel);
    checkAndUse(pcm, samples, robotPanel);
    checkAndUse(pcm, samples, rollPanel);
    checkAndUse(pcm, samples, trancePanel);
    checkAndUse(pcm, samples, volPanel);
}

bool
FXControlPanel::checkSomethingOn()
{
    return compressorPanel.first || distortionPanel.first || echoPanel.first ||
           eqPanel.first || filterPanel.first || flangerPanel.first ||
           ocsFilterPanel.first || pannerPanel.first || phaserPanel.first ||
           robotPanel.first || rollPanel.first || trancePanel.first ||
           volPanel.first;
}