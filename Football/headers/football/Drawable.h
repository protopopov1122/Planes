#ifndef FOOTBALL_DRAWABLE_H_
#define FOOTBALL_DRAWABLE_H_

#include "football/base.h"

namespace Football {

	class Drawable {
		public:
			Drawable (wxBitmap &, wxSize &);
			Drawable(const Drawable &);
			
			wxSize &getSize();
			float getRotation() const;
			void setRotation(float);
			wxBitmap &getBitmap();
			
			static wxSize getCanvasSize();
			static void setBaseScreenSize(wxSize);
		private:
			static wxSize baseScreenSize;
			void updateBitmap();
			
			wxSize size;
			wxSize currentSize;
			wxBitmap base;
			wxBitmap current;
			float rotation;
	};
}

#endif