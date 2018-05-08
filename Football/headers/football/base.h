#ifndef FOOTBALL_BASE_H_
#define FOOTBALL_BASE_H_

#include <cstdlib>
#include <cinttypes>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

	
namespace Football {
	
	struct Position {
		Position(float x, float y) : x(x), y(y) {}
		Position(const Position &pos) : x(pos.x), y(pos.y) {}
		Position &operator=(const Position pos) {
			this->x = pos.x;
			this->y = pos.y;
			return *this;
		}
		
		float x;
		float y;
	};
	
	struct Rect {
		Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
		Rect(const Rect &rect) : x(rect.x), y(rect.y), w(rect.w), h(rect.h) {}
		Rect &operator=(const Rect rect) {
			this->x = rect.x;
			this->y = rect.y;
			this->w = rect.w;
			this->h = rect.h;
			return *this;
		}
		
		float x;
		float y;
		float w;
		float h;
	};
}

#endif