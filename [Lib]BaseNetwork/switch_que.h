#pragma once

void SwitchQue();
BOOL StartSwitchQue(HWND hWnd);
void EndSwitchQueThread();
void MainThreadComplete();

BOOL StartSwitchQueThread(HWND hWnd);
void SwitchQueEnter();
void SwitchQueLeave();
void SwitchQue();
void MainThreadComplete();
void EndSwitchQueThread();