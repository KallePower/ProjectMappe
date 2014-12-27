#include <iostream>
#include "projectMacros.h"
//#include "DataStructs.h"
#include "Utility.h"
#include "Connectable.h"
#include "IGObject.h"
#include "Ground.h"

#define This this->conXtor->Connection()

static unsigned objIDs = 99;
std::vector<GobID> usedIDs = std::vector<GobID>();

bool
_IDIsFree(GobID id)
{
	for(auto it=usedIDs.begin();it!=usedIDs.end();it++)
		if((*it)==id)
			return false;
	return true;
}

IGObject::IGObject(void)
{
	IsVisible=true;
	this->_idIsSet=false;
	conXtor = new IConnectable();
	conXtor->SetConnection(this);
	
	GroundValue = 0;
	IsGrounded(false);
//	UpdateManager::getInstance()->SignInForLateUpdate(this);

}

IGObject::~IGObject(void)
{
	delete conXtor;
}

bool
IGObject::SetID(GobID id)
{
	if(this->_idIsSet==false)
	{
		this->ID = id;
		itoa(this->ID,&this->Name[0],10);
		usedIDs.push_back(id);
		return this->_idIsSet=true;
	}
	else
		return false;
}

unsigned
IGObject::LockID(void)
{
	if(!_idIsSet)
	{
		while(!_IDIsFree(++objIDs));
		this->ID = objIDs;
		usedIDs.push_back(this->ID);
		itoa(this->ID,&this->Name[0],10);
		this->_idIsSet=true;
		return this->ID;
	}
}

void
IGObject::InitializeObject(bool addToSceneGraph)
{
	if(addToSceneGraph)
	{
		SetID(SCENE->Add(this));
		LockID();
	}
}


GobID
IGObject::GetID(void)
{
	return this->ID;
}

// Ground
bool
IGObject::IsGrounded()
{
	return _isGrounded;
}

void
IGObject::IsGrounded(bool status)
{
	_isGrounded = status;
}

Vector3 
IGObject::move(Vector3 m) 
{
	getTransform()->movement = m - getTransform()->position;
	getTransform()->position = m;
	if(AlwaysFaceMovingdirection)
	{
		Vector3 vec = getTransform()->movement.normalized();
	    Vector3 axis = getTransform()->forward.cros(vec);
		 
		rotate(glm::acos(getTransform()->forward.dot(vec))/(M_PI/180.0),axis);
		getTransform()->forward = vec;
	}

	#ifdef OBJECT_DEBUG_MESSAGES_ON
		printf("OBJECT: %s-%i: moved to X: %f, Y: %f, Z: %f\n",GetName(),GetID(),getTransform()->position.x,getTransform()->position.y,getTransform()->position.z);
	#endif
	return getTransform()->position;


}

Vector3 
IGObject::rotate(Vector3 r) 
{
	getTransform()->rotation = r;
	return getTransform()->rotation;
}

Vector3
IGObject::rotate(float rotationAngle,Vector3 axis)
{
	angle = rotationAngle;
	return rotate(axis);
}

Vector3 
IGObject::scale(Vector3 s) 
{
	getTransform()->scale = s;
	return getTransform()->scale;
}

Vector3 
IGObject::move(float X,float Y,float Z)
{
	return move(Vector3(X,Y,Z));
}

Vector3 
IGObject::rotate(float X,float Y,float Z)
{
	return rotate(Vector3(X,Y,Z));
}

Vector3 
IGObject::scale(float X,float Y,float Z)
{
	return scale(Vector3(X,Y,Z));
}





char* 
IGObject::GetName(void)
{
	return &this->Name[0];
}

void
IGObject::SetName(char* name)
{
	int i = 0;
	while(i<63)
	{
		this->Name[i] = name[i];
		if(this->Name[i]=='\0')
			return;
		i++;
	}
	Name[63]='\0';
}

Transform*
IGObject::getTransform()
{
	return &This->transform;
}


IGObject::operator IConnectable(void)
{
	return *conXtor;
}







#undef This;


