#ifndef _cRESOURCEHEADER_H_
#define _cRESOURCEHEADER_H_

#include "MHFile.h"

#define SWINDOWTYPE(a)		switch(a){

#define CASE(a)			case a:{
#define BREAK			}break;
#define EWINDOWTYPE			}


#define CMD_ST(a)	char buffer[255]; strcpy(buffer, a); if(0){
#define CMD_CS(b)		}else if(strcmp(buffer, b) == 0){
#define CMD_EN			}

//#ifdef _JAPAN_LOCAL_

//#ifdef _FILE_BIN_
//#define FILE_IMAGE_MSG			"./image/image_msg.bin"
//#define FILE_IMAGE_PATH			"./image/image_path_JP.bin"	//
//#define FILE_CHAT_MSG			"./image/chat_msg.bin"
//#define FILE_IMAGE_HARD_PATH	"./image/image_hard_path.bin"
//#define FILE_IMAGE_ITEM_PATH	"./image/image_item_path.bin"
//#define FILE_IMAGE_MUGONG_PATH	"./image/image_mugong_path.bin"
//#define FILE_IMAGE_ABILITY_PATH "./image/image_ability_path.bin"
//#define FILE_IMAGE_BUFF_PATH	"./image/image_Buff_path.bin"
//#define FILE_IMAGE_MINIMAP_PATH	"./image/image_Minimap_path.bin"
//#define FILE_IMAGE_JACKPOT_PATH	"./image/image_jackpot_path.bin"
//#define FILE_CHATTIP_MSG		"./image/ChatTipMsg.bin"

//#else
//#define FILE_IMAGE_MSG			"./image/image_msg.txt"
//#define FILE_IMAGE_PATH			"./image/image_path.txt"
//#define FILE_CHAT_MSG			"./image/chat_msg.txt"
//#define FILE_IMAGE_HARD_PATH	"./image/image_hard_path.txt"
//#define FILE_IMAGE_ITEM_PATH	"./image/image_item_path.txt"
//#define FILE_IMAGE_MUGONG_PATH	"./image/image_mugong_path.txt"
//#define FILE_IMAGE_ABILITY_PATH "./image/image_ability_path.txt"
//#define FILE_IMAGE_BUFF_PATH	"./image/image_Buff_path.txt"
//#define FILE_IMAGE_MINIMAP_PATH	"./image/image_Minimap_path.txt"
//#define FILE_IMAGE_JACKPOT_PATH	"./image/image_jackpot_path.txt"
//#define FILE_CHATTIP_MSG		"./image/ChatTipMsg.txt"

//#endif

//#else	//_JAPAN_LOCAL_

#ifdef _FILE_BIN_
#define FILE_IMAGE_MSG			"./image/image_msg.bin"
#define FILE_IMAGE_PATH			"./image/image_path.bin"
#define FILE_CHAT_MSG			"./image/chat_msg.bin"
#define FILE_IMAGE_HARD_PATH	"./image/image_hard_path.bin"
#define FILE_IMAGE_ITEM_PATH	"./image/image_item_path.bin"
#define FILE_IMAGE_MUGONG_PATH	"./image/image_mugong_path.bin"
#define FILE_IMAGE_ABILITY_PATH "./image/image_ability_path.bin"
#define FILE_IMAGE_BUFF_PATH	"./image/image_Buff_path.bin"
#define FILE_IMAGE_MINIMAP_PATH	"./image/image_Minimap_path.bin"
#define FILE_IMAGE_JACKPOT_PATH	"./image/image_jackpot_path.bin"
#define FILE_CHATTIP_MSG		"./image/ChatTipMsg.bin"

#else
#define FILE_IMAGE_MSG			"./image/image_msg.txt"
#define FILE_IMAGE_PATH			"./image/image_path.txt"
#define FILE_CHAT_MSG			"./image/chat_msg.txt"
#define FILE_IMAGE_HARD_PATH	"./image/image_hard_path.txt"
#define FILE_IMAGE_ITEM_PATH	"./image/image_item_path.txt"
#define FILE_IMAGE_MUGONG_PATH	"./image/image_mugong_path.txt"
#define FILE_IMAGE_ABILITY_PATH "./image/image_ability_path.txt"
#define FILE_IMAGE_BUFF_PATH	"./image/image_Buff_path.txt"
#define FILE_IMAGE_MINIMAP_PATH	"./image/image_Minimap_path.txt"
#define FILE_IMAGE_JACKPOT_PATH	"./image/image_jackpot_path.txt"
#define FILE_CHATTIP_MSG		"./image/ChatTipMsg.txt"

#endif
						
//#endif //_JAPAN_LOCAL_

#endif //_cRESOURCEHEADER_H_
//
