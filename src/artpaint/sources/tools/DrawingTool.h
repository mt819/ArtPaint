/* 

	Filename:	DrawingTool.h
	Contents:	DrawingTool base-class declaration	
	Author:		Heikki Suhonen
	
*/

#ifndef DRAWING_TOOL_H
#define	DRAWING_TOOL_H

#include "PaintApplication.h"
#include "ImageView.h"
#include "Tools.h"
#include "MessageConstants.h"
#include "UtilityClasses.h"
#include "Controls.h"
#include "BitmapDrawer.h"
#include "ToolScript.h"
#include "Image.h"

#define	HS_MAX_TOOL_NAME_LENGTH		50

// These are used in tool setup-window.
#define TOOL_CHANGED 	'Tlcg'
#define OPTION_CHANGED	'Opcg'


// this is a base class that specific tool-classes will be based on
class DrawingTool {
private:
	
	
	
protected:
	char name[HS_MAX_TOOL_NAME_LENGTH];
	int32 type;

	int32	options;
	int32	number_of_options;
	
	// this struct contains the tool's settings
	tool_settings	settings;
	
	// The UseTool-function should set this region. Before starting the
	// UseTool-function should wait for this region to become empty
	BRect		last_updated_rect;
			
public:
		DrawingTool(const char *tool_name, int32 tool_type);
virtual	~DrawingTool();

virtual	ToolScript*	UseTool(ImageView*,uint32,BPoint,BPoint);
virtual	int32		UseToolWithScript(ToolScript*,BBitmap*);

virtual	BView*		makeConfigView();
virtual	void		UpdateConfigView(BView*) {};


inline	int32	Options() { return options; }
virtual	void	SetOption(int32 option,int32 value, BHandler *source=NULL);

virtual	int32	GetCurrentValue(int32 option);

inline	const	char*	GetName() const { return name; }
inline	const	int32	GetType() const { return type; } 		

// these functions read and write tool's settings to a file
virtual status_t	readSettings(BFile &file,bool is_little_endian);
virtual	status_t	writeSettings(BFile &file);

		BRect	LastUpdatedRect();
		
virtual	const	void*	ReturnToolCursor();

virtual	const	char*	ReturnHelpString(bool is_in_use);
};




class DrawingToolConfigView : public BView {
protected:
	DrawingTool		*tool;
	
public:
		DrawingToolConfigView(BRect,DrawingTool*);
		~DrawingToolConfigView();
		
void	AttachedToWindow();		

void	MessageReceived(BMessage*);
};

#endif