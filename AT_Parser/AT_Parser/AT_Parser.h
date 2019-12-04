
#ifndef _AT_PARSER_H_
#define _AT_PARSER_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define MAX_LINE_SIZE 1024
#define MAX_STR_SIZE 256

#define CR 0x0D
#define LF 0x0A

typedef enum
{
	EReturnType_FeedMe = 0,
	EReturnType_Error = 1,
	EReturnType_End = 2
}EReturnType;

typedef struct
{
	bool ok;
	uint32_t lineCount;
	char str[MAX_LINE_SIZE][MAX_STR_SIZE + 1];
}AT_Data;

extern AT_Data data;
extern bool isNormalCommand;

EReturnType parse(uint8_t ch);
const char* getStringFromEReturnType(EReturnType ret);

#endif
