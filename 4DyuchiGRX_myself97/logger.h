

//  파일 로거 초기화.
//  파일 이름에 널값을 주면 기본값인 log.txt로..
//int LogInit( char* FileName);

//int LogExit(void);
#define LogEnable

int LogInit(void);

int Log( char* Sentence);

int LogPrintf( char* szFormat, ...);

