#pragma once

namespace command {
class Subsystem {
public:
    virtual ~Subsystem() = default;
    // Called periodically by the main loop or scheduler
    virtual void periodic() {}
};
} // namespace command
