/*********************************************************************************/
/*
@file           MidiTransport.h

@brief          Decode standard MIDI and MCU transport messages.
*/
/*********************************************************************************/

#ifndef __MIDI_TRANSPORT_H__
#define __MIDI_TRANSPORT_H__

#include <vector>

enum class MidiTransportCommand
{
    None,
    Play,
    Stop,
    Rewind
};

inline MidiTransportCommand decodeMidiTransport(const std::vector<unsigned char> &message)
{
    if (message.empty())
        return MidiTransportCommand::None;

    // MIDI real-time transport messages.
    if (message[0] == 0xfa || message[0] == 0xfb)
        return MidiTransportCommand::Play;
    if (message[0] == 0xfc)
        return MidiTransportCommand::Stop;

    // MIDI Machine Control: F0 7F <device> 06 <command> F7.
    if (message.size() >= 6 && message[0] == 0xf0 && message[1] == 0x7f
        && message[3] == 0x06 && message.back() == 0xf7)
    {
        switch (message[4])
        {
        case 0x01: return MidiTransportCommand::Stop;
        case 0x02: // Play
        case 0x03: return MidiTransportCommand::Play; // Deferred Play
        case 0x05: return MidiTransportCommand::Rewind;
        default: return MidiTransportCommand::None;
        }
    }

    // Mackie Control Universal transport buttons are note messages on channel 1.
    // Only act on button-down; the matching zero-velocity/button-up event is ignored.
    if (message.size() >= 3 && message[0] == 0x90 && message[2] != 0)
    {
        switch (message[1])
        {
        case 0x5b: return MidiTransportCommand::Rewind;
        case 0x5d: return MidiTransportCommand::Stop;
        case 0x5e: return MidiTransportCommand::Play;
        default: return MidiTransportCommand::None;
        }
    }

    // Arturia KeyLab Essential Mk3 transport buttons in its DAW program.
    // The buttons send controller value 127 while pressed and 0 on release.
    if (message.size() >= 3 && (message[0] & 0xf0) == 0xb0 && message[2] != 0)
    {
        switch (message[1])
        {
        case 0x14: return MidiTransportCommand::Stop;
        case 0x15: return MidiTransportCommand::Play;
        case 0x19: return MidiTransportCommand::Rewind;
        default: return MidiTransportCommand::None;
        }
    }

    return MidiTransportCommand::None;
}

#endif // __MIDI_TRANSPORT_H__
