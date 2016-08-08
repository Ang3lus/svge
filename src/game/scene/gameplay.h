#pragma once

#include <vector>
#include <memory>
#include <bullet/btBulletDynamicsCommon.h>
#include "scene.h"
#include "game/voxel.h"
#include "game/player.h"

namespace svge {
namespace game {
namespace scene {

class Gameplay : public Scene {
 public:
  void load() override;
  void enter() override;
  void exit() override;
  void update(const core::time::Delta& dt) override;
  void render(const core::time::Delta& dt, Video& video) override;
 private:
  Voxels voxels_;
  Player player_;

  class Physics{
   public:
    Physics();
    Physics(const Physics&) = delete;
    Physics& operator=(const Physics&) = delete;
    ~Physics();

   void step(const core::time::Delta& dt);
   void add(const Voxel& voxel);
   glm::vec3 position(Voxels::size_type idx) const;
   void set_player(const Player& player);
   glm::vec3 player_position() const;
   void set_player_linear_velocity(const glm::vec3& vel);
  private:
    btDbvtBroadphase broadphase_;
    btDefaultCollisionConfiguration collision_configuration_;
    btCollisionDispatcher dispatcher_{&collision_configuration_};
    btSequentialImpulseConstraintSolver solver_;
    btDiscreteDynamicsWorld dynamics_world_{&dispatcher_, &broadphase_, &solver_, &collision_configuration_};

    btBoxShape voxel_shape_{btVector3(0.5, 0.5, 0.5)};

    btCompoundShape voxel_compound_shape_{true, 16777216};
    std::unique_ptr<btRigidBody> voxel_compound_rigid_body_;

    btCapsuleShape player_shape_{0, 0};
    std::unique_ptr<btRigidBody> player_body_;

  } physics_;
};

} // namespace scene
} // namespace game
} // namespace svge
