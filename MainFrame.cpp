#include "MainFrame.h"
#include <wx/persist/toplevel.h>
#include <wx/display.h>

void MainFrame::InitializeItems() {
    m_items.push_back(Item("Sword", "A sharp blade for close combat", 5, "path/to/sword.png"));
    m_items.push_back(Item("Shield", "Protective gear to block attacks", 3, "path/to/shield.png"));
    m_items.push_back(Item("Potion", "Restores 50 HP when consumed", 10, "path/to/potion.png"));
    m_items.push_back(Item("Bow", "Long-range weapon for precise attacks", 2, "path/to/bow.png"));
    m_items.push_back(Item("Arrow", "Ammunition for bow. Deals 25 damage", 50, "path/to/arrow.png"));
}

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    InitializeItems();

    wxDisplay display;
    wxRect screenSize = display.GetGeometry();
    int width = screenSize.GetWidth() * 0.6;
    int height = screenSize.GetHeight() * 0.6;
    SetSize(width, height);
    SetMinSize(wxSize(400, 300));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    wxPanel* leftPanel = new wxPanel(this);
    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);

    m_listBox = new wxListBox(leftPanel, wxID_ANY);

    // Populate listbox from items
    for (const auto& item : m_items) {
        m_listBox->Append(item.getName());
    }

    leftSizer->Add(m_listBox, 1, wxEXPAND | wxALL, 5);
    leftPanel->SetSizer(leftSizer);
    mainSizer->Add(leftPanel, 20, wxEXPAND);

    wxPanel* rightPanel = new wxPanel(this);
    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

    m_itemCount = new wxStaticText(rightPanel, wxID_ANY, "Quantity: 0");
    rightSizer->Add(m_itemCount, 0, wxALL, 5);

    m_itemTitle = new wxStaticText(rightPanel, wxID_ANY, "Select an item",
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL | wxST_NO_AUTORESIZE);
    wxFont titleFont = m_itemTitle->GetFont();
    titleFont.SetPointSize(14);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    m_itemTitle->SetFont(titleFont);
    rightSizer->Add(m_itemTitle, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxEXPAND, 5);

    m_itemDescription = new wxTextCtrl(rightPanel, wxID_ANY, "Item description will appear here",
        wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    rightSizer->Add(m_itemDescription, 1, wxALL | wxEXPAND, 5);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    m_incrementBtn = new wxButton(rightPanel, wxID_ANY, "Add");
    m_decrementBtn = new wxButton(rightPanel, wxID_ANY, "Remove");
    buttonSizer->Add(m_incrementBtn, 0, wxALL, 5);
    buttonSizer->Add(m_decrementBtn, 0, wxALL, 5);

    rightSizer->Add(buttonSizer, 0, wxALL, 5);

    m_setAmountBtn = new wxButton(rightPanel, wxID_ANY, "Set Amount");
    m_setNameBtn = new wxButton(rightPanel, wxID_ANY, "Set Name");
    m_setImageBtn = new wxButton(rightPanel, wxID_ANY, "Set Image");
    wxButton* saveBtn = new wxButton(rightPanel, wxID_ANY, "Save Items");
    wxButton* loadBtn = new wxButton(rightPanel, wxID_ANY, "Load Items");

    rightSizer->Add(m_setAmountBtn, 0, wxALL, 5);
    rightSizer->Add(m_setNameBtn, 0, wxALL, 5);
    rightSizer->Add(m_setImageBtn, 0, wxALL, 5);
    rightSizer->Add(saveBtn, 0, wxALL, 5);
    rightSizer->Add(loadBtn, 0, wxALL, 5);

    // Bind events
    m_incrementBtn->Bind(wxEVT_BUTTON, &MainFrame::OnIncrement, this);
    m_decrementBtn->Bind(wxEVT_BUTTON, &MainFrame::OnDecrement, this);
    m_setAmountBtn->Bind(wxEVT_BUTTON, &MainFrame::OnSetAmount, this);
    m_setNameBtn->Bind(wxEVT_BUTTON, &MainFrame::OnSetName, this);
    m_setImageBtn->Bind(wxEVT_BUTTON, &MainFrame::OnSetImage, this);
    saveBtn->Bind(wxEVT_BUTTON, &MainFrame::OnSave, this);
    loadBtn->Bind(wxEVT_BUTTON, &MainFrame::OnLoad, this);

    m_itemDescription->Bind(wxEVT_KILL_FOCUS, [this](wxFocusEvent& evt) {
        int selection = m_listBox->GetSelection();
        if (selection != wxNOT_FOUND) {
            m_items[selection].setDescription(m_itemDescription->GetValue().ToStdString());
        }
        evt.Skip();
    });

    rightPanel->SetSizer(rightSizer);
    mainSizer->Add(rightPanel, 80, wxEXPAND);

    SetSizer(mainSizer);

    m_listBox->Bind(wxEVT_LISTBOX, &MainFrame::OnListBoxSelect, this);

    SetName("MainFrame");
    wxPersistentRegisterAndRestore(this, "MainFrame");
}

void MainFrame::OnListBoxSelect(wxCommandEvent& evt) {
    int selection = m_listBox->GetSelection();
    if (selection != wxNOT_FOUND) {
        const Item& selectedItem = m_items[selection];

        m_itemTitle->SetLabel(selectedItem.getName());
        m_itemDescription->SetLabel(selectedItem.getDescription());
        m_itemCount->SetLabel(wxString::Format("Quantity: %d", selectedItem.getAmount()));
    }
}

void MainFrame::OnIncrement(wxCommandEvent& evt) {
    int selection = m_listBox->GetSelection();
    if (selection != wxNOT_FOUND) {
        m_items[selection].increment();
        m_itemCount->SetLabel(wxString::Format("Quantity: %d", m_items[selection].getAmount()));
    }
}

void MainFrame::OnDecrement(wxCommandEvent& evt) {
    int selection = m_listBox->GetSelection();
    if (selection != wxNOT_FOUND) {
        m_items[selection].decrement();
        m_itemCount->SetLabel(wxString::Format("Quantity: %d", m_items[selection].getAmount()));
    }
}

void MainFrame::OnSetAmount(wxCommandEvent& evt) {
    int selection = m_listBox->GetSelection();
    if (selection != wxNOT_FOUND) {
        wxString input = wxGetTextFromUser("Enter new amount:", "Set Amount");
        if (!input.IsEmpty()) {
            long amount;
            if (input.ToLong(&amount)) {
                m_items[selection].setAmount(amount);
                m_itemCount->SetLabel(wxString::Format("Quantity: %d", m_items[selection].getAmount()));
            }
        }
    }
}

void MainFrame::OnSetName(wxCommandEvent& evt) {
    int selection = m_listBox->GetSelection();
    if (selection != wxNOT_FOUND) {
        wxString input = wxGetTextFromUser("Enter new name:", "Set Name");
        if (!input.IsEmpty()) {
            m_items[selection].setName(input.ToStdString());
            m_listBox->SetString(selection, input);
            m_itemTitle->SetLabel(input);
        }
    }
}

void MainFrame::OnSetImage(wxCommandEvent& evt) {
    int selection = m_listBox->GetSelection();
    if (selection != wxNOT_FOUND) {
        wxFileDialog dialog(this, "Choose image file", "", "",
            "Image files (*.png;*.jpg)|*.png;*.jpg", wxFD_OPEN);
        if (dialog.ShowModal() == wxID_OK) {
            m_items[selection].setImage(dialog.GetPath().ToStdString());
        }
    }
}

void MainFrame::OnSave(wxCommandEvent& evt) {
    Save::SaveItems(m_items, "save/inventory.csv");
}

void MainFrame::OnLoad(wxCommandEvent& evt) {
    Save::LoadItems(m_items, "save/inventory.csv");
    m_listBox->Clear();
    for (const auto& item : m_items) {
        m_listBox->Append(item.getName());
    }
}
