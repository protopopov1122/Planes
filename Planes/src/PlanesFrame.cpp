/*
  MIT License

  Copyright (c) 2018 Jevgēnijs Protopopovs

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include <wx/sizer.h>
#include "planes/PlanesFrame.h"
#include "planes/PlanesApp.h"
#include "planes/ReportDialog.h"

namespace Planes {
  PlanesFrame::PlanesFrame(std::string background)
    : wxFrame::wxFrame(nullptr, wxID_DEFAULT, "Planes", wxDefaultPosition, wxSize(800, 600)) {
      wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
      this->SetSizer(sizer);
	  
	  wxPanel *fullPanel = new wxPanel(this);
	  sizer->Add(fullPanel, 1, wxEXPAND | wxALL);
	  wxBoxSizer *fullSizer = new wxBoxSizer(wxHORIZONTAL);
	  fullPanel->SetSizer(fullSizer);
	  
      this->canvas = new PlanesCanvas(fullPanel, wxID_ANY, 50, background);
      fullSizer->Add(canvas, 1, wxEXPAND);

	  wxPanel *panel = new wxPanel(fullPanel);
	  fullSizer->Add(panel, 0, wxALL, 5);
      wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
      panel->SetSizer(panelSizer);
	  

      this->insertCheckbox = new wxCheckBox(panel, wxID_ANY, "Insert mode");
      panelSizer->Add(this->insertCheckbox);
      this->insertCheckbox->Bind(wxEVT_CHECKBOX, &PlanesFrame::OnPlaneInsert, this);

      this->modelsCombo = new wxComboBox(panel, wxID_ANY);
      panelSizer->Add(modelsCombo, 0, wxEXPAND);
      wxGetApp().getModelFactory().getModels(this->models);
      for (const auto model : this->models) {
        std::string modelName = model->getName();
        modelsCombo->Append(modelName);
      }
      modelsCombo->SetSelection(0);
      this->modelsCombo->Bind(wxEVT_COMBOBOX, &PlanesFrame::OnPlaneInsert, this);
      this->modelsCombo->Enable(this->insertCheckbox->IsChecked());

      this->planeName = new wxTextCtrl(panel, wxID_ANY);
	  planeName->SetHint("Name (optional)");
      panelSizer->Add(this->planeName, 0, wxEXPAND);
      this->planeName->Bind(wxEVT_TEXT, &PlanesFrame::OnPlaneName, this);

      wxButton *reportButton = new wxButton(panel, wxID_ANY, "Report");
      panelSizer->Add(reportButton, 0, wxEXPAND);
      reportButton->Bind(wxEVT_BUTTON, &PlanesFrame::OnReportClick, this);

      this->Bind(wxEVT_CLOSE_WINDOW, &PlanesFrame::OnClose, this);
      
      Maximize(true);
  }

  PlanesCanvas *PlanesFrame::getCanvas() {
    return this->canvas;
  }

  void PlanesFrame::OnPlaneInsert(wxCommandEvent &evt) {
    if (this->insertCheckbox->IsChecked() &&
      this->modelsCombo->GetSelection() != wxNOT_FOUND) {
      this->canvas->createPlaneMode(this->models.at(this->modelsCombo->GetSelection()));
      this->canvas->setNewPlaneName(this->planeName->GetValue().ToStdString());
    } else {
      this->canvas->createPlaneMode(nullptr);
    }
    this->modelsCombo->Enable(this->insertCheckbox->IsChecked());
    this->planeName->Enable(this->insertCheckbox->IsChecked());
  }

  void PlanesFrame::OnPlaneName(wxCommandEvent &evt) {
    this->canvas->setNewPlaneName(this->planeName->GetValue().ToStdString());
  }

  void PlanesFrame::OnReportClick(wxCommandEvent &evt) {
    PlanesReportDialog *dialog = new PlanesReportDialog(this, wxID_ANY);
    dialog->ShowModal();
  }

  void PlanesFrame::OnClose(wxCloseEvent &evt) {
    this->canvas->Close();
    this->Destroy();
  }
}