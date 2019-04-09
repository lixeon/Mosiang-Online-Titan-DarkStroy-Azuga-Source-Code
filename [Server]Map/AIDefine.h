#ifndef __AIDEFINE_H__
#define __AIDEFINE_H__

#define BEGIN_STATE(a)		{	switch((a))	{	case -1:	{	// COMMAND °¡ -1ÀÌ µÉ¸®°¡ ¾øÀ¸¹Ç·Î...
#define END_STATE				}	return;		}	}
#define STATEOF(a)				}	return;		case (a):	{

enum eStateEvent
{ 
	eSEVENT_NULL,
	eSEVENT_Process,
	eSEVENT_Message,
	eSEVENT_Enter,
	eSEVENT_Leave,
};

#define BEGIN_EVENT(a)		{	switch((a))	{	case -1:	{	// COMMAND °¡ -1ÀÌ µÉ¸®°¡ ¾øÀ¸¹Ç·Î...
#define END_EVENT				}	return;		}	}
#define OnEnter				}	return;		case (eSEVENT_Enter):	{
#define OnMsg				}	return;		case (eSEVENT_Message):	{
#define OnLeave				}	return;		case (eSEVENT_Leave):	{
#define OnProcess			}	return;		case (eSEVENT_Process):	{

#define BEGIN_COMMON_MSG(a)		if(msg){	switch((a))	{	case -1:	{	// COMMAND °¡ -1ÀÌ µÉ¸®°¡ ¾øÀ¸¹Ç·Î...
#define END_COMMON_MSG				}	break;		}	}
#define MSGOF(a)				}	break;		case (a):	{


enum eMONSTER_EMOTION
{
	eME_PLEASURE,
	eME_COMFORT,
	eME_DOZE,
	eME_SAD,
	eME_ANGER,
};

enum enumMESSAGEKINDS
{
	eMK_Chat,
	eMK_Recall_Direct,
	eMK_Recall_Script,
	eMK_PlayerCurPos,
	eMK_MobCurPos,
	eMK_HelpRequest, 
	eMK_HelpShout,
	eMK_HelpObey,
};




#endif __AIDEFINE_H__
