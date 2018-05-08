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

#include <sstream>
#include <iostream>
#include <wx/splitter.h>
#include "planes/ReportDialog.h"
#include "planes/PlanesApp.h"

namespace Planes {

  PlanesReportDialog::PlanesReportDialog(wxWindow *win, wxWindowID id)
    : wxDialog::wxDialog(win, id, "Report", wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER | wxDEFAULT_DIALOG_STYLE | wxMAXIMIZE_BOX) {

    wxGetApp().getDatabase().getSessions(this->sessions);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(sizer);

    wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE);
    sizer->Add(splitter, 1, wxEXPAND | wxALL, 5);

    this->sessionList = new wxListBox(splitter, wxID_ANY);
    for (const auto &sess : this->sessions) {
      this->sessionList->Append(ctime(&sess.opened));
    }

    planePanel = new wxPanel(splitter);
    wxBoxSizer *planeSizer = new wxBoxSizer(wxVERTICAL);
    planePanel->SetSizer(planeSizer);
    this->sessionDescr = new wxStaticText(planePanel, wxID_ANY, "");
    planeSizer->Add(this->sessionDescr);
    this->planeList = new wxDataViewListCtrl(planePanel, wxID_ANY);
    planeSizer->Add(this->planeList, 1, wxEXPAND);
    this->planeList->AppendTextColumn("ID");
    this->planeList->AppendTextColumn("Name");
    this->planeList->AppendTextColumn("Model");
    this->planeList->AppendTextColumn("Created");
    this->planeList->AppendTextColumn("Destroyed");
	
	this->planeList->GetColumn(3)->SetWidth(300);
	this->planeList->GetColumn(4)->SetWidth(300);

    splitter->Initialize(this->sessionList);
    splitter->SplitVertically(this->sessionList, planePanel);
    splitter->SetSashGravity(0.3f);


    wxButton *okButton = new wxButton(this, wxID_ANY, "OK");
    sizer->Add(okButton, 0, wxALL | wxALIGN_CENTER, 5);
    sessionList->Bind(wxEVT_LISTBOX, &PlanesReportDialog::OnSessionClick, this);
    okButton->Bind(wxEVT_BUTTON, &PlanesReportDialog::OnOkClick, this);
	
	this->planeList->Refresh();
	this->planeList->Update();
  }

  void PlanesReportDialog::OnSessionClick(wxCommandEvent &evt) {
    this->planeList->DeleteAllItems();
    this->sessionDescr->SetLabel("");
    if (this->sessionList->GetSelection() != wxNOT_FOUND) {
      SessionDTO &sess = this->sessions.at(this->sessionList->GetSelection());
      std::vector<PlaneDTO> planes;
      wxGetApp().getDatabase().getPlanes(sess.id, planes);
      size_t i = 1;
      for (const PlaneDTO &plane : planes) {
  			wxVector<wxVariant> data;
				data.push_back(wxVariant(std::to_string(i++)));
				data.push_back(wxVariant(plane.name));
				data.push_back(wxVariant(plane.model));
				data.push_back(wxVariant(ctime(&plane.created)));
				data.push_back(plane.destroyed > 0 ? wxVariant(ctime(&plane.destroyed)) : wxVariant(""));
        this->planeList->AppendItem(data);
      }

      std::stringstream ss;
      ss << "Opened: " << ctime(&sess.opened) << std::endl;
      if (sess.closed > 0) {
        ss << "Closed: " << ctime(&sess.closed) << std::endl;
      }
      ss << planes.size() << " planes" << std::endl;
      this->sessionDescr->SetLabel(ss.str());
    }
	this->planeList->Refresh();
	this->planeList->Update();
    planePanel->Layout();
	planePanel->Refresh();
  }

  void PlanesReportDialog::OnOkClick(wxCommandEvent &evt) {
    this->Close(true);
  }
}