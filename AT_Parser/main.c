#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "AT_Parser.h"

int main(int argc, char* argv[])
{
	FILE* file = fopen("test_file_simple_at_ok.txt", "rb");

	while (!feof(file))
	{
		char ch = fgetc(file);
		const char* returnTypeString;
		EReturnType ret = parse(ch);

		returnTypeString = getStringFromEReturnType(ret);
		printf("Parse returned %s for %c\n", returnTypeString, ch);

		if (ret == EReturnType_Error || ret == EReturnType_End)
		{
			break;
		}
	}
	return 0;
}