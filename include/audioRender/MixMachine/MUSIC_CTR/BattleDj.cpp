#include "BattleDj.hpp"
#include "PDJE_LOG_SETTER.hpp"

BattleDj::BattleDj()
{
}

BattleDj::~BattleDj()
{
}

bool
BattleDj::GetDataFrom(MUSIC_CTR &mc)
{
    if (mc.SendData(st, D)) {
        return true;
    } else {
        critlog(
            "failed to get data from soundtouch. From BattleDJ GetDataFrom.");
        return false;
    }
}

bool
BattleDj::Spin(MixStruct &ms)
{
    DJ_JOBS job;
    job.attachIn      = ms.frame_in;
    job.attachOut     = ms.frame_out;
    job.STT           = SoundTouchType::RAW;
    job.getFromOrigin = true;
    try {
        job.SPEED = std::stof(ms.RP.getFirst().cStr());
    } catch (std::exception &e) {
        critlog("failed to convert string to float. From BattleDJ Spin. ErrStr "
                "& exceptionlog: ");
        critlog(ms.RP.getFirst().cStr());
        critlog(e.what());
        return false;
    }
    if (job.SPEED == 0.0) {
        warnlog("ignore effect because speed is zero. From BattleDj Spin.");
        return false;
    }

    job.sourcePoint = job.attachIn;

    jobs.push_back(job);
    return true;
}

bool
BattleDj::Rev(MixStruct &ms)
{
    DJ_JOBS j;
    j.attachIn      = ms.frame_in;
    j.attachOut     = ms.frame_out;
    j.STT           = SoundTouchType::MASTER;
    j.getFromOrigin = false;
    try {
        j.SPEED = std::stof(ms.RP.getFirst().cStr());
    } catch (std::exception &e) {
        critlog("failed to convert string to float. From BattleDJ Rev. ErrStr "
                "& exceptionlog: ");
        critlog(ms.RP.getFirst().cStr());
        critlog(e.what());
        return false;
    }
    if (j.SPEED == 0.0) {
        warnlog("ignore effect because speed is zero. From BattleDj Rev.");
        return false;
    }
    j.SPEED       = j.SPEED < 0 ? j.SPEED : (-1.0 * j.SPEED);
    j.sourcePoint = j.attachIn;
    jobs.push_back(j);
    return true;
}

bool
BattleDj::Scratch(MixStruct &ms)
{
    DJ_JOBS j;
    j.attachIn      = ms.frame_in;
    j.attachOut     = ms.frame_out;
    j.STT           = SoundTouchType::RAW;
    j.getFromOrigin = true;

    try {
        j.sourcePoint = std::stoul(ms.RP.getFirst().cStr());
        j.SPEED       = std::stof(ms.RP.getSecond().cStr());
    } catch (std::exception &e) {
        critlog("failed to convert string to long. From BattleDJ Scratch. two "
                "ErrStr & exceptionlog: ");
        critlog(ms.RP.getFirst().cStr());
        critlog(ms.RP.getSecond().cStr());
        critlog(e.what());
        return false;
    }
    if (j.SPEED == 0.0) {
        warnlog("ignore effect because speed is zero. From BattleDj Scratch.");
        return false;
    }
    jobs.push_back(j);
    return true;
}

bool
BattleDj::Pitch(MixStruct &ms)
{
    DJ_JOBS j;
    j.attachIn      = ms.frame_in;
    j.attachOut     = ms.frame_out;
    j.STT           = SoundTouchType::PITCH;
    j.getFromOrigin = false;
    j.sourcePoint   = j.attachIn;
    try {
        j.SPEED = abs(std::stof(ms.RP.getFirst().cStr()));
    } catch (std::exception &e) {
        critlog("failed to convert string to float. From BattleDJ Pitch. "
                "ErrStr & exceptionlog: ");
        critlog(ms.RP.getFirst().cStr());
        critlog(e.what());
        return false;
    }
    jobs.push_back(j);
    return true;
}

std::optional<SIMD_FLOAT *>
BattleDj::operator<<(std::optional<SIMD_FLOAT *> Array)
{
    st->setTempo(1.0);
    st->setPitch(1.0);
    st->setRate(1.0);
    if (jobs.empty() || (!Array.has_value()) || (!StartPos.has_value())) {
        infolog("battledj jobs empty from BattleDJ op<<. this maybe safe.");
        return Array;
    }
    for (auto i : jobs) {
        unsigned long Range    = i.attachOut > i.attachIn
                                     ? i.attachOut - i.attachIn
                                     : i.attachIn - i.attachOut;
        unsigned long SPDRange = Range * abs(i.SPEED);

        switch (i.STT) {
        case SoundTouchType::MASTER:
            st->setTempo(abs(i.SPEED));
            st->setRate(1.0);
            st->setPitch(1.0);
            break;
        case SoundTouchType::RAW:
            st->setRate(abs(i.SPEED));
            st->setTempo(1.0);
            st->setPitch(1.0);
            break;
        case SoundTouchType::PITCH:
            st->setPitch(abs(i.SPEED));
            st->setTempo(1.0);
            st->setRate(1.0);
            break;
        default:
            break;
        }

        std::vector<float> Buf(SPDRange * CHANNEL);
        if (i.getFromOrigin) {
            if (i.SPEED > 0) {
                if (!D->changePos(i.sourcePoint - StartPos.value_or(0))) {
                    continue;
                }
            } else {
                if (!D->changePos((i.sourcePoint - StartPos.value_or(0)) -
                                  SPDRange)) {
                    continue;
                }
            }
            if (!D->getRange(SPDRange, Buf)) {
                continue;
            }
        } else {
            auto CopyStartItr = Array.value()->data();
            if (i.SPEED > 0) {
                CopyStartItr += (i.sourcePoint - StartPos.value());
            } else {
                CopyStartItr +=
                    ((i.sourcePoint - StartPos.value_or(0)) - SPDRange);
            }
            memcpy(Buf.data(), CopyStartItr, Buf.size() * sizeof(float));
        }
        if (i.SPEED < 0) {
            std::reverse(Buf.begin(), Buf.end());
        }
        st->clear();
        st->putSamples(Buf.data(), SPDRange);
        Range                = st->receiveSamples(Buf.data(), Range);
        unsigned long sPoint = i.attachIn - StartPos.value_or(0);

        sPoint *= CHANNEL;
        float *AP = Array.value()->data();
        AP += sPoint;
        memcpy(AP, Buf.data(), (Range * CHANNEL * sizeof(float)));
    }
    return Array;
}