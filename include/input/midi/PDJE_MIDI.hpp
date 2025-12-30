#pragma once

#include <libremidi/libremidi.hpp>
#include "PDJE_Buffer.hpp"
#include <iostream>//debugiostream

namespace PDJE_MIDI{

struct PDJE_API PDJE_Input_MIDI_Log {
    
};

class MIDI{
    private:
    libremidi::observer obs;


    public:
    void ConfigInputFeatures();
    void Config();

    MIDI(){
        libremidi::input_configuration inputCB{
            
            .on_message = [&](const libremidi::message& m){
                
                auto ch = m.get_channel();

                if (m.get_message_type() == libremidi::message_type::NOTE_ON){
                    const auto& b = m.bytes;
                    uint8_t note = b[1];
                    uint8_t vel = b[2];
                }
                m.get_meta_event_type();
                m.timestamp;
                //use msg
            }
        };//set input logic
        libremidi::midi_in midiin{inputCB, libremidi::midi1::in_default_configuration() };//ready configures
        auto ins = obs.get_input_ports();//start getting data
        for(auto& p : ins){//print all connected devices.
            std::cout << "In: " << p.port_name << " , " << p.display_name << std::endl;
        }
        midiin.open_port(ins[0]);//add input midi device.
        midiin.open_virtual_port("Dummy port");//add virtual midi device.

    }
    ~MIDI();
};
};