#include "mtlexp.h"

interface I3DModel;
INT_PTR CreatePanel(HINSTANCE hInst,HWND hWnd);
I3DModel*	GetRootModel();
void		ReleaseModel();

