#pragma once
#include <vector>
#include <functional>
#include <cstdint>
#include <cstddef>

struct AutonStep {
    std::function<void()> onStart;       // optional
    std::function<void()> action;        // called every tick while active
    std::function<bool()> isComplete;    // optional: true when step done
    std::function<void(bool)> onEnd;     // optional, param = interrupted
    uint32_t timeoutMs = 0;              // optional timeout; 0 = no timeout
};

class AutonSequence {
public:
    explicit AutonSequence(std::vector<AutonStep> steps);
    void start(uint32_t nowMs);
    void update(uint32_t nowMs); // call each tick
    bool isFinished() const;
    void cancel();

private:
    std::vector<AutonStep>steps;
    size_t index = 0;
    uint32_t stepStart = 0;
    bool started = false;
    bool cancelled = false;
};
