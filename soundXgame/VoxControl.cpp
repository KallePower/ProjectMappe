#include "VoxControl.h"
#include "VoxGrid.h"
#include "projectMacros.h"
#include "CameraModesIncluder.h"

bool __ZedMode = false;
float _YPS=0.01;
string _files[6];


VoxControl::VoxControl(void)
{
	timer = 0;
	tempvector = *Vector3::Zero;
	bumpmapchannel=0;
		//_files[0]="buntbild_128.ppm";
		//_files[1]="drei_.ppm";
		//_files[2]="ich.ppm";
		//_files[3]="tr.ppm";
		//_files[4]="bluobbber.ppm";
		_files[0]="Bump_6.ppm";
		_files[1]="Bump_5.ppm";
		_files[2]="Bump_3.ppm";
		_files[3]="Color_1.ppm";
		_files[4]="Color_3.ppm";
		_files[5]="Color_4.ppm";
		bumpmapchannel=0;
		bumpmapIndex=0;
		imageIndex=0;
}


VoxControl::~VoxControl(void)
{
}


bool 
VoxControl::Initialize(void)
{
		INPUT->attachKey(this);
		return true;
}

char _lastKey='\0';
int _currentSellection=0;
void 
VoxControl::keyPress(char key)
{
	if(SCENE->camera->GetTarget()==this->Connection())
	{
	if(key=='x')
	{
		if(_lastKey!='x')
		{
			if(++_currentSellection>=5)
				_currentSellection=0;
			printf("File \"%s\" Sellected...\n",_files[_currentSellection]);
			_lastKey='x';
		}
	}


	if(key == 'r')
	{
		if(_lastKey!='r')
		{
			this->vConnection()->ReLoade(_files[imageIndex=_currentSellection]);
			printf("Image \"%s\" geloadedt...\n",_files[_currentSellection]);
			vConnection()->MainSizzes.x=0.2;
			vConnection()->MainSizzes.y=0.045f;
			_lastKey='r';
		}
	}


	if(key == 'l')
	{
		if(_lastKey!='l')
		{
			this->vConnection()->LoadeMap(_files[bumpmapIndex=_currentSellection],bumpmapchannel);
		printf("BumpMap file-\"%s\"-Channel %i geloadedt !\n",_files[_currentSellection],bumpmapchannel);
		if(++bumpmapchannel>3)
			bumpmapchannel=0;
		_lastKey='l';
		}
	}


	if(key=='b')
		this->vConnection()->Mode(IVoxelObject::ColorMode::BUNT);

	if(key=='n')
		this->vConnection()->Mode(IVoxelObject::ColorMode::NORMAL);

	if(key=='h')
		this->vConnection()->Mode(IVoxelObject::ColorMode::BYTE);

	if(key=='v')
	{
		if(_lastKey!='v')
		{
			SCENE->camera->SetTarget(this->Connection());
			_lastKey='v';
		}
	}

	if(key=='t')
	{
		if(_lastKey!='t')
		{
			if(transparenseEnabled)
			{
				glDisable(GL_BLEND);
			}
			else
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
			transparenseEnabled = !transparenseEnabled;
			_lastKey='t';
		}
	}

	if(key=='u')
	{
		if(_lastKey!='u')
		{
			this->vConnection()->flip(++this->vConnection()->flipt);
			_lastKey='u';
		}
	}

	if(key=='z')
		__ZedMode=true;
	else
		__ZedMode=false;
	}
}



VoxGrid* 
VoxControl::vConnection(void)
{
	return (VoxGrid*)this->connection;
}
float bumper = 0.0000f;
void
VoxControl::DoUpdate(void)
{
	if(_lastKey!='\0')
	{
		timer+=INPUT->FrameTime;
		if(timer>0.33)
		{
			timer=0;
			_lastKey='\0';
		}
	}
	if(SCENE->camera->GetTarget()==this->vConnection())
	{
		//if(SCENE->camera->mode<TargetGrabber>()->IsATargetGrabbed())
		//	SCENE->camera->ModeSocket->GetCameraMode<TargetGrabber>()->Mode(TargetGrabber::MODE::OFF);
		
		if(INPUT->Mouse.LEFT.HOLD)
		{
			this->vConnection()->MainSizzes.x *= 1-INPUT->Mouse.Movement.x/300;
			if(!__ZedMode)this->vConnection()->MainSizzes.y *= 1+INPUT->Mouse.Movement.y/600;
			else this->vConnection()->move(0,0, INPUT->Mouse.Movement.y);
			printf("MainSizerX: %f ,MainSizerY: %f\n",this->vConnection()->MainSizzes.x,this->vConnection()->MainSizzes.y);
		}
		if(INPUT->Mouse.MIDDLE.HOLD)
		{
			tempvector = this->vConnection()->getTransform()->rotation;
			vConnection()->getTransform()->position.z = ( tempvector.z += INPUT->Mouse.Position.x);
			tempvector.y += INPUT->Mouse.Movement.y;
			this->vConnection()->rotate(tempvector.x,tempvector.y,tempvector.z);
		}

		if(INPUT->Mouse.WheelV==WHEEL::UP)
		{
			if(INPUT->Mouse.LEFT.HOLD)
				bumper=0.00001;
			SCENE->camera->transform.position += Vector3(0,_YPS,0);
		}
	else 
		if(INPUT->Mouse.WheelV==WHEEL::DOWN)
		{
			if(INPUT->Mouse.LEFT.HOLD)
				bumper= -0.00001;
			SCENE->camera->transform.position -= Vector3(0,_YPS,0);
		}

		vConnection()->BumpFactor += bumper;
	}
}