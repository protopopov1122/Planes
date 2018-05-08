#include "football/Drawable.h"
#include "football/FootballApp.h"
#include "football/FootballFrame.h"

namespace Football {
	
	Drawable::Drawable(wxBitmap &base, wxSize &visualSize) : size(visualSize), rotation(0.0f) {
		wxImage img = base.ConvertToImage();
		img.Rescale(visualSize.GetX(), visualSize.GetY());
		this->currentSize = visualSize;
		this->base = wxBitmap(img);
		this->updateBitmap();
	}

	Drawable::Drawable(const Drawable &drawable) : size(drawable.size), rotation(0.0f) {
		wxBitmap base(drawable.base);
		wxImage img = base.ConvertToImage();
		img.Rescale(size.GetX(), size.GetY());
		this->currentSize = size;
		this->base = wxBitmap(img);
		this->updateBitmap();
	}
	
	wxSize &Drawable::getSize() {
		wxSize canvasSize = Drawable::getCanvasSize();
		wxSize currentSize = wxSize(this->size.GetWidth() * canvasSize.GetWidth() / Drawable::baseScreenSize.GetX(),
			this->size.GetHeight() * canvasSize.GetHeight() / Drawable::baseScreenSize.GetY());
		if (this->currentSize != currentSize) {
			this->currentSize = currentSize;
			this->updateBitmap();
		}
		return this->currentSize;
	}
	
	float Drawable::getRotation() const {
		return this->rotation;
	}
	
	void Drawable::setRotation(float rot) {
		this->rotation = rot;
		while (this->rotation >= 2 * M_PI) {
		  this->rotation -= 2 * M_PI;
		}
		while (this->rotation <= -2 * M_PI) {
		  this->rotation += 2 * M_PI;
		}
		this->updateBitmap();
	}
	
	wxBitmap &Drawable::getBitmap() {
		return this->current;
	}
	
	void Drawable::updateBitmap() {
		wxImage img = this->base.ConvertToImage();
		img.Rotate(this->rotation, wxPoint(img.GetWidth() / 2, img.GetHeight() / 2));
		img.Rescale(this->currentSize.GetX(), this->currentSize.GetY());
		wxBitmap bitmap(img);
		this->current = bitmap;
	}
	
	
	wxSize Drawable::baseScreenSize = wxSize(1920, 1080);
	
	wxSize Drawable::getCanvasSize() {
		return wxGetApp().getFrame()->getCanvas()->GetSize();
	}

	void Drawable::setBaseScreenSize(wxSize sz) {
		Drawable::baseScreenSize = sz;
	}
}