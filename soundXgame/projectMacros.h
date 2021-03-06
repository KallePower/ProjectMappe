#ifndef _POJECT_MACROS__
#define _POJECT_MACROS__

#include <vector>
#include <iostream>
#include <windows.h>
#include <windef.h>

#include "InputManager.h"
#include "SceneGraph.h"
#include "UpdateManager.h"
#include "GuiManager.h"
#include "BassAudio.h"
#include "ObjectManagement.h"

#define SCREENWIDTH_STARTUP 1600
#define SCREENHEIGHT_STARTUP 1000
#define SCREENWIDTH InputManager::getInstance()->GetViewportRectangle()[2]
#define SCREENHEIGHT InputManager::getInstance()->GetViewportRectangle()[3]
#define MAIMUM_NUMBER_OF_VOXELS (16384)
#define CONTROLERPOLLINTERVAL (50)
#define clrscr system("cls")
#define M_PI (3.14159265358979323846)
#define	EMPTY (4294967295u)






/////////////////////////////////////////////////////////
// Accses to the project's Static Environment-Objects: //
/////////////////////////////////////////////////////////

// SCENE->
/* 
	handles the Scene... (how this could be ?) 
*/

#define SCENE	SceneGraph::getInstance()


// INPUT->
/* 
   handles everything thats got to do with user-input: 		
   contains actual Mouse- ,keybord-, controller- States,	
   also Timers and actual delta-FrameTime and some other 	
   helpul stuff can be found here. 							
   It also handles InvokationList where IObserver-derived
   objects can be attatched to, to be calld when Input ocurres.. 
*/

#define INPUT	InputManager::getInstance()			


// UPDATE->
/*
   Mamages the Per-Frame Called Updatefunctions...
   wich where are:

		DoEarly	  - happens at first.. calls it's update-client-object's "DoEarly()"-function... happens just before the ObserverPattern-based Events are Fired !
		DoUpdate  - same as above but happenes after he Observer-Invokations... call's "DoUpdate()"-functiom on all objects signed in for Update... 
		DoLate    - It's calling-Time depends on the "LATE_BEFORE_DRAW / LATE_AFTER_DRAW" directive..  calls "DoLate()"-function on it's clients...
*/	

#define UPDATE	UpdateManager::getInstance()





// GUI2D->
/*
  for drawing 2D-Textures, writing text and setting up buttons and sliders., etc... 
  also should contain options for saving, loading, xml-serialization and such stuff soon...
*/

#define GUI GuiManager::getInstance()



#define OBJECT ObjectManagement::getInstance()



#endif