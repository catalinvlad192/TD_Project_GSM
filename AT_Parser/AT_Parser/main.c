#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "AT_Parser.h"

/*
	Vlad Catalin-Andrei
	Salapa Ionut
*/

int main(int argc, char* argv[])
{
	if (argc == 2) {
		FILE* file = fopen(argv[1], "rb");

		while (!feof(file))
		{
			char ch = fgetc(file);
			const char* returnTypeString;
			EReturnType ret = parse(ch);

			returnTypeString = getStringFromEReturnType(ret);
			printf("Parse returned %s for %c\n", returnTypeString, ch);

			if (ret == EReturnType_Error)
			{
				printf("Wrong character received\n");
				break;
			}
			else if (ret == EReturnType_End)
			{
				//print data strings.
				printf("Print data\n");
				int i;
				for (i = 0; i < data.lineCount; i++)
				{
					printf("%s\n", data.str[i]);
				}
				break;
			}
		}
	}
	else {
		printf("Wrong number of args: %d\n",argc);
	}
	return 0;
}
