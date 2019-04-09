// line_node.h: interface for the line_node class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINE_NODE_H__4B08F8FA_73F2_4C0B_9FA1_4EF1C8FF2F82__INCLUDED_)
#define AFX_LINE_NODE_H__4B08F8FA_73F2_4C0B_9FA1_4EF1C8FF2F82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define LIN_NODE_TEXT_SIZE	255

class line_node
{
public:
	line_node(){	
		nextLine = NULL;
		prevLine = NULL;
		len = 0;
		line = new char[LIN_NODE_TEXT_SIZE];
		memset(line, 0, LIN_NODE_TEXT_SIZE);
	}
	line_node(int size){
		nextLine = NULL;
		prevLine = NULL;
		len = 0;
		line = new char[size];
		memset(line, 0, size);
	}
	~line_node()
	{ 
		delete line; 
		line = NULL; 
		nextLine = NULL;
		prevLine = NULL;
	}
	
	char * curposition;
	char * line;
	DWORD len;
	line_node * nextLine;
	line_node * prevLine;
	static void set_line_node(char * text, line_node ** top);
	static void release_line_node(line_node ** top);
};

#endif // !defined(AFX_LINE_NODE_H__4B08F8FA_73F2_4C0B_9FA1_4EF1C8FF2F82__INCLUDED_)
