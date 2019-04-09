#pragma once

#include "../4DyuchiNET_Common/typedef.h"


BOOL StartMainThread(DWORD dwFlag);
void EndMainThread();	// 메인 스레드 종료

HANDLE	GetMainThreadMsgEvent();	// 메인스레드에 메시지 수신을 통보할 이벤트 핸들 리턴
HANDLE	GetMainThreadUserDefineEvent(DWORD index);	// 사용자 정의 이벤트 핸들 리턴

void SetMainThreadUserDefineEventFunc(DWORD index,EVENTCALLBACK pFunc);	// 사용자 정의 이벤트가 발생했을때 사용할 함수 설정
																		// Modified by chan78 at 2001/10/17
void SetMainThreadUserDefineEventNum(DWORD num);	// 사용자 정의 이벤트 갯수 설정

void BreakMainThread();	// 메인 스레드 일시정지
void ResumeMainThread();// 메인 스레드 재가동
void ProcessPreConnectedItem();	// accept,connect만 된 pre-connection들을 처리
void SetOnInitialFunc(OnIntialFunc pFunc);




