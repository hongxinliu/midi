#pragma once

#include "midi/show/actors/actor.h"

namespace midi {

class ActorNote : public Actor {
 public:
  void perform() override;
};

}  // namespace midi
