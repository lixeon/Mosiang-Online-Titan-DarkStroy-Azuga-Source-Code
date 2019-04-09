#ifndef SSE_MATH_H
#define SSE_MATH_H

#include "../4DyuchiGRX_common/typedef.h"
/*
#include "d3dtype.h"

void SSECalcLightPoint(D3DTLVERTEX* result,VECTOR4* p,VECTOR4* normal,VECTOR4* light,float* bgra,DWORD num);
void SSETransformVectorWeight(VECTOR4* rv,VECTOR4* sv,D3DMATRIX* sm,float weight);
void SSETransformVectorWeight(VECTOR4* rv,VECTOR4* sv,D3DMATRIX* sm,float* pweight,DWORD count);
void SSESetIdentityMatrix(D3DMATRIX* mat);
void SSEDotProduct3(float* result,VECTOR4* a,VECTOR4* b);
void SSEDotProduct4(float* result,VECTOR4* a,VECTOR4* b);
void SSENormalize(VECTOR4* r,VECTOR4* v);
DWORD SSECalcLightPoint(VECTOR4 *p, VECTOR4* light,VECTOR4* normal,DWORD color);
void SSECalcLightPoint(unsigned char* pcolor,VECTOR4 *p, VECTOR4* light,VECTOR4* normal,DWORD color);


void SSETransformVector3(VECTOR4* rv,VECTOR4* sv,D3DMATRIX* sm);
void SSETransformToTLVertex(D3DTLVERTEX* rv,D3DVERTEX* sv,D3DMATRIX* sm,CLIP* clip,DWORD count);
void SSETransformToTLVertex(D3DTLVERTEX* rv,VECTOR4* sv,D3DMATRIX* sm,CLIP* clip,DWORD count);
void SSETransformToTLVertex(D3DTLVERTEX* rv,PHYSIQUEVERTEX* pv,CLIP* clip,DWORD count);
void SSETransformToTLVertexLight(D3DTLVERTEX* rv,VECTOR4* sv,D3DMATRIX* sm,CLIP* clip,DWORD count,VECTOR4* light);

void SSETransformVector4(VECTOR4* rv,VECTOR4* sv,D3DMATRIX* sm);
void SSETransformVector4(VECTOR4* rv,VECTOR4* sv,D3DMATRIX* sm,DWORD count);


void SSEMemcpy(void* dest,void* src,const DWORD size);
void SSESum(VECTOR4* rv,VECTOR4** ppv4,DWORD count);
void SSESum(D3DTLVERTEX* rv,VECTOR4** ppv4,DWORD count);
void SSERaster(D3DTLVERTEX* rv,VECTOR4* sv,CLIP* clip,DWORD count);
void SSEMatrixFromQuaternion( D3DMATRIX* mat,QUARTERNION* q);
*/

void SSEMatrixMultiply(D3DMATRIX* rm,D3DMATRIX* m1,D3DMATRIX* m2);
void SSEQuaternionSlerp(QUARTERNION* q,QUARTERNION* a,QUARTERNION* b,float fAlpha);


float SSESin(float x);
float SSECos(float x);
float SSEASin(float x);
float SSEACos(float x);


float Sin(float x);
float Cos(float x);
float ASin(float x);
float ACos(float x);


#endif
