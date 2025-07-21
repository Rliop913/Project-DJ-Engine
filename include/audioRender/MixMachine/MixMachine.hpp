#pragma once

#include <unordered_map>
#include <thread>
#include <optional>
#include <sstream>

#include <mutex>

#include "MixTranslator.hpp"
#include "dbRoot.hpp"
#include "EFFECTS.hpp"
#include "Decoder.hpp"
#include "BattleDj.hpp"

#include "PDJE_LOG_SETTER.hpp"
#include <source_location>
#define TRY(CODE)\
try\
{\
    CODE\
}\
catch(std::exception& e)\
{\
    auto now = std::source_location::current();\
    critlog(now.file_name());\
    std::string lineNumber = std::to_string(now.line());\
    critlog(lineNumber);\
    critlog(now.function_name());\
    critlog(e.what());\
    return false;\
}



/**
 * @brief interpolator options
 * 
 */
enum InterpolateType{
    LINEAR,
    COSINE,
    CUBIC,
    FLAT
};

using ID = long;
/**
 * @brief Argument parsing class for handling parameters of 8-point interpolation
 * 
 */
struct PDJE_API EightPointValues{
    float vals[8] = {0, };
    EightPointValues(const std::string& rawData){
        std::stringstream sdata(rawData);
        std::string token;
        int counter = 0;
        while(std::getline(sdata, token, ',')){
            try
            {
                vals[counter++] = std::stof(token);
                if(counter > 7){
                    break;
                }
            }
            catch(...)
            {
                break;
            }
        }
    }
};

#define FLAG_ALL_IS_OK -99

/**
 * @brief prerenderer class
 * 
 */
class PDJE_API MixMachine{
private:
    // FRAME_POS getMixSize(FRAME_POS frames);
public:
    int FLAG_SOMETHING_WRONG_ID = FLAG_ALL_IS_OK; //-99 is ok
    std::mutex renderLock;
    // std::vector<std::thread> renderPool;

    std::unordered_map<ID, std::vector<MixStruct>> Memorized;

    /**
     * @brief Sorts data by ID
     * 
     * @param binary the translated capnp data.
     * @return true 
     * @return false 
     */
    bool IDsort(const MixTranslator& binary);

    /**
     * @brief Main function — executes Prerender
     * 
     * @param db RootDB
     * @param bpms BPM object
     * @return true 
     * @return false 
     */
    bool mix(litedb& db, const BPM& bpms);
    
    std::vector<float> rendered_out;

    /**
     * @brief Implements behavior for each type.
     * 
     * @tparam TypeEnum 
     * @tparam T 
     * @param ms 
     * @param data 
     * @return true 
     * @return false 
     */
    template<TypeEnum, typename T>
    bool TypeWorks(MixStruct& ms, T& data);
    /**
     * @brief Implements behavior for each type.
     * 
     * @tparam ypeEnum 
     * @tparam T 
     * @param ms 
     * @param data 
     * @param db 
     * @return true 
     * @return false 
     */
    template<TypeEnum, typename T>
    bool TypeWorks(MixStruct& ms, T& data, litedb& db);
    /**
     * @brief Implements behavior for each type.
     * 
     * @tparam ypeEnum 
     * @tparam T 
     * @param ms 
     * @param data 
     * @param Vec 
     * @return true 
     * @return false 
     */
    template<TypeEnum, typename T>
    bool TypeWorks(MixStruct& ms, T& data, SIMD_FLOAT* Vec);
    
    /**
     * @brief initialize interpolator class
     * 
     * @tparam FXtype 
     * @param FXvec 
     * @param PCMvec 
     * @param ms 
     * @return true 
     * @return false 
     */
    template<typename FXtype>
    bool InterpolateInit(FXtype& FXvec, SIMD_FLOAT*& PCMvec, MixStruct& ms){
        FXvec.emplace_back(PCMvec, ms.frame_in, ms.frame_out);

        TRY(
            FXvec.back().selectInterpolator =
            std::stoi(ms.RP.getFirst().cStr());
        )
        if(FXvec.back().selectInterpolator == InterpolateType::FLAT){
            TRY(
                FXvec.back().vZero =
                std::stof(ms.RP.getSecond().cStr());
            )
        }
        else{
            EightPointValues EPV(ms.RP.getSecond().cStr());
            FXvec.back().v1 = EPV.vals[0];
            FXvec.back().v2 = EPV.vals[1];
            FXvec.back().v3 = EPV.vals[2];
            FXvec.back().v4 = EPV.vals[3];
            FXvec.back().v5 = EPV.vals[4];
            FXvec.back().v6 = EPV.vals[5];
            FXvec.back().v7 = EPV.vals[6];
            FXvec.back().v8 = EPV.vals[7];
        }

        FXvec.back().frames = ms.frame_out - ms.frame_in;
        FXvec.back().timerActive = 0;
        return true;
    }

    /**
     * @brief Constructs a new MixMachine object.
     *
     */
    MixMachine();
    /**
     * @brief Destroys the MixMachine object.
     *
     */
    ~MixMachine();
};

