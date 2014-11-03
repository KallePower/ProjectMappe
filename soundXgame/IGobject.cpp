
#include <iostream>
#include "projectMacros.h"
#include "DataStructs.h"
#include "Utility.h"
#include "Connectable.h"
#include "IGobject.h"

#define This this->conXtor->Connection()
static unsigned objIDs = 99;

IGobject::IGobject(void)
{
	IsVisible=false;
	this->_idIsSet=false;

	if(!this->_idIsSet)
	{
		this->ID = ++objIDs;
		this->_idIsSet=true;
	}

	itoa(this->ID,&this->Name[0],10);
}


IGobject::~IGobject(void)
{
	delete conXtor;
}

IMeshGobject::IMeshGobject(void)
{

	conXtor = new IConnectable();
	conXtor->SetConnection(this);

}

IMeshGobject::~IMeshGobject(void)
{

}

void
IMeshGobject::init(const char* objFile,const char* textureFile,bool addToSceneGraph)
{
	
	init(objFile,textureFile);

	if(addToSceneGraph)
		SCENE->Add(this);

}

void
IMeshGobject::init(const char* objFile, const char* textureFile)
{
	if(this->_idIsSet!=true)
	{
		this->ID = ++objIDs;
		this->_idIsSet=true;
	}
	Utility::loadObj(objFile,this->verts,this->uvs,this->norms);
	this->textureID = Utility::loadTexture(textureFile);
	glm::vec3 temp1 = glm::vec3(0,0,1);

	transform.forward = Vector3(temp1.x,temp1.y,temp1.z);
	
	//this->transform.right =&temp2;
	//this->transform.up =&temp3;

	verts.push_back(glm::vec3(temp1.x,temp1.y,temp1.z));
	//verts.push_back(temp2);
	//verts.push_back(temp3);

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	IsVisible = true;
}



GobID
IGobject::GetObjectID(void)
{
	return this->ID;
}



char* 
IGobject::GetName(void)
{
	return &this->Name[0];
}

void
IGobject::SetName(char* name)
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

TransformA*
IGobject::getTransform()
{
	return &This->transform;
}


IGobject::operator IConnectable(void)
{
	return *conXtor;
}


//template<typename T> T*
//IGobject::AddConnectable(void)
//{
//	return conXtor->AddConnectable<T>();
//}
//
//template<typename T> T*
//IGobject::GetConnected(void)
//{
//	return conXtor->GetConnected<T>();
//}








Vector3
IMeshGobject::move(Vector3 to)
{
	getTransform()->movement = (to - getTransform()->position);
	getTransform()->position = to;
	printf("\n%s - ID: %i Has Moved to %f,%f,%f !",GetName(),GetObjectID(),getTransform()->position.x,getTransform()->position.y,getTransform()->position.z);
	return getTransform()->position;
}

Vector3
IMeshGobject::move(float tox,float toy,float toz)
{
	getTransform()->movement.x = (tox - getTransform()->position.x);
	getTransform()->movement.y = (toy - getTransform()->position.y);
	getTransform()->movement.z = (toz - getTransform()->position.z);

	getTransform()->position.x = tox;
	getTransform()->position.y = toy;
	getTransform()->position.z = toz;

	
	printf("\n%s - ID: %i Has Moved to %f,%f,%f !",GetName(),GetObjectID(),getTransform()->position.x,getTransform()->position.y,getTransform()->position.z);


	return getTransform()->position;
}


Vector3
IMeshGobject::scale(Vector3 to)
{
	getTransform()->scale = to;
	return getTransform()->scale;
}

Vector3
IMeshGobject::rotate(Vector3 to)
{
	getTransform()->rotation = to;
	getTransform()->forward = glm::normalize((glm::vec3)getTransform()->rotation);
	return getTransform()->rotation;
}

Vector3
IMeshGobject::rotate(float toiX,float toYps,float toZed)
{
	getTransform()->rotation.x = toiX;
	getTransform()->rotation.y = toYps;
	getTransform()->rotation.z = toZed;
	getTransform()->forward = glm::normalize((glm::vec3)getTransform()->rotation);

	return getTransform()->rotation;
}

void
IMeshGobject::draw()
{
	if(IsVisible)
	{
		glPushMatrix();
		{
			glBindTexture(GL_TEXTURE_2D, textureID);

			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
			glVertexPointer(3, GL_FLOAT, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
			glTexCoordPointer(2, GL_FLOAT, 0, 0);
		
		
			//Translate...
			glTranslatef(getTransform()->position.x, getTransform()->position.y,getTransform()->position.z);

			//Rotate...
			glRotatef(getTransform()->rotation.x, 1, 0, 0);
			glRotatef(getTransform()->rotation.y, 0, 1, 0);
			glRotatef(getTransform()->rotation.z, 0, 0, 1);

			

		//	printf("forward: %f  \n\n",this->getTransform()->forward->z);
			//Scaleate...
		//	glScalef(this->transform.scale.x,this->transform.scale.y,this->transform.scale.z);
		
			glDrawArrays(GL_TRIANGLES, 0, verts.size());
		}
		glPopMatrix();
	}
}

#undef This;

//TransformA*
//AbsGobject::getTransform(void)
//{
//	return &this->transform;
//}