#include "gameplay.h"

#include <random>
#include <glm/gtx/quaternion.hpp>
#include "third_party/easylogging/easylogging++.h"
#include "third_party/OpenSimplexNoise.hh"
#include "game/video.h"
#include "game/input.h"

namespace svge {
namespace game {
namespace scene {

Gameplay::Physics::Physics() {
  dynamics_world_.setGravity(btVector3(0, -9.81, 0));

  btRigidBody::btRigidBodyConstructionInfo voxel_compound_rigid_body_ci(
    0,
    nullptr,
    &voxel_compound_shape_,
    btVector3(0, 0, 0));

	voxel_compound_rigid_body_ = std::make_unique<btRigidBody>(voxel_compound_rigid_body_ci);

  dynamics_world_.addRigidBody(voxel_compound_rigid_body_.get());
}

Gameplay::Physics::~Physics() {
  if (voxel_compound_rigid_body_) {
    dynamics_world_.removeRigidBody(voxel_compound_rigid_body_.get());
  }

  if (player_body_) {
    dynamics_world_.removeRigidBody(player_body_.get());
    delete player_body_->getMotionState();
  }
}
void Gameplay::Physics::step(const core::time::Delta& dt) {
  dynamics_world_.stepSimulation(dt.count());
}

void Gameplay::Physics::add(const Voxel& voxel) {
  btTransform t;
  t.setIdentity();
  t.setOrigin(btVector3(voxel.position.x, voxel.position.y, voxel.position.z));
  voxel_compound_shape_.addChildShape(t, &voxel_shape_);
}

glm::vec3 Gameplay::Physics::position(Voxels::size_type idx) const {
  return {};
}

void Gameplay::Physics::set_player(const Player& player) {
  if (player_body_) {
    dynamics_world_.removeRigidBody(player_body_.get());
    delete player_body_->getMotionState();
  }

  auto player_motion_state = std::make_unique<btDefaultMotionState>(
    btTransform(
      btQuaternion(0, 0, 0, 1),
      btVector3(player.position.x, player.position.y, player.position.z)));

  btScalar mass = 1;
  btVector3 fall_inertia(0, 0, 0);
  player_shape_ = btCapsuleShape(0.5, player.kHeight - 2 * 0.5);
  player_shape_.calculateLocalInertia(mass, fall_inertia);

  btRigidBody::btRigidBodyConstructionInfo player_rigid_body_ci(
    mass,
    player_motion_state.get(),
    &player_shape_,
    fall_inertia);

	player_body_ = std::make_unique<btRigidBody>(player_rigid_body_ci);
  player_motion_state.release();
  player_body_->setAngularFactor(btVector3(0, 0, 0));

  dynamics_world_.addRigidBody(player_body_.get());
}

glm::vec3 Gameplay::Physics::player_position() const {
  btTransform trans;
  player_body_->getMotionState()->getWorldTransform(trans);
  btVector3 origin = trans.getOrigin();
  return {origin.getX(), origin.getY(), origin.getZ()};
}

void Gameplay::Physics::set_player_linear_velocity(const glm::vec3& vel) {
  // If the player is not affected by anything then it can be deactivated so setting velocity will have no effect.
  // To avoid this we activate the player.
  player_body_->activate();
  const auto& current_vel = player_body_->getLinearVelocity();
  player_body_->setLinearVelocity(btVector3(vel.x, current_vel.getY() + vel.y, vel.z));
}

void Gameplay::load() {
  LOG(INFO);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis;
  const int kViewDistance = 64;
  const float kTerrainCaveScale = 20;
  OSN::Noise<2> noise;
  OSN::Noise<3> noise3;
  for (int x = -kViewDistance; x < kViewDistance; ++x) {
    for (int z = -kViewDistance; z < kViewDistance; ++z) {
      int y =
        static_cast<int>(
          ((noise.eval(x / kTerrainCaveScale, z / kTerrainCaveScale) + 0.3) * 0.5) * kTerrainCaveScale);
      for (int i = y; i > y - kViewDistance; --i) {
        if (i != y && noise3.eval(x / kTerrainCaveScale, i / kTerrainCaveScale, z / kTerrainCaveScale) > 0) {
          continue;
        }
        voxels_.push_back({
          glm::vec3(x, i, z),
          glm::vec4(i < 0 ? 0 : dis(gen), dis(gen), i < 0 ? dis(gen) : 0, 1)});
        physics_.add(voxels_.back());
      }

      if (0 == x && 0 == z) {
        player_.position = voxels_.back().position;
        player_.position.y += kViewDistance + (player_.kHeight / 2) * 0.5;
        physics_.set_player(player_);
      }
    }
  }

  LOG(INFO) << "Generated " << voxels_.size() << " voxels";
}

void Gameplay::enter() {
  LOG(INFO);
}

void Gameplay::exit() {
  LOG(INFO);
}

void Gameplay::update(const core::time::Delta& dt) {
  {
    const auto kMouseDeltaPos = input::mouse_position_delta();
    const float kRotationSpeed = 0.1f * dt.count();
    player_.rotation = glm::angleAxis(kRotationSpeed * kMouseDeltaPos.x, glm::vec3(0.0f, -1.0f, 0.0f))
      * player_.rotation
      * glm::angleAxis(kRotationSpeed * kMouseDeltaPos.y, glm::vec3(-1.0f, 0.0f, 0.0f));
  }


  {
    const float kPlayerSpeed = 10;
    glm::vec3 speed_forward = player_.rotation * glm::vec3(0.0f, 0.0f, -1.0f);
    speed_forward.y = 0;
    speed_forward = glm::normalize(speed_forward) * kPlayerSpeed;
    glm::vec3 speed_right = player_.rotation * glm::vec3(kPlayerSpeed, 0.0f, 0.0f);
    glm::vec3 speed_up(0.0f, 1.0f, 0.0f);
    glm::vec3 final_velocity;
    if (input::key_pressed(input::KayboardKey::kW)) {
      final_velocity += speed_forward;
    }

    if (input::key_pressed(input::KayboardKey::kS)) {
      final_velocity -= speed_forward;
    }

    if (input::key_pressed(input::KayboardKey::kA)) {
      final_velocity -= speed_right;
    }

    if (input::key_pressed(input::KayboardKey::kD)) {
      final_velocity += speed_right;
    }

    if (input::key_pressed(input::KayboardKey::kSpace)) {
      final_velocity += speed_up;
    }

    if (input::key_pressed(input::KayboardKey::kLeftCtrl)) {
      final_velocity -= speed_up;
    }

    physics_.set_player_linear_velocity(final_velocity);
  }

  physics_.step(dt);
  player_.position = physics_.player_position();
  // Move from the center of mass to the position of the eyes
  player_.position.y += (player_.kHeight / 2) * 0.5;
}

void Gameplay::render(const core::time::Delta& dt, Video& video) {
  video.render(player_, voxels_);
}

} // namespace scene
} // namespace game
} // namespace svge
