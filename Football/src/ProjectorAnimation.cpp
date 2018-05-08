#include "football/ProjectorAnimation.h"
#include <iostream>
#include <wx/graphics.h>

namespace Football {

ProjectorAnimation::ProjectorAnimation(float length)
    : full_length(length), current(0.0f) {}

float ProjectorAnimation::length() const { return this->full_length; }

bool ProjectorAnimation::finished() const {
  return this->current >= this->full_length;
}

void ProjectorAnimation::start() { this->current = 0.0f; }

void ProjectorAnimation::render(wxMemoryDC &dc, wxSize &size, float millis) {
  if (this->finished()) {
    return;
  }
  wxGraphicsContext *gc = wxGraphicsContext::Create(dc);
  if (gc != nullptr) {
    this->draw(*gc, size);
    gc->Flush();
    delete gc;
  }
  this->current += millis;
}

void ProjectorAnimation::draw(wxGraphicsContext &dc, wxSize &size) {
  wxColour trcolour(255, 255, 0, 160);
  wxColour colour(255, 255, 0);
  dc.SetBrush(trcolour);
  dc.SetPen(trcolour);
  wxDouble x, y, x2;
  float position = this->current / this->full_length;
  if (position < 0.5f) {
    x = 2 * position * size.GetWidth();
    x2 = 2 * (0.5f - position) * size.GetWidth();
    y = 2 * position * size.GetHeight();
  } else {
    x = 2 * (1.0f - position) * size.GetWidth();
    x2 = 2 * (position - 0.5f) * size.GetWidth();
    y = 2 * (1.0f - position) * size.GetHeight();
  }
  wxDouble width = size.GetWidth() / 10;
  wxDouble height = size.GetHeight() / 10;
  x -= width / 2;
  x2 -= width / 2;
  y -= height / 2;
  wxGraphicsPath path = dc.CreatePath();
  path.MoveToPoint(0, 0);
  path.AddLineToPoint(x, y + 4 * height / 6);
  path.AddLineToPoint(x + 4 * width / 6, y);
  path.AddLineToPoint(0, 0);
  dc.FillPath(path);
  wxGraphicsPath path2 = dc.CreatePath();
  path2.MoveToPoint(size.GetWidth(), 0);
  path2.AddLineToPoint(x2 + width, y + 4 * height / 6);
  path2.AddLineToPoint(x2 + 2 * width / 6, y);
  path2.AddLineToPoint(size.GetWidth(), 0);
  dc.FillPath(path2);

  dc.SetBrush(colour);
  dc.DrawEllipse(x, y, width, height);
  dc.DrawEllipse(x2, y, width, height);
}
}