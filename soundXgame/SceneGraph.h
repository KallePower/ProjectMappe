#ifndef __SCENEGRAPH__
#define __SCENEGRAPH__

#define MAX_MUM_SCENE_OBJECTS (4096)

#include "GuiManager.h"

typedef unsigned int GobID;
typedef char* string;

class IGObject;
class Cam;


class SceneGraph
{
private:
	SceneGraph(void);
	List<IDrawable*,MAX_MUM_SCENE_OBJECTS> drawables;
	float r,g,b;
	void UpdateSky(void);
	void SetGameIsRunningState(bool);

public:
	~SceneGraph(void);
	static const char* start(void);
	static SceneGraph* getInstance();
	void DrawAll(void);
	void DrawSky(void);


	//Activate Color-flashing Sky drawer !...
	bool SkyFlasherActive;

	//If set "true" actual frames per second will be drawn at the upper left screen corner.
	bool ShowFPS;

	//Add's an object to the scene and generate a new ID that will be returned... 
	unsigned int Add(IDrawable* object);

	// removes an object from the scene, but does not destroy it.
	// use "Destruct(IGObject*)" to delete an object permanently...
	void Remove(IDrawable* object);

	//removes the object with the given ID from the scene and calls it's destructor... 
	void Destruct(GobID ID);

	//removes the object named "Name" from the scene and calls it's destructor... 
	void Destruct(string Name);

	//if the Object can be found in the scene, it will be removed and destroyed... 
	void Destruct(IDrawable* Obj);

	// access to the scene's main-camera...
	Cam* camera;

	// Find an object by it's name...
	IObjection<IConnectable>* Object(char* name);  

	// Find an object by it's ID...
	IObjection<IConnectable>*  Object(unsigned int ID);

	// total count on objects in scene...
	unsigned ObjectsCount(void);

	// returns the GobID with the highest value that is contained in the scene graph...
	// its's the ID of the last Object at the end of the scene-draw-List...
	GobID HighestID(void);
};

#endif