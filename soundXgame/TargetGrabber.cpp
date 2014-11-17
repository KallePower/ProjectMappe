#include "TargetGrabber.h"
#include "CameraModesIncluder.h"


TargetGrabber::TargetGrabber(void)
{
	_targetGRABBED=false;
	ModeName = "TargetGrabber";
}


TargetGrabber::~TargetGrabber(void)
{

}

int 
TargetGrabber::StaticCamModeID = -1;

bool
TargetGrabber::Initialize(void)
{
	StaticCamModeID = this->CamModeID();
	return true;
}

void 
TargetGrabber::UpdateMode(void)
{
	if(_targetGRABBED)
	{
		_targetGRABBED = !INPUT->Mouse.LEFT.DOUBLE;
		camera->GetTarget()->getTransform()->position = (camera->transform.position + (camera->transform.forward * camera->GetTargetDistance())) + (INPUT->Mouse.MIDDLE.HOLD? (camera->transform.forward * -INPUT->Mouse.Movement.y/10) : Vector3(0,0,0));
	}
	else if(INPUT->Mouse.LEFT.DOUBLE)
	{
		printf("CAMERA: TARGETGRABBER: target \"%s\" grabbed!...\n",GrabTarget());
	}
}

IGObject*
TargetGrabber::GrabTarget(void)
{
	if(!camera->GetTarget())
		return NULL;
	_targetGRABBED = true;
	return camera->GetTarget();
}