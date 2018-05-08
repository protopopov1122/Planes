#ifndef FOOTBALL_PROJECTOR_ANIMATION_H_
#define FOOTBALL_PROJECTOR_ANIMATION_H_

#include "football/base.h"

namespace Football {

class ProjectorAnimation {
public:
  ProjectorAnimation(float);
  float length() const;
  bool finished() const;
  void start();
  void render(wxMemoryDC &, wxSize &, float);

private:
  void draw(wxGraphicsContext &, wxSize &);
  const float full_length;
  float current;
};
}

#endif