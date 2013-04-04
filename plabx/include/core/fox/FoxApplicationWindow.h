/*
 * FoxApplicationWindow.h
 *
 *  Created on: 11/05/2009
 *      Author: pete
 */

#ifndef FOXAPPLICATIONWINDOW_H_
#define FOXAPPLICATIONWINDOW_H_

#include <fox/fx.h>

class FoxInputHandler;

class FoxApplicationWindow : public FXMainWindow
{
public:
	FoxApplicationWindow(FXApp* app, int x, int y, int width, int height);
	virtual ~FoxApplicationWindow();

    virtual void create();

    enum {
        ID_HELP_CONTENTS = FXMainWindow::ID_LAST, ID_HELP_ABOUT,
        ID_FILE_NEW, ID_FILE_OPEN, ID_FILE_SAVE, ID_FILE_SAVE_AS,
        ID_EDIT_UNDO, ID_EDIT_REDO, ID_EDIT_CUT, ID_EDIT_COPY, ID_EDIT_PASTE,
        ID_CHORE,
        ID_GL_VIEW,

        // Dummy High Level (HL) events...
        ID_HL_POINT_SELECTED,
        ID_HL_POLYGN_DRAWN,

        ID_LAST };

public:
	void setInput(FoxInputHandler* input);

public /*Message handlers*/:
	// long onNew(FXObject* sender, FXSelector message, void* event);
	// long onOpen(FXObject* sender, FXSelector message, void* event);
	// long onSave(FXObject* sender, FXSelector message, void* event);
	// long onIdle(FXObject* sender, FXSelector sel, void* ptr);
	// long onMouseMove(FXObject* sender, FXSelector sel, void* ptr);


	// Dummy high level event triggers...
	long onPointSelected(FXObject*, FXSelector, void*);
	long onPolygonDrawn(FXObject*, FXSelector, void*);

protected:
	FoxApplicationWindow(){};
	FXDECLARE(FoxApplicationWindow);

	FXMenuPane* fileMenu, *editMenu, *helpMenu;
	FXStatusBar* statusBar;
	FXProgressDialog* progressDialog;

	FoxInputHandler* inputHandler;

};

#endif /* FOXAPPLICATIONWINDOW_H_ */
