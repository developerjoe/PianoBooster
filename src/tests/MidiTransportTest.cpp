#include "MidiTransport.h"

#include <cassert>
#include <initializer_list>
#include <vector>

static std::vector<unsigned char> message(std::initializer_list<unsigned char> bytes)
{
    return std::vector<unsigned char>(bytes);
}

int main()
{
    // Mackie Control Universal button-down messages.
    assert(decodeMidiTransport(message({0x90, 0x5e, 0x7f})) == MidiTransportCommand::Play);
    assert(decodeMidiTransport(message({0x90, 0x5d, 0x7f})) == MidiTransportCommand::Stop);
    assert(decodeMidiTransport(message({0x90, 0x5b, 0x7f})) == MidiTransportCommand::Rewind);
    assert(decodeMidiTransport(message({0x90, 0x5e, 0x00})) == MidiTransportCommand::None);

    // Arturia KeyLab Essential Mk3 DAW-program controller messages.
    assert(decodeMidiTransport(message({0xb0, 0x15, 0x7f})) == MidiTransportCommand::Play);
    assert(decodeMidiTransport(message({0xb0, 0x14, 0x7f})) == MidiTransportCommand::Stop);
    assert(decodeMidiTransport(message({0xb0, 0x19, 0x7f})) == MidiTransportCommand::Rewind);
    assert(decodeMidiTransport(message({0xb0, 0x15, 0x00})) == MidiTransportCommand::None);
    assert(decodeMidiTransport(message({0xb4, 0x15, 0x7f})) == MidiTransportCommand::Play);

    // MIDI Machine Control and MIDI real-time transports.
    assert(decodeMidiTransport(message({0xf0, 0x7f, 0x7f, 0x06, 0x02, 0xf7})) == MidiTransportCommand::Play);
    assert(decodeMidiTransport(message({0xf0, 0x7f, 0x7f, 0x06, 0x01, 0xf7})) == MidiTransportCommand::Stop);
    assert(decodeMidiTransport(message({0xf0, 0x7f, 0x7f, 0x06, 0x05, 0xf7})) == MidiTransportCommand::Rewind);
    assert(decodeMidiTransport(message({0xfa})) == MidiTransportCommand::Play);
    assert(decodeMidiTransport(message({0xfc})) == MidiTransportCommand::Stop);

    // Musical notes and unrelated MCU buttons remain ordinary MIDI input.
    assert(decodeMidiTransport(message({0x90, 0x3c, 0x64})) == MidiTransportCommand::None);
    assert(decodeMidiTransport(message({0x90, 0x5f, 0x7f})) == MidiTransportCommand::None);
    assert(decodeMidiTransport(message({0xb0, 0x16, 0x7f})) == MidiTransportCommand::None);
    assert(decodeMidiTransport(message({})) == MidiTransportCommand::None);
    return 0;
}
