#include "EFFECTS.hpp"

FaustEffects::FaustEffects(int initSampleRate)
{
    compressor.managingClass.init(initSampleRate);
    distortion.managingClass.init(initSampleRate);
    echo.managingClass.init(initSampleRate);
    eq.managingClass.init(initSampleRate);
    filter.managingClass.init(initSampleRate);
    flanger.managingClass.init(initSampleRate);
    ocsFilter.managingClass.init(initSampleRate);
    panner.managingClass.init(initSampleRate);
    phaser.managingClass.init(initSampleRate);
    roll.managingClass.init(initSampleRate);
    trance.managingClass.init(initSampleRate);
    vol.managingClass.init(initSampleRate);
    robot.managingClass.init(initSampleRate);
}

void
FaustEffects::consumeAll()
{
    eq.consume(eqData);
    robot.consume(robotData);
    distortion.consume(distortionData);
    echo.consume(echoData);
    filter.consume(filterData);
    flanger.consume(flangerData);
    ocsFilter.consume(ocsFilterData);
    panner.consume(pannerData);
    phaser.consume(phaserData);
    trance.consume(tranceData);
    roll.consume(rollData);
    vol.consume(volData);
    compressor.consume(compressorData);
}