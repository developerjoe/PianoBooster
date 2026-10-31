#include "Bar.h"

#include <iostream>

// Bar.cpp only needs the MIDI file's current PPQN and debug logger. Keeping
// these definitions local makes this a focused CBar regression test.
int CMidiFile::m_ppqn = DEFAULT_PPQN;

void ppLogDebug(const char *, ...)
{
}

bool reachesEndBar(CBar &bar)
{
    const qint64 barTicks = bar.getBarLength() * SPEED_ADJUST_FACTOR;

    bar.addDeltaTime(barTicks - 1);
    if ((bar.readEventBits() & EVENT_BITS_endBarReached) != 0)
        return false;

    bar.addDeltaTime(1);
    return (bar.readEventBits() & EVENT_BITS_endBarReached) != 0;
}

int main()
{
    CBar bar;
    bar.setTimeSig(4, 4);
    bar.setPlayFromBar(0.0);
    bar.setEndBar(1.0);

    if (!reachesEndBar(bar))
    {
        std::cerr << "End Bar was not reached when Start Bar was zero\n";
        return 1;
    }

    bar.rewind();
    if (!reachesEndBar(bar))
    {
        std::cerr << "End Bar was not reached after looping back to zero\n";
        return 1;
    }

    return 0;
}
