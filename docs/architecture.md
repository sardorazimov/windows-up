# Miransas Control Architecture

## Overview

Miransas Control is a Windows 11 optimization and privacy utility built around a modular architecture. The design keeps UI concerns separate from operational logic and uses a feature-based composition model for extensibility.

## Goals

- Provide a clean foundation for future optimization features
- Keep the codebase modular and maintainable
- Follow SOLID principles and modern C++23 practices
- Use RAII and avoid global state
- Prefer std::unique_ptr over raw pointers

## High-Level Structure

- Core: shared abstractions, result types, feature contracts, and Windows helpers
- Logger: centralized logging service
- Registry: registry access abstraction
- Services: Windows service lifecycle utilities
- ScheduledTasks: task scheduler integration surfaces
- Privacy: privacy-oriented feature modules
- Cleaner: cleanup-oriented feature modules
- Performance: performance tuning feature modules
- Diagnostics: diagnostics and health reports
- Restore: restore-point and rollback surfaces
- Security: security posture modules
- Processes: process inspection and control modules
- Packages: application/package management surfaces
- Filesystem: file system utilities and scanning surfaces
- UI: Dear ImGui presentation layer

## Design Principles

1. Interfaces over concrete implementations.
2. Each module owns its own responsibilities.
3. Feature registration happens at runtime through FeatureManager.
4. Errors are represented with Result and Error types instead of exceptions for operational flows.
5. Windows-specific behavior is isolated behind platform abstractions.

## Runtime Composition

The application boots by constructing an ImGuiLayer, registering feature modules with FeatureManager, and initializing them in order.

## Future Work

The current scaffold intentionally avoids implementing optimization logic. Future work can add concrete feature classes, service integrations, and UI panels without changing the core architecture.
