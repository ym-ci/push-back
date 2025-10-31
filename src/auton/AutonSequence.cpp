#include "auton/AutonSequence.h"

AutonSequence::AutonSequence(std::vector<AutonStep> steps)
    : m_steps(std::move(steps)) {}

void AutonSequence::start(uint32_t nowMs) {
    m_started = true;
    m_index = 0;
    m_stepStart = nowMs;
    m_cancelled = false;
    if (!m_steps.empty() && m_steps[0].onStart) m_steps[0].onStart();
}

void AutonSequence::update(uint32_t nowMs) {
    if (!m_started || m_cancelled || m_index >= m_steps.size()) return;

    auto &step = m_steps[m_index];
    if (step.action) step.action();

    bool done = false;
    if (step.isComplete && step.isComplete()) done = true;
    if (!done && step.timeoutMs > 0 && (nowMs - m_stepStart) >= step.timeoutMs) done = true;

    if (done) {
        if (step.onEnd) step.onEnd(false);
        ++m_index;
        if (m_index < m_steps.size()) {
            m_stepStart = nowMs;
            if (m_steps[m_index].onStart) m_steps[m_index].onStart();
        }
    }
}

bool AutonSequence::isFinished() const {
    return m_cancelled || (m_started && m_index >= m_steps.size());
}

void AutonSequence::cancel() {
    m_cancelled = true;
    if (m_index < m_steps.size() && m_steps[m_index].onEnd) {
        m_steps[m_index].onEnd(true);
    }
}
