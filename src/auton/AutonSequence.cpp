#include "auton/AutonSequence.h"

AutonSequence::AutonSequence(std::vector<AutonStep> steps)
    :steps(std::move(steps)) {}

void AutonSequence::start(uint32_t nowMs) {
   started = true;
   index = 0;
   stepStart = nowMs;
   cancelled = false;
    if (!steps.empty() &&steps[0].onStart)steps[0].onStart();
}

void AutonSequence::update(uint32_t nowMs) {
    if (!started ||cancelled ||index >=steps.size()) return;

    auto &step =steps[index];
    if (step.action) step.action();

    bool done = false;
    if (step.isComplete && step.isComplete()) done = true;
    if (!done && step.timeoutMs > 0 && (nowMs -stepStart) >= step.timeoutMs) done = true;

    if (done) {
        if (step.onEnd) step.onEnd(false);
        ++index;
        if (index <steps.size()) {
           stepStart = nowMs;
            if (steps[index].onStart)steps[index].onStart();
        }
    }
}

bool AutonSequence::isFinished() const {
    return cancelled || (started &&index >=steps.size());
}

void AutonSequence::cancel() {
   cancelled = true;
    if (index <steps.size() &&steps[index].onEnd) {
       steps[index].onEnd(true);
    }
}
