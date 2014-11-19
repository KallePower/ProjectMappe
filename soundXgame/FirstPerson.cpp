#include "FirstPerson.h"
#include "projectMacros.h"

int FirstPerson::StaticCamModeID = -1;


FirstPerson::FirstPerson(void)
{
	ModeName = "FirstPerson";
}

FirstPerson::~FirstPerson(void)
{

}



bool
FirstPerson::Initialize(void)
{
	angle		= 0;
	lx			= 0;
	lz			= -1;
	x			= 0;
	z			= 5;
	eyeY		= 1;
	moveSpeed	= 0.1f;
	mouseSpeed	= 1.0f;
	mouseX		= 0;
	mouseY		= 0;
	mouseX = SCREENWIDTH/2;
	mouseY = SCREENHEIGHT/2;
	INPUT->attachKey(this);
	INPUT->attachMouseMove(this);
	INPUT->attachMouseWheel(this);
	INPUT->attachSpecial(this);
	StaticCamModeID = this->CamModeID();
	return true;
}

void 
FirstPerson::UpdateMode(void)
{ 
	camera->move(x, eyeY, z);
	camera->rotate(x+lx, 1.0f, z+lz); 
	this->IsDirty=false;
}


void 
FirstPerson::keyPress(char key)
{

	if(IsActive)
	{
		switch(key)
		{
			case 119: // W
				x += lx * moveSpeed;
				z += lz * moveSpeed;
				IsDirty=true;
				break;
			case 115: // S
				x -= lx * moveSpeed;
				z -= lz * moveSpeed;
				IsDirty=true;
				break;
			case 97: // A
				x += lz * moveSpeed;
				z += lx * (moveSpeed*-1);
				IsDirty=true;
				break;
			case 100: // D
				x -= lz * moveSpeed;
				z -= lx * (moveSpeed*-1);
				IsDirty=true;
				break;
		}
				

		UpdateMode();
	}
}

void
FirstPerson::specialKeyPressed(int key)
{
	if(IsActive)
	{
	switch (key) {
		case GLUT_KEY_UP:
			x += lx * moveSpeed;
			z += lz * moveSpeed;
			IsDirty=true;
			break;
		case GLUT_KEY_DOWN:
			x -= lx * moveSpeed;
			z -= lz * moveSpeed;
			IsDirty=true;
			break;
		case GLUT_KEY_LEFT:
			x += lz * moveSpeed;
			z += lx * (moveSpeed*-1);
			IsDirty=true;
			break;
		case GLUT_KEY_RIGHT:
			x -= lz * moveSpeed;
			z -= lx * (moveSpeed*-1);
			IsDirty=true;
			break;
		}
	UpdateMode();
	}
}

void
FirstPerson::mouseMotion(int newX, int newY)
{
	if(IsActive)
	{
		// if mouse have'n change -> return
		if(newX == mouseX && newY == mouseY)
			return;

		// check difference between last-frame mouse pos
		int diffX = newX - mouseX;
		int diffY = newY - mouseY;

		// calculate
		angle += 0.005f * diffX * mouseSpeed;
		lx = sin(angle);
		lz = -cos(angle);
		eyeY += (float)diffY / 300;
	
		// set mouse pos center to screen
		mouseX = SCREENWIDTH/2;
		mouseY = SCREENHEIGHT/2;
	
		// fix to static mouse pos
		glutWarpPointer(mouseX, mouseY);

		// flag check data uptodate
		IsDirty=true;
		UpdateMode();
	}
}

