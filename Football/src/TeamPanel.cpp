#include "football/TeamPanel.h"
#include <vector>
#include <iostream>

namespace Football {

	class PlayerVectorCollector : public PlayerVisitor {
		public:
			PlayerVectorCollector(std::vector<Player *> &vec) : vec(vec) {}
			void visit(Player &pl) {
				this->vec.push_back(&pl);
			}
		private:
			std::vector<Player *> &vec;
	};
	
	TeamPanel::TeamPanel(wxWindow *win, wxWindowID id, Team &team)
		: wxPanel::wxPanel(win, id), team(team) {
			
		wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
		this->SetSizer(sizer);
		this->update();
	}
	
	void TeamPanel::update() {
		this->GetSizer()->Clear(true);
		std::vector<Player *> vec;
		PlayerVectorCollector collector(vec);
		this->team.visit(collector, false);
		std::size_t index = 1;
		for (Player *pl : vec) {
			wxCheckBox *cbox = new wxCheckBox(this, wxID_ANY, std::to_string(index++));
			cbox->SetValue(pl->isEnabled());
			this->GetSizer()->Add(cbox);
			cbox->Bind(wxEVT_CHECKBOX, [=](wxCommandEvent &evt) {
				pl->enable(cbox->GetValue());
			});
		}
		this->energySpinner = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, 0, 1000, static_cast<int>(this->team.getParameters().getEnergy() * 1000));
		this->GetSizer()->Add(new wxStaticText(this, wxID_ANY, "Energy"));
		this->energySpinner->Bind(wxEVT_SPINCTRL, &TeamPanel::OnEnergyChange, this);
		this->energySpinner->Bind(wxEVT_TEXT, &TeamPanel::OnManualEnergyChange, this);
		this->GetSizer()->Add(this->energySpinner);
		Layout();
	}
	
	void TeamPanel::OnEnergyChange(wxSpinEvent &evt) {
		float energy = this->energySpinner->GetValue() / 1000.0f;
		this->team.getParameters().setEnergy(energy);
	}
	
	void TeamPanel::OnManualEnergyChange(wxCommandEvent &evt) {
		float energy = this->energySpinner->GetValue() / 1000.0f;
		this->team.getParameters().setEnergy(energy);
	}
}