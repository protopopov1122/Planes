#ifndef FOOTBALL_FOOTBALL_CANVAS_H_
#define FOOTBALL_FOOTBALL_CANVAS_H_

#include "football/base.h"
#include "football/ProjectorAnimation.h"
#include <wx/bitmap.h>
#include <wx/timer.h>
#include <wx/sound.h>
#include <wx/thread.h>
#include <wx/dcbuffer.h>

namespace Football {

  wxDECLARE_EVENT(FOOTBALL_STATUS_CHANGE, wxCommandEvent);
  wxDECLARE_EVENT(FOOTBALL_SCORE_CHANGE, wxCommandEvent);
  wxDECLARE_EVENT(FOOTBALL_ROUND, wxCommandEvent);

  enum class FootballStatus {
	  Playing,
	  Paused,
	  Projector,
	  StartWhistle
  };
  
  class AudioThread : public wxThread {
	public:
		AudioThread();
		bool play(wxSound &);
		bool isPlaying();
		void exit();
	protected:
		void *Entry() override;
	private:
		wxSound *sound;
		bool work;
		wxMutex mutex;
		wxCondition cond;
  };

  class FootballCanvas : public wxWindow {
    public:
      FootballCanvas(wxWindow *, wxWindowID, int8_t, std::string &);
	  
	  FootballStatus getStatus();
	  void setStatus(FootballStatus);
	  
	  void loadSounds(std::string, std::string);
	  void enableSounds(bool);
	  void enableProjectors(bool);
	  float getFPS();
    private:
	  void updateFPS(float);
		void OnClose(wxCloseEvent &);
		void OnPaintEvent(wxPaintEvent &);
		
		void render(wxBufferedPaintDC &);
	  void draw(wxDC &);

	  FootballStatus status;
      wxTimer *timer;
      wxBitmap background;
	  ProjectorAnimation projector;
      clock_t lastClock;
	  
	  bool projectors;
	  bool playSound;
	  wxSound goalSound;
	  wxSound startSound;
	  
	  float fpsSum;
	  uint8_t fpsCount;
	  AudioThread *soundThread;
  };
}

#endif