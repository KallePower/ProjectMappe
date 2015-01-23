#include "Edit.h"

int
Edit::ID = -1;

Edit::Edit(void)
{
	ModeName = "Edit";
	isPrimarMode=true;
}


Edit::~Edit(void)
{
}

bool 
Edit::Initialize(void)
{

	angle		= 0;
	lx			= 0;
	lz			= -1;
	x			= -17.0f*3.5f;
	y			= 0;
	z			= 17.0f*3.5f;
	eyeY		= 1;
	moveSpeed	= 0.5f;
	mouseSpeed	= 1.0f;
	mouseX = SCREENWIDTH/2;
	mouseY = SCREENHEIGHT/2;
	INPUT->attachKey(this);
	INPUT->attachMouseMove(this);
	INPUT->attachMouseWheel(this);
	INPUT->attachSpecial(this);
	ID = this->CamModeID();
	accelerate = false;
	return true;
}


void 
Edit::UpdateMode(void)
{ 
	camera->move(x, eyeY, z);
	camera->rotate(x+lx, y + 1.7f, z+lz); 
	this->IsDirty=false;

	gluLookAt(camera->transform.position.x, camera->transform.position.y, camera->transform.position.z,
	camera->transform.rotation.x,camera->transform.rotation.y,camera->transform.rotation.z,	0, 1, 0);
}


void 
Edit::keyPress(char key)
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
Edit::specialKeyPressed(int key)
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
Edit::mouseMotion(int newX, int newY)
{
	if(!IsActive)
		return;

	if(INPUT->Mouse.RIGHT.HOLD)
	{
		// if mouse haven't change -> return
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
		mouseX = SCREENWIDTH / 2;
		mouseY = SCREENHEIGHT / 2;

		//center cursor...
		glutWarpPointer(mouseX, mouseY);

		// flag check data uptodate
		IsDirty=true;
		UpdateMode();
	}

}

void
Edit::OnActivate(void)
{
	//show cursor
	glutSetCursor(GLUT_CURSOR_INHERIT);
	//update view
	camera->NeedViewUpdate=true;
	glutReshapeWindow(SCREENWIDTH-1,SCREENHEIGHT-1);
}
