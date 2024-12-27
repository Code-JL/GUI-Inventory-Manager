#include "MainFrame.h"
#include <wx/persist/toplevel.h>
#include <wx/display.h>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    // Get screen size
    wxDisplay display;
    wxRect screenSize = display.GetGeometry();

    // Calculate 60% of screen dimensions
    int width = screenSize.GetWidth() * 0.6;
    int height = screenSize.GetHeight() * 0.6;

    // Set size
    SetSize(width, height);

    // Enable persistence
    SetName("MainFrame");
    wxPersistentRegisterAndRestore(this, "MainFrame");
}
