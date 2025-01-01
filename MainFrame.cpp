#include "MainFrame.h"
#include <wx/persist/toplevel.h>
#include <wx/display.h>

// Initialize default inventory items
void MainFrame::InitializeItems() {
    m_items.push_back(Item("Sword", "A sharp blade for close combat", 5, "path/to/sword.png"));
    m_items.push_back(Item("Shield", "Protective gear to block attacks", 3, "path/to/shield.png"));
    m_items.push_back(Item("Potion", "Restores 50 HP when consumed", 10, "path/to/potion.png"));
    m_items.push_back(Item("Bow", "Long-range weapon for precise attacks", 2, "path/to/bow.png"));
    m_items.push_back(Item("Arrow", "Ammunition for bow. Deals 25 damage", 50, "path/to/arrow.png"));
}

// Main constructor for the application window
MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    InitializeItems();

    // Set window size based on screen dimensions
    wxDisplay display;
    wxRect screenSize = display.GetGeometry();
    int width = screenSize.GetWidth() * 0.6;
    int height = screenSize.GetHeight() * 0.6;
    SetSize(width, height);
    SetMinSize(wxSize(800, 500));

    // Create unique ID for "Manage Categories" menu item
    const int ID_MANAGE_CATEGORIES = wxID_HIGHEST + 1;

    // Create and setup menu bar
    wxMenuBar* menuBar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(wxID_NEW, "&New");
    fileMenu->Append(wxID_OPEN, "&Load");
    fileMenu->Append(wxID_SAVE, "&Save");
    fileMenu->Append(ID_MANAGE_CATEGORIES, "Manage &Categories");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "&Exit");



    wxMenu* helpMenu = new wxMenu();
    helpMenu->Append(wxID_ABOUT, "&About");

    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");
    SetMenuBar(menuBar);

    // Setup main layout
    wxPanel* mainPanel = new wxPanel(this);
    wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

    // Setup left panel with item list
    wxPanel* leftPanel = new wxPanel(mainPanel);
    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
    m_listBox = new wxListBox(leftPanel, wxID_ANY);
    m_listBox->SetMinSize(wxSize(300, -1));
    m_listBox->SetMaxSize(wxSize(300, -1));

    // Setup search bar
    wxBoxSizer* searchSizer = new wxBoxSizer(wxHORIZONTAL);
    m_searchBox = new wxTextCtrl(leftPanel, wxID_ANY, "",
        wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_filterButton = new wxButton(leftPanel, wxID_ANY, "Filters");

    searchSizer->Add(m_searchBox, 1, wxEXPAND | wxRIGHT, 5);
    searchSizer->Add(m_filterButton, 0);

    leftSizer->Insert(0, searchSizer, 0, wxEXPAND | wxALL, 5);

    // Populate list box with items
    for (const auto& item : m_items) {
        m_listBox->Append(item.getName());
    }

    leftSizer->Add(m_listBox, 1, wxEXPAND | wxALL, 5);
    leftPanel->SetSizer(leftSizer);

    // Setup right panel with item details
    wxPanel* rightPanel = new wxPanel(mainPanel);
    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

    // Setup item title display
    m_itemTitle = new wxStaticText(rightPanel, wxID_ANY, "Select an item");
    wxFont titleFont = m_itemTitle->GetFont();
    titleFont.SetPointSize(20);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    m_itemTitle->SetFont(titleFont);
    rightSizer->Add(m_itemTitle, 0, wxALIGN_CENTER | wxALL, 5);

    // Setup item count display
    m_itemCount = new wxStaticText(rightPanel, wxID_ANY, "Quantity: 0");
    rightSizer->Add(m_itemCount, 0, wxALIGN_CENTER | wxALL, 5);

    //
    m_itemCategory = new wxStaticText(rightPanel, wxID_ANY, "Category: [None]");
    rightSizer->Add(m_itemCategory, 0, wxALIGN_CENTER | wxALL, 5);

    rightSizer->AddStretchSpacer();

    // Setup description panel
    wxSize desSize = wxSize(800, 400);
    wxBoxSizer* descriptionContainerSizer = new wxBoxSizer(wxHORIZONTAL);
    wxPanel* descriptionPanel = new wxPanel(rightPanel);
    wxBoxSizer* descriptionSizer = new wxBoxSizer(wxVERTICAL);

    m_itemDescription = new wxTextCtrl(descriptionPanel, wxID_ANY, "Item description will appear here",
        wxDefaultPosition, desSize, wxTE_MULTILINE | wxTE_CENTER);
    m_itemDescription->SetMaxSize(desSize);

    descriptionSizer->Add(m_itemDescription, 1, wxEXPAND | wxALL, 5);
    descriptionPanel->SetSizer(descriptionSizer);

    // Center the description panel
    descriptionContainerSizer->AddStretchSpacer(1);
    descriptionContainerSizer->Add(descriptionPanel, 0, wxEXPAND | wxALL, 5);
    descriptionContainerSizer->AddStretchSpacer(1);

    rightSizer->Add(descriptionContainerSizer, 1, wxEXPAND);

    // Setup button controls
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    m_incrementBtn = new wxButton(rightPanel, wxID_ANY, "Add");
    m_decrementBtn = new wxButton(rightPanel, wxID_ANY, "Remove");
    m_setAmountBtn = new wxButton(rightPanel, wxID_ANY, "Set Amount");
    m_setNameBtn = new wxButton(rightPanel, wxID_ANY, "Set Name");
    m_setImageBtn = new wxButton(rightPanel, wxID_ANY, "Set Image");

    buttonSizer->Add(m_incrementBtn, 0, wxALL, 5);
    buttonSizer->Add(m_decrementBtn, 0, wxALL, 5);
    buttonSizer->Add(m_setAmountBtn, 0, wxALL, 5);
    buttonSizer->Add(m_setNameBtn, 0, wxALL, 5);
    buttonSizer->Add(m_setImageBtn, 0, wxALL, 5);

    rightSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    rightPanel->SetSizer(rightSizer);

    // Finalize main layout
    horizontalSizer->Add(leftPanel, 0, wxEXPAND | wxALL, 5);
    horizontalSizer->Add(rightPanel, 1, wxEXPAND | wxALL, 5);
    mainPanel->SetSizer(horizontalSizer);

    // Bind event handlers
    m_incrementBtn->Bind(wxEVT_BUTTON, &MainFrame::OnIncrement, this);
    m_decrementBtn->Bind(wxEVT_BUTTON, &MainFrame::OnDecrement, this);
    m_setAmountBtn->Bind(wxEVT_BUTTON, &MainFrame::OnSetAmount, this);
    m_setNameBtn->Bind(wxEVT_BUTTON, &MainFrame::OnSetName, this);
    m_setImageBtn->Bind(wxEVT_BUTTON, &MainFrame::OnSetImage, this);
    m_searchBox->Bind(wxEVT_TEXT, &MainFrame::OnSearchInput, this);
    m_filterButton->Bind(wxEVT_BUTTON, &MainFrame::OnFilterButton, this);

    // Bind menu events
    fileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSave, this, wxID_SAVE);
    fileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnLoad, this, wxID_OPEN);
    fileMenu->Bind(wxEVT_MENU, &MainFrame::OnManageCategories, this, ID_MANAGE_CATEGORIES);
    fileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnExit, this, wxID_EXIT);
    helpMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAbout, this, wxID_ABOUT);

    // Bind description update event
    m_itemDescription->Bind(wxEVT_KILL_FOCUS, [this](wxFocusEvent& evt) {
        int selection = m_listBox->GetSelection();
        if (selection != wxNOT_FOUND) {
            m_items[selection].setDescription(m_itemDescription->GetValue().ToStdString());
        }
        evt.Skip();
        });

    m_listBox->Bind(wxEVT_LISTBOX, &MainFrame::OnListBoxSelect, this);

    // Setup window persistence
    SetName("MainFrame");
    wxPersistentRegisterAndRestore(this, "MainFrame");
}

// Event handler for item selection
void MainFrame::OnListBoxSelect(wxCommandEvent& evt) {
    int selection = m_listBox->GetSelection();
    if (selection != wxNOT_FOUND) {
        const Item& selectedItem = m_items[selection];
        m_itemTitle->SetLabel(selectedItem.getName());
        m_itemDescription->SetLabel(selectedItem.getDescription());
        m_itemCount->SetLabel(wxString::Format("Quantity: %d", selectedItem.getAmount()));
    }
}

// Event handlers for item manipulation
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
// Event handler for search
void MainFrame::OnSearchInput(wxCommandEvent& evt) {
    UpdateItemList();
}

void MainFrame::OnFilterButton(wxCommandEvent& evt) {
    FilterDialog dialog(this, m_minQuantity, m_maxQuantity, m_selectedCategory, m_categories);
    if (dialog.ShowModal() == wxID_OK) {
        UpdateItemList();
    }
}

void MainFrame::UpdateItemList() {
    wxString searchTerm = m_searchBox->GetValue().Lower();
    m_listBox->Clear();

    for (const auto& item : m_items) {
        // Check if item matches all filters
        bool matchesSearch = wxString::FromUTF8(item.getName()).Lower().Contains(searchTerm);
        bool matchesQuantity = item.getAmount() >= m_minQuantity &&
            item.getAmount() <= m_maxQuantity;
        bool matchesCategory = m_selectedCategory.empty() ||
            item.getCategory() == m_selectedCategory;

        if (matchesSearch && matchesQuantity && matchesCategory) {
            m_listBox->Append(item.getName());
        }
    }
}


// Menu bar event handlers
void MainFrame::OnSave(wxCommandEvent& evt) {
    Save::SaveToCfg(m_items, m_categories, "save/inventory.cfg");
}

void MainFrame::OnLoad(wxCommandEvent& evt) {
    Save::LoadFromCfg(m_items, m_categories, "save/inventory.cfg");
    UpdateItemList();

    // Update category display if an item is selected
    int selection = m_listBox->GetSelection();
    if (selection != wxNOT_FOUND) {
        m_itemCategory->SetLabel("Category: " + m_items[selection].getCategory());
    }
}


void MainFrame::OnManageCategories(wxCommandEvent& evt) {
    CategoryDialog dialog(this, m_categories);
    if (dialog.ShowModal() == wxID_OK) {
        // Update categories for all items if needed
        int selection = m_listBox->GetSelection();
        if (selection != wxNOT_FOUND) {
            m_itemCategory->SetLabel("Category: " + m_items[selection].getCategory());
        }
    }
}

void MainFrame::OnExit(wxCommandEvent& evt) {
    int answer = wxMessageBox("Would you like to save before exiting?",
        "Save Changes?",
        wxYES_NO | wxCANCEL | wxICON_QUESTION);

    if (answer == wxYES) {
        OnSave(evt);
        Close();
    }
    else if (answer == wxNO) {
        Close();
    }
}

void MainFrame::OnAbout(wxCommandEvent& evt) {
    wxMessageBox("GUI Iventory Manager\n\nClick OK to visit our website",
        "About", wxOK | wxICON_INFORMATION);
    wxLaunchDefaultBrowser("github.com/Code-JL/GUI-Inventory-Manager");
}
