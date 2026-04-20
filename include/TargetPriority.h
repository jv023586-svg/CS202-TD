#pragma once

/// How Capsule (and Turret, via shared target) picks among enemies in range.
enum class TargetPriority {
    Random, ///< Pick uniformly among valid targets (chaotic demo).
    First   ///< Lowest spawn index first (ordered demo; TargetQueue can back this later).
};
