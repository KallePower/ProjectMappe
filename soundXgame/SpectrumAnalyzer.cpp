#include "SpectrumAnalyzer.h"
#include "projectMacros.h"
#include "Connectable.h"
#include "PointAndClick.h"

//void
//_OnScaleY(IConnectable* sender)
//{
//	printf("SpectrumAnalizer: scaleY: %f\n",(float)((Knob*)sender)->Value);
//}
//
//void
//_OnRotateSpecrumAnalizer(IConnectable* sender)
//{
//	printf("SpectrumAnalizer: rotationY: %f\n",(float)((Knob*)sender)->Value);
//}
//
//void _setFALLOFFAMOUNT(IConnectable* sender)
//{
//	printf("SpectrumAnalizer: FallOffAmount: %f\n",(float)((Knob*)sender)->Value);
//}


SpectrumAnalyzer::SpectrumAnalyzer(void)
{
	IGObject::iherited.push_back((unsigned)typeid(IEditable).hash_code());
	IGObject::InitializeObject();
	this->ChartHeight = 100.0f;

	//GUIPanel = (ObjectMenu*)GuiManager::getInstance()->Panel("Editor-Panel");
	//AddControlElement(ControlElementBinding::ELEMENT_TYPE::KNOB,&this->getTransform()->rotation.y,"Rotation",_OnRotateSpecrumAnalizer);
	//AddControlElement(ControlElementBinding::ELEMENT_TYPE::KNOB,&this->fallOffAmount,"fallOff",_setFALLOFFAMOUNT);
	//AddControlElement(ControlElementBinding::ELEMENT_TYPE::KNOB,&this->getTransform()->scale.y,"Scale Y",_OnScaleY);
}

void
SpectrumAnalyzer::Initialize(void)
{
	//Setting up a color...
	color.byte[1]=0;
	color.byte[2]=255;
	color.byte[3]=127;
	color.byte[0]=60;

	// Setting up an offset-vector 
	// (will be added to each Sub-object's vertices's)  
	Vector3 offset = getTransform()->position;
	offset.y += 0.5;
	float X = offset.x;
	X-=(SPECTRUM_SIZE*0.5);
	
	// Instantiate as many Cubes as there are frequency-bands 
	// in the used fft-window... for each, increase its position-offset
	// by its size along the offset-direction.
	for(int i = 0; i<SPECTRUM_SIZE;i++)
	{
		offset.x = X + i;
		bands[i]=(new Cubus(color,true,false,offset));
		bands[i]->IsGrounded(true);
		bands[i]->GroundedWithPivot = true;
		bands[i]->scale(getTransform()->scale);
	}
	
	// reset movement every step you select it for executing with bands
	transform.movement = *Vector3::Zero;
	
	/* Setting the Meters "FallOff"...
	 * as lower the value, as slower the visuals will
	 * Fall back to Zero if there's no signal. */
	fallOffAmount = 0.05f / 3.0f * ChartHeight;
	
	// this set's the hight of the analyzers chart.
	// band measures will multiplied by it.
	getTransform()->scale.y = ChartHeight;

	//get being invoked at EarlyUpdate, to get the FFT-Data...
	UPDATE->SignInForEarlyUpdate(this);

}

void 
SpectrumAnalyzer::SetFalloff(float falloff)
{
	fallOffAmount = falloff;
}

void
SpectrumAnalyzer::DoEarly(void)
{ // Get The FFT-Data-Buffer from desired audio-channel.
  // ...in this case the Background audio channel....
	fftData = (float*)AUDIO->GetBackgroundAudioFFT(FFT_SIZE::Small);
}

void
SpectrumAnalyzer::DoUpdate(void)
{//Reading out the Early-getted fft-buffer and apply
 //it's values on the Sub-Cubes....
	float changeFactor;
	for(int i=0;i<SPECTRUM_SIZE;i++)
	{
		//scaling data values to the desired size...
		//also let's flatten the scale and maybe do some compression 
		//to make the chart looks more strait, and the bands 
		//look'n more equivalent then real spectrum-data will do...  

		changeFactor = (fftData[i]*((float)1+i*0.128));         
		ChangeSize(i,changeFactor * getTransform()->scale.y);
		ChangeColor(i,changeFactor);
	}
}

void
SpectrumAnalyzer::DoLate(void)
{
	//maybe the sub-updates here better...
	
}

void 
SpectrumAnalyzer::ChangeColor(int index,float factor)
{
	//Do flashing the colors.....
	color.byte[1] = ((factor*10) * 255.0);
	color.byte[2] = (1.0 - (factor*10))*255.0;
	bands[index]->SetColor(color);
}
void
SpectrumAnalyzer::ChangeSize(int band,float newScaleY)
{   
	//apply the measured and compressed signal values to the
	//Sub-cube's Transform scale's if the values have changed...

	//if the new value is lower than the old value,
	//shrink the cube's Y-Axis by the "Fall-back"-value...
	if(newScaleY < bands[band]->getTransform()->scale.y)
		bands[band]->getTransform()->scale.y -= fallOffAmount;
	else  // if th new value is higher than the old one, just set it's Y-scale to the new value...
		bands[band]->getTransform()->scale.y = newScaleY;
}

void
SpectrumAnalyzer::draw(void)
{

	Vector3 degree90(0,90,0);
	Vector3 vec = getTransform()->rotation;
	for(int i = 0;i<3;i++)
	{
		rotate(getTransform()->rotation + degree90);
		drawOnce();
	}
	rotate(vec);
	drawOnce();

}

void
SpectrumAnalyzer::drawOnce(void)
{
	if(_CHANGEDposition || _CHANGEDrotation || _CHANGEDscale)
	{
		// Re-Positioning,seizing and rotating the SubCubes
		// if the whole analyzer object has been moved,resized 
		// or rotated during last update..  -->> maybe this could be don at LateUpdate better... don't know....
		for(int i=0;i<SPECTRUM_SIZE;i++)
		{
			if(_CHANGEDposition)
				bands[i]->getTransform()->position += transform.movement;
			if(_CHANGEDrotation)
				bands[i]->getTransform()->rotation = getTransform()->rotation;
			if(_CHANGEDscale)
				bands[i]->scale(transform.scale.x,bands[i]->getTransform()->scale.y,transform.scale.z);
		}
		_CHANGEDposition = _CHANGEDrotation = _CHANGEDscale = false;
		transform.movement = *Vector3::Zero;
	}

	//draw each sub-cube....
	for(int i=0;i<SPECTRUM_SIZE;i++)
	{
		bands[i]->draw();
	}	
}

Vector3
SpectrumAnalyzer::move(float X,float Y,float Z)
{
	IMeshObject::move(Vector3(X,Y,Z));
	_CHANGEDposition=true;
	return getTransform()->position;
}

Vector3
SpectrumAnalyzer::rotate(Vector3 rotary)
{
	_CHANGEDrotation=true;
	return IMeshObject::rotate(rotary);
}

// scale this object to the size each sub object will have.. 
Vector3
SpectrumAnalyzer::scale(float X,float Y,float Z)
{
	_CHANGEDscale=true;
	return IMeshObject::scale(Vector3(X,Y,Z));
}

SpectrumAnalyzer::~SpectrumAnalyzer(void)
{   // don't forget signing out from the
	// Events we're registered to if destructing...
	UPDATE->SignOutFromEarlyUpdate(this);
	UPDATE->SignOutFromUpdate(this);
	UPDATE->SignOutFromLateUpdate(this);
	for(int i = SPECTRUM_SIZE-1;i>=0;i--)
		bands[i]->~Cubus();
	//delete fftData;
}