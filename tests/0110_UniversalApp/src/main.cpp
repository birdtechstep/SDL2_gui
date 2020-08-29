#include <SDL2/SDL.h>
#include <iostream>
#if defined(WIN32)
#include "..\_VisualC\WinApp\WinApp\resource.h"
#endif
#include "SDL_gui.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace std;

char title[] = "0110_UniversalApp";


static GUI_App *app;
static GUI_View *topView;

int main(int argc, char *argv[]) {
    app = GUI_App::create( title, 0, 0, 0, GUI_APP_TOP_BAR | GUI_APP_MENU | GUI_APP_STATUS_BAR );
    if( app == NULL ) {
        exit( 1 );
    }

#if defined(WIN32)
    GUI_SetWindowIcon(IDI_ICON1);
#endif

    topView = app->topView;

    app->menuView->setCallback( [=](GUI_View *v) {
        GUI_Menu *l = (GUI_Menu *)v;
        GUI_Log( "%s\n", l->selectedItem->title.c_str() );
        l->close( GUI_AppMenuCollapseTime );
    } );
    
    app->menuView->addSimpleMenu( "About app...", true );
    app->menuView->addSimpleMenu( "Simle List Item 2" );
    app->menuView->addSimpleMenu( "Simle List Item 3", true );
    app->menuView->addSimpleMenu( "Quit", true );
    
    app->contentView->setPadding( 10, 10, 10, 10 );
    app->contentView->setLayout( GUI_LAYOUT_VERTICAL );
    
    std::string platform = SDL_GetPlatform();
    GUI_Label *platformLabel = GUI_Label::create(app->contentView, platform.c_str());
    GUI_Label *tabletLabel = GUI_Label::create(app->contentView, SDL_IsTablet() ? "Tablet" : "Phone" );
    GUI_Label *resolutionLabel = GUI_Label::fromPrintf( app->contentView, "Drawable: %i x %i", GUI_physicalWindowWidth, GUI_physicalWindowHeight );
    GUI_Label *virtualLabel = GUI_Label::fromPrintf( app->contentView, "Virtual: %i x %i", GUI_windowWidth, GUI_windowHeight );
    GUI_Label *scaleLabel = GUI_Label::fromPrintf( app->contentView, "Scale: %0.2f (mouse: %0.2f)", GUI_scale, GUI_mouseScale );
    GUI_FPSView *fps = GUI_FPSView::create( app->contentView );

    GUI_onWindowSizeChange = [=](int w, int h) {
        GUI_Log( "Window size change %i %i\n", w, h );

        resolutionLabel->printf( "Drawable: %i x %i", GUI_physicalWindowWidth, GUI_physicalWindowHeight );
        virtualLabel->printf( "Virtual: %i x %i", GUI_windowWidth, GUI_windowHeight );
        scaleLabel->printf( "Scale: %0.2f (mouse: %0.2f)", GUI_scale, GUI_mouseScale );
    };

#ifdef __IPHONEOS__
    GUI_Label * deviceLabel = GUI_Label::create(app->contentView, getDeviceModel() );
    GUI_Label * orientationLabel = GUI_Label::create(app->contentView, getOrientation() );
    GUI_Label * notchLabel = GUI_Label::create(app->contentView, hasNotch() ? "Has notch" : "No notch" );
#else
#endif

    app->run();
    

    delete( app );

    return 0;
}




