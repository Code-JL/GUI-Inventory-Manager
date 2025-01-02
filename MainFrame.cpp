#include "MainFrame.h"
#include <wx/persist/toplevel.h>
#include <wx/display.h>
#include <wx/progdlg.h>
#include <wx/filename.h>

// Main constructor for the application window
MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {

    // Initialize default inventory items
    if (Save::FileDoesNotExist("save/inventory.cfg")) {
        InitializeDefaultInventory();
        Save::SaveToCfg(m_items, m_categories, "save/inventory.cfg");
    }
    else {
        Save::LoadFromCfg(m_items, m_categories, "save/inventory.cfg");
    }

    // Set window size based on screen dimensions
    wxDisplay display;
    wxRect screenSize = display.GetGeometry();
    int width = screenSize.GetWidth() * 0.6;
    int height = screenSize.GetHeight() * 0.6;
    SetSize(width, height);
    SetMinSize(wxSize(800, 500));

    // Create and setup menu bar
    wxMenuBar* menuBar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(wxID_NEW, "&New");
    fileMenu->Append(wxID_OPEN, "&Load");
    fileMenu->Append(wxID_SAVE, "&Save");
    fileMenu->Append(ID_MANAGE_CATEGORIES, "Manage &Categories");
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_EXPORT_CSV, "Export to &CSV...");
    fileMenu->Append(ID_IMPORT, "&Import...");
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
    m_setCategoryBtn = new wxButton(rightPanel, wxID_ANY, "Change Category");

    buttonSizer->Add(m_incrementBtn, 0, wxALL, 5);
    buttonSizer->Add(m_decrementBtn, 0, wxALL, 5);
    buttonSizer->Add(m_setAmountBtn, 0, wxALL, 5);
    buttonSizer->Add(m_setNameBtn, 0, wxALL, 5);
    buttonSizer->Add(m_setImageBtn, 0, wxALL, 5);
    buttonSizer->Add(m_setCategoryBtn, 0, wxALL, 5);

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
    m_setCategoryBtn->Bind(wxEVT_BUTTON, &MainFrame::OnSetCategory, this);
    m_searchBox->Bind(wxEVT_TEXT, &MainFrame::OnSearchInput, this);
    m_filterButton->Bind(wxEVT_BUTTON, &MainFrame::OnFilterButton, this);

    // Bind menu events
    fileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnNew, this, wxID_NEW);
    fileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSave, this, wxID_SAVE);
    fileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnLoad, this, wxID_OPEN);
    fileMenu->Bind(wxEVT_MENU, &MainFrame::OnManageCategories, this, ID_MANAGE_CATEGORIES);
    fileMenu->Bind(wxEVT_MENU, &MainFrame::OnExportCSV, this, ID_EXPORT_CSV);
    fileMenu->Bind(wxEVT_MENU, &MainFrame::OnImport, this, ID_IMPORT);
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
        m_itemTitle->SetLabel(m_items[selection].getName());
        m_itemDescription->SetValue(m_items[selection].getDescription());
        m_itemCount->SetLabel(wxString::Format("Quantity: %d", m_items[selection].getAmount()));
        m_itemCategory->SetLabel("Category: " + wxString::FromUTF8(m_items[selection].getCategory()));
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

void MainFrame::OnSetCategory(wxCommandEvent& evt) {
    int selection = m_listBox->GetSelection();
    if (selection != wxNOT_FOUND) {
        wxDialog dialog(this, wxID_ANY, "Change Category");
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        // Create and populate the choice control
        wxChoice* categoryChoice = new wxChoice(&dialog, wxID_ANY);
        categoryChoice->Append("[None]");
        for (const auto& category : m_categories) {
            categoryChoice->Append(category);
        }

        // Set current category
        int categoryIndex = 0;  // Default to [None]
        std::string currentCategory = m_items[selection].getCategory();
        for (size_t i = 0; i < m_categories.size(); ++i) {
            if (m_categories[i] == currentCategory) {
                categoryIndex = i + 1;  // +1 because [None] is at index 0
                break;
            }
        }
        categoryChoice->SetSelection(categoryIndex);

        sizer->Add(categoryChoice, 0, wxALL | wxEXPAND, 5);
        sizer->Add(dialog.CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxEXPAND, 5);
        dialog.SetSizer(sizer);

        if (dialog.ShowModal() == wxID_OK) {
            wxString newCategory = categoryChoice->GetSelection() == 0 ?
                wxString("[None]") : categoryChoice->GetString(categoryChoice->GetSelection());
            m_items[selection].setCategory(newCategory.ToStdString());
            m_itemCategory->SetLabel("Category: " + newCategory);
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
void MainFrame::InitializeDefaultInventory() {
    m_categories = { "Misc", "Armor", "Weapon" };

    Item sword("Sword", "A sharp blade for close combat", 5, "path/to/sword.png");
    sword.setCategory("Weapon");
    m_items.push_back(sword);

    Item shield("Shield", "Protective gear to block attacks", 3, "path/to/shield.png");
    shield.setCategory("Armor");
    m_items.push_back(shield);

    Item potion("Potion", "Restores 50 HP when consumed", 10, "path/to/potion.png");
    potion.setCategory("Misc");
    m_items.push_back(potion);

    Item bow("Bow", "Long-range weapon for precise attacks", 2, "path/to/bow.png");
    bow.setCategory("Weapon");
    m_items.push_back(bow);

    Item arrow("Arrow", "Ammunition for bow. Deals 25 damage", 50, "path/to/arrow.png");
    arrow.setCategory("Misc");
    m_items.push_back(arrow);
}

void MainFrame::OnNew(wxCommandEvent& evt) {
    wxMessageDialog dialog(
        this,
        "Would you like to load the default inventory or start with an empty inventory?",
        "New Inventory",
        wxYES_NO | wxCANCEL | wxICON_QUESTION
    );

    dialog.SetYesNoCancelLabels("Load Default", "Start Empty", "Cancel");
    int answer = dialog.ShowModal();

    if (answer != wxID_CANCEL) {
        m_items.clear();
        m_categories.clear();
        m_minQuantity = 0;
        m_maxQuantity = 999;
        m_selectedCategory = "";

        if (answer == wxID_YES) {
            InitializeDefaultInventory();
        }

        // Clear and update displays
        m_listBox->Clear();
        m_itemTitle->SetLabel("");
        m_itemDescription->SetValue("");
        m_itemCount->SetLabel("Quantity: 0");
        m_itemCategory->SetLabel("Category: [None]");
        m_searchBox->SetValue("");

        UpdateItemList();
    }
}

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

void MainFrame::OnExportCSV(wxCommandEvent& evt) {
    wxFileDialog saveFileDialog(this, "Export Inventory to CSV",
        "", "inventory.csv",
        "CSV files (*.csv)|*.csv",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;

    Save::SaveItems(m_items, saveFileDialog.GetPath().ToStdString());
}

void MainFrame::OnImport(wxCommandEvent& evt) {
    ImportDialog dialog(this);
    if (dialog.ShowModal() == wxID_OK) {
        wxProgressDialog progress("Importing Files",
            "Processing files...",
            dialog.GetSelectedFiles().size(),
            this,
            wxPD_APP_MODAL | wxPD_AUTO_HIDE | wxPD_SMOOTH);

        std::vector<std::string> newCategories;
        int fileCount = 0;

        for (const auto& filePath : dialog.GetSelectedFiles()) {
            progress.Update(fileCount++, "Importing: " + wxFileName(filePath).GetFullName());

            if (dialog.IsCSVFormat()) {
                std::vector<Item> importedItems;
                Save::LoadItems(importedItems, filePath);

                // Collect categories
                for (const auto& item : importedItems) {
                    std::string category = item.getCategory();
                    if (category != "[None]" &&
                        std::find(newCategories.begin(), newCategories.end(), category) == newCategories.end() &&
                        std::find(m_categories.begin(), m_categories.end(), category) == m_categories.end()) {
                        newCategories.push_back(category);
                    }
                }

                // Handle duplicate names
                for (auto& item : importedItems) {
                    std::string baseName = item.getName();
                    int suffix = 1;
                    while (std::any_of(m_items.begin(), m_items.end(),
                        [&](const Item& existing) { return existing.getName() == item.getName(); })) {
                        item.setName(baseName + " (" + std::to_string(suffix++) + ")");
                    }
                    m_items.push_back(item);
                }
            }
            else {  // CFG format
                std::vector<Item> importedItems;
                std::vector<std::string> importedCategories;
                Save::LoadFromCfg(importedItems, importedCategories, filePath);

                // Merge categories
                for (const auto& category : importedCategories) {
                    if (std::find(m_categories.begin(), m_categories.end(), category) == m_categories.end()) {
                        m_categories.push_back(category);
                    }
                }

                // Handle duplicate names
                for (auto& item : importedItems) {
                    std::string baseName = item.getName();
                    int suffix = 1;
                    while (std::any_of(m_items.begin(), m_items.end(),
                        [&](const Item& existing) { return existing.getName() == item.getName(); })) {
                        item.setName(baseName + " (" + std::to_string(suffix++) + ")");
                    }
                    m_items.push_back(item);
                }
            }
        }

        // Add new categories from CSV imports
        m_categories.insert(m_categories.end(), newCategories.begin(), newCategories.end());

        UpdateItemList();
        wxMessageBox("Import completed successfully!", "Import Complete", wxICON_INFORMATION);
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
