#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/config.h>

#pragma warning(disable: 28251)
wxIMPLEMENT_APP(App);

bool App::OnInit() {
    // Set up configuration storage
    wxConfig::Set(new wxConfig("InventoryManager"));

    // Initialize persistence manager
    m_persistMgr = &wxPersistenceManager::Get();

    MainFrame* mainframe = new MainFrame("Inventory Manager");

    // Restore window position and size
    if (!wxPersistenceManager::Get().RegisterAndRestore(mainframe))
    {
        mainframe->Center();
    }

    mainframe->Show();
    return true;
}

int App::OnExit()
{
    delete wxConfig::Set(nullptr);
    return wxApp::OnExit();
}
