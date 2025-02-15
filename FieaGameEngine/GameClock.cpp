#include "pch.h"
#include "GameClock.h"

using namespace std::chrono;

namespace Fiea::GameEngine
{
    /*
    GameClock::GameClock(now_func now) : _now(now)
    {
        // if this was not overridden in the member initialization, use the provided
        //  system clock, by default (typical case)
        if (_now == nullptr) {
            _now = high_resolution_clock::now;
        }
        assert(_now != nullptr);

        _startTime = _now();
    }

    GameTime GameClock::Current() const {
        GameTime time;
        time._start = time._start = _startTime;
        time._current = _now();
        return time;
    }

    Millis GameClock::Elapsed(const GameTime& time) const
    {
        return duration_cast<std::chrono::milliseconds>(_now() - time._last).count();
    }

    void GameClock::Update(GameTime& time) const
    {
        time._last = time._current;
        time._current = _now();
    }
    */
    /*
    void GameClock::UpdateForced(GameTime& time, long long forcedTime) const
    {
        time._current = _now();
        //std::chrono::duration<long long, std::chrono::milliseconds> tempDuration(forcedTime);
        //std::chrono::high_resolution_clock::duration tempDuration = std::chrono::milliseconds(forcedTime);
        //std::chrono::duration<long long, std::chrono::milliseconds> tempDuration(forcedTime);
        std::chrono::high_resolution_clock::duration tempDuration(forcedTime * 1000000);
        //std::chrono::high_resolution_clock::time_point tempTimePoint(tempDuration);
        time._last = time._current;
        time._last -= tempDuration;
        //auto hello = time._current - tempTimePoint;
    }
    */
}