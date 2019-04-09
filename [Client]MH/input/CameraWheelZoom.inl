
//#define ZOOMRATE -80.0f
#define ZOOMRATE -1.0f
#define FLYRATE -1.0f
//////////////////////////////////////////////////////////////////////////
// Ä«¸Þ¶ó ÁÜÀÎ¾Æ¿ô
void CameraWheelZoom(CKeyboard* Keyboard,CMouse* Mouse)
{
	int Wheel = Mouse->GetWheel();
	if(Wheel)
	{
		if( CAMERA->GetCameraMode() == eCM_EyeView )
		{
			float fWheel = Wheel * FLYRATE;
			CAMERA->Fly(fWheel);
		}
		else
		{
			float fWheel = Wheel * ZOOMRATE;
			CAMERA->ZoomDistance(0,fWheel,0);
		}
	}
}
