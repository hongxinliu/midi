#pragma once

#include <atomic>
#include <condition_variable>
#include <list>
#include <mutex>
#include <thread>
#include "midi/show/actors/actor.h"
#include "midi/show/actors/particle.h"

namespace midi {

class ActorParticle : public Actor {
 public:
  ActorParticle();
  ~ActorParticle();

 public:
  void initialize() override;
  void perform() override;

 protected:
  void threadProcess();

 protected:
  std::list<std::shared_ptr<Particle>> particles_;
  Vec2d gravity_;

  std::atomic<bool> enable_thread_process_;
  std::unique_ptr<std::thread> handle_thread_process_;
  std::mutex mutex_;
  std::condition_variable cv_;
  QImage bg_;
};

}  // namespace midi
