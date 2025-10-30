// v2.4 — Strafing-shoot offset fix (ZGloom-Vita)
// Drop-in helper to spawn bullets/muzzle flash *centered* on weapon muzzle,
// independent of player strafing velocity. Include in gamelogic.cpp (or where you fire).
#pragma once
#include <cmath>

struct FixVec3 { float x,y,z; };

// Minimal interface expected from your codebase (adapt if names differ):
// - Provide player's world position (center), current camera yaw (radians), player eye height.
// - Provide spawn hooks for projectiles and muzzle flash.
// - bullet_speed: units/sec
// - kMuzzleForward: forward offset from player center to muzzle (e.g., ~24.f)
// - kMuzzleHeight:  vertical offset from floor/center to muzzle (e.g., playerEyeHeight)
namespace WeaponFixV24 {

inline void yawToBasis(float yaw, float& fx, float& fy, float& rx, float& ry) {
    // forward = (cos, sin); right = (-sin, cos)
    fx = std::cos(yaw);  fy = std::sin(yaw);
    rx = -fy;            ry =  fx;
}

// User must implement these adapters (or map them to your existing functions)
void SpawnBullet(const FixVec3& pos, const FixVec3& vel);     // IMPLEMENT
void SpawnMuzzleFlash(const FixVec3& pos, float yaw);         // IMPLEMENT

inline void FireCenteredShot(const FixVec3& playerPos, float playerEyeHeight,
                             float camYawRadians, float bullet_speed,
                             float kMuzzleForward, float kMuzzleHeight = 0.0f)
{
    float fx,fy, rx,ry;
    yawToBasis(camYawRadians, fx,fy, rx,ry);

    // Muzzle world position (no lateral shift unless you *want* a slight right-hand offset)
    const float kGunSide = 0.0f; // keep 0 to center perfectly; set e.g. +3.f if you want a tiny right-handed offset
    FixVec3 muzzle{
        playerPos.x + fx * kMuzzleForward + rx * kGunSide,
        playerPos.y + kMuzzleHeight + playerEyeHeight,
        playerPos.z + fy * kMuzzleForward + ry * kGunSide
    };

    // Shot direction purely from camera forward (decoupled from strafing velocity)
    FixVec3 vel{ fx * bullet_speed, 0.0f, fy * bullet_speed };

    SpawnBullet(muzzle, vel);
    SpawnMuzzleFlash(muzzle, camYawRadians);
}

} // namespace WeaponFixV24
