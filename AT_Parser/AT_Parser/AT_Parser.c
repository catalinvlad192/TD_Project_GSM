
#include "AT_Parser.h"
#include <string.h>
#include <stdio.h>

AT_Data data;
bool isNormalCommand = true;

const char* getStringFromEReturnType(EReturnType ret)
{
	static char buffer[25];
	if (ret == EReturnType_End)
		sprintf(buffer, "EReturnType_End");
	else if (ret == EReturnType_Error)
		sprintf(buffer, "EReturnType_Error");
	else if (ret == EReturnType_FeedMe)
		sprintf(buffer, "EReturnType_FeedMe");
	return buffer;
}

void setMemoryOfString(void)
{
	int i;
	data.lineCount = 0;
	for (i = 0; i < MAX_LINE_SIZE; i++)
	{
		memset(data.str[i], 0, sizeof(data.str[i]));
	}
}

EReturnType parse(uint8_t ch)
{
	static uint8_t state = 0;
	static uint32_t lineIndex = 0;
	static uint32_t stringIndex = 0;

	switch (state)
	{
		case 0:
		{
			if (ch == CR)
			{
				setMemoryOfString();
				state = 1;
				return EReturnType_FeedMe;
			}
			printf("Stuck in state 0 because no CR present\n");
			return EReturnType_FeedMe;
		}
		case 1:
		{
			if (ch == LF)
			{
				state = 2;
				return EReturnType_FeedMe;
			}
			else
			{
				state = 0;
				lineIndex = 0;
				data.lineCount = 0;
				stringIndex = 0;
				setMemoryOfString();
				printf("Error receiving LF in state 1\n");
				return EReturnType_Error;
			}
		}
		case 2:
		{
			if (ch == 'O')
			{
				state = 3;
			}
			else if (ch == 'E')
			{
				state = 7;
			}
			else if (ch == '+' && isNormalCommand)
			{
				state = 12;
			}
			else if ((ch !=CR && ch !=LF) && !isNormalCommand)
			{
				state = 13;
				if (lineIndex < MAX_LINE_SIZE) {
					if (stringIndex < MAX_STR_SIZE - 1) {
						data.str[lineIndex][stringIndex++] = ch;
					}
					else if (stringIndex == MAX_STR_SIZE - 1)
					{
						data.str[lineIndex][stringIndex] = '\0';
						stringIndex++;
					}
				}
			}
			else
			{
				state = 0;
				lineIndex = 0;
				data.lineCount = 0;
				stringIndex = 0;
				setMemoryOfString();
				printf("Error receiving O, E or + in state 2\n");
				return EReturnType_Error;
			}
			return EReturnType_FeedMe;
		}
		case 3:
		{
			if (ch == 'K')
			{
				state = 4;
				data.ok = true;
				return EReturnType_FeedMe;
			}
			else
			{
				state = 0;
				lineIndex = 0;
				data.lineCount = 0;
				stringIndex = 0;
				setMemoryOfString();
				printf("Error receiving K in state 3\n");
				return EReturnType_Error;
			}
		}
		case 4:
		{
			if (ch == CR)
			{
				state = 5;
				return EReturnType_FeedMe;
			}
			else
			{
				state = 0;
				lineIndex = 0;
				data.lineCount = 0;
				stringIndex = 0;
				setMemoryOfString();
				printf("Error receiving CR in state 4\n");
				return EReturnType_Error;
			}
		}
		case 5:
		{
			if (ch == LF)
			{
				state = 0;
				lineIndex = 0;
				stringIndex = 0;
				return EReturnType_End;
			}
			else
			{
				state = 0;
				lineIndex = 0;
				data.lineCount = 0;
				stringIndex = 0;
				setMemoryOfString();
				printf("Error receiving LF in state 5\n");
				return EReturnType_Error;
			}
		}
		case 7:
		{
			if (ch == 'R')
			{
				state = 8;
				return EReturnType_FeedMe;
			}
			else
			{
				state = 0;
				lineIndex = 0;
				data.lineCount = 0;
				stringIndex = 0;
				setMemoryOfString();
				printf("Error receiving R in state 7\n");
				return EReturnType_Error;
			}
		}
		case 8:
		{
			if (ch == 'R')
			{
				state = 9;
				return EReturnType_FeedMe;
			}
			else
			{
				state = 0;
				lineIndex = 0;
				data.lineCount = 0;
				stringIndex = 0;
				setMemoryOfString();
				printf("Error receiving R in state 8\n");
				return EReturnType_Error;
			}
		}
		case 9:
		{
			if (ch == 'O')
			{
				state = 10;
				return EReturnType_FeedMe;
			}
			else
			{
				state = 0;
				lineIndex = 0;
				data.lineCount = 0;
				stringIndex = 0;
				setMemoryOfString();
				printf("Error receiving O in state 9\n");
				return EReturnType_Error;
			}
		}
		case 10:
		{
			if (ch == 'R')
			{
				state = 11;
				data.ok = false;
				return EReturnType_FeedMe;
			}
			else
			{
				state = 0;
				lineIndex = 0;
				data.lineCount = 0;
				stringIndex = 0;
				setMemoryOfString();
				printf("Error receiving R in state 10\n");
				return EReturnType_Error;
			}
		}
		case 11:
		{
			if (ch == CR)
			{
				state = 5;
				return EReturnType_FeedMe;
			}
			else
			{
				state = 0;
				lineIndex = 0;
				data.lineCount = 0;
				stringIndex = 0;
				setMemoryOfString();
				printf("Error receiving CR in state 11\n");
				return EReturnType_Error;
			}
		}
		case 12:
		{
			if (ch != CR && ch != LF)
			{
				state = 13;
				if (lineIndex < MAX_LINE_SIZE) {
					if (stringIndex < MAX_STR_SIZE - 1) {
						data.str[lineIndex][stringIndex++] = ch;
					}
					else if (stringIndex == MAX_STR_SIZE - 1)
					{
						data.str[lineIndex][stringIndex] = '\0';
						stringIndex++;
					}
				}
				return EReturnType_FeedMe;
			}
			else
			{
				state = 0;
				lineIndex = 0;
				data.lineCount = 0;
				stringIndex = 0;
				setMemoryOfString();
				printf("Error receiving CR or LF instead of any other char in state 12\n");
				return EReturnType_Error;
			}
		}
		case 13:
		{
			if (ch != CR && ch != LF)
			{
				state = 13;
				if (lineIndex < MAX_LINE_SIZE) {
					if (stringIndex < MAX_STR_SIZE - 1) {
						data.str[lineIndex][stringIndex++] = ch;
					}
					else if (stringIndex == MAX_STR_SIZE - 1)
					{
						data.str[lineIndex][stringIndex] = '\0';
					}
				}
			}
			else if (ch == CR)
			{
				state = 14;

				if (lineIndex < MAX_LINE_SIZE) {
					data.str[lineIndex][stringIndex] = '\0';
					stringIndex = 0;
					lineIndex++;
					data.lineCount++;
				}
			}
			else
			{
				state = 0;
				lineIndex = 0;
				data.lineCount = 0;
				stringIndex = 0;
				setMemoryOfString();
				printf("Error! Received LF in state 13\n");
				return EReturnType_Error;
			}
			return EReturnType_FeedMe;
		}
		case 14:
		{
			if (ch == LF)
			{
				state = 15;
				return EReturnType_FeedMe;
			}
			else
			{
				state = 0;
				lineIndex = 0;
				data.lineCount = 0;
				stringIndex = 0;
				setMemoryOfString();
				printf("Error receiving LF in state 14\n");
				return EReturnType_Error;
			}
		}
		case 15:
		{
			if (ch == '+' && isNormalCommand)
			{
				state = 12;
			}
			else if (ch == CR)
			{
				state = 16;
			}
			else
			{
				state = 0;
				lineIndex = 0;
				data.lineCount = 0;
				stringIndex = 0;
				setMemoryOfString();
				printf("Error receiving CR or + in state 15\n");
				return EReturnType_Error;
			}
			return EReturnType_FeedMe;
		}
		case 16:
		{
			if (ch == LF)
			{
				state = 17;
				return EReturnType_FeedMe;
			}
			else
			{
				state = 0;
				lineIndex = 0;
				data.lineCount = 0;
				stringIndex = 0;
				setMemoryOfString();
				printf("Error receiving LF in state 16\n");
				return EReturnType_Error;
			}
		}
		case 17:
		{
			if (ch == 'E')
			{
				state = 7;
			}
			if (ch == 'O')
			{
				state = 3;
			}
			else
			{
				state = 0;
				lineIndex = 0;
				data.lineCount = 0;
				stringIndex = 0;
				setMemoryOfString();
				printf("Error receiving E or O in state 17\n");
				return EReturnType_Error;
			}
			return EReturnType_FeedMe;
		}
	}
	printf("Should not return here\n");
	return EReturnType_FeedMe;
}
