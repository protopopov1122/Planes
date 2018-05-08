#ifndef FOOTBALL_TEAM_PANEL_H_
#define FOOTBALL_TEAM_PANEL_H_

#include "football/base.h"
#include "football/Team.h"
#include <wx/spinctrl.h>

namespace Football {
	
	class TeamPanel : public wxPanel {
		public:
			TeamPanel(wxWindow *, wxWindowID, Team &);
			void update();
		private:
			void OnEnergyChange(wxSpinEvent &);
			void OnManualEnergyChange(wxCommandEvent &);
			Team &team;
			wxSpinCtrl *energySpinner;
	};
}

#endif