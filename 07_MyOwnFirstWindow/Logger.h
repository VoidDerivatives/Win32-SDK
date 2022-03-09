#ifndef		LOGGER_HEADER
	#define		 LOGGER_HEADER

#define		ON		 1
#define		OFF		 0

#define		LOG		ON

#define		LOG_WINMAIN		LOG
#define		LOG_WINPROC		LOG

void CreateLog(FILE **);
void DestroyLog(FILE **);

#endif     // LOGGER_HEADER

