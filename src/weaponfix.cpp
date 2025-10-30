// v2.4 — Strafing-shoot offset fix (ZGloom-Vita)
// Default stubs: wire these to your game's real spawn functions.
#include "weaponfix.h"

namespace WeaponFixV24 {

void SpawnBullet(const FixVec3& /*pos*/, const FixVec3& /*vel*/) {
    // TODO: hook into your game's projectile spawn (e.g., GameLogic::SpawnBullet or World::AddProjectile)
    // Example:
    // GameLogic::Get().SpawnBullet(pos, vel, /*damage=*/...);
}

void SpawnMuzzleFlash(const FixVec3& /*pos*/, float /*yaw*/) {
    // TODO: hook into your muzzle flash system (sprite/particle/light at pos facing yaw)
}

} // namespace WeaponFixV24
