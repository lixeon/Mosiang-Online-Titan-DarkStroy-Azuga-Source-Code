// line_node.cpp: implementation of the line_node class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "line_node.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void line_node::release_line_node(line_node ** top)
{
	line_node * curLineNode = *top;
	line_node * tmpNode = NULL;
	while(curLineNode)
	{
		tmpNode = curLineNode;
		curLineNode = curLineNode->nextLine;
		if(tmpNode)
		{
			delete tmpNode;
			tmpNode = NULL;
		}
	}
	*top = NULL;
}

void line_node::set_line_node(char * text, line_node ** top)
{
	line_node::release_line_node(top);

	if(*text == 0 || top == NULL) return;
	
	char * sp = text;
	line_node * curLineNode = (*top) = new line_node;
	curLineNode->nextLine = NULL;
	curLineNode->prevLine = NULL;
	char * cur_line = curLineNode->line;
	while(*sp != 0)
	{
		if(*sp & 0x80)
		{
			*cur_line++ = *sp++;
			*cur_line++ = *sp++;
		}
		else
		{
			switch(*sp)
			{
			case TEXT_DELIMITER:
				{
					++sp;
					char tmp = *sp;
					switch(tmp)
					{
					case TEXT_NEWLINECHAR:	// new line
						{
							*cur_line = 0;
							curLineNode->len = strlen(curLineNode->line);
							curLineNode->nextLine = new line_node;
							curLineNode->prevLine = curLineNode;
							curLineNode = curLineNode->nextLine;
							curLineNode->nextLine = NULL;
							curLineNode->prevLine = NULL;
							cur_line = curLineNode->line;
						}
						break;
					case TEXT_TABCHAR:
						{
						}
						break;
					case TEXT_SPACECHAR:
						{	
							*cur_line = ' ';
							++cur_line;
						}
						break;
					}// - switch()
				}
				break;
			default:
				{
					*cur_line = *sp;
					++cur_line;
				}
			}//- switch()
			++sp;
		}
	}
	curLineNode->len = strlen(curLineNode->line);
}