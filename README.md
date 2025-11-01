# push-back (PROS + LemLib)

This repository is a small template for VEX using PROS and LemLib following a WPILib-like command/subsystem architecture.

Getting started
- Install PROS (https://pros.cs.purdue.edu/) and toolchain for your OS.
- From the workspace root run the PROS build to generate compile commands:

```powershell
pros build-compile-commands --no-analytics
```

Project layout
- `include/` - public headers
- `src/` - implementation
- `include/subsystems/Drivetrain.h` - example subsystem
- `include/commands/ArcadeDriveCommand.h` - example default command
- `include/command/` - minimal command framework used by the template

Notes
- This is intentionally minimal. You can expand `command::Scheduler` to support command requirements and interrupt semantics later.
