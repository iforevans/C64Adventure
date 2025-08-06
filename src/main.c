#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>
#include <c64/kernalio.h>

/* Some useful defines */
#define FILE_NUM	1
#define DEVICE_NUM	8
#define CHANNEL_NUM	2
#define MAX_LOCS	100

#define REC_LOC_START	"LOC"
#define REC_LOC_END		"ENDLOC"
#define REC_OBJ_START	"OBJ"
#define REC_OBJ_END		"ENDOBJ"
#define GAMEDATA_END	"END"

typedef struct Location
{
	unsigned char id;
	char short_desc[16];
	char long_desc[160];
	unsigned char exits[8];
	struct Location *next;
} Location;

void stripTrailingCR(char *line, size_t lineLen) 
{
    if (lineLen > 0 && line[lineLen - 1] == '\r') 
	{
        line[lineLen - 1] = '\0';
    }
}

bool parse_int(const char *s, int *out) 
{
	/* Base 10; skips leading space, handles +/-. */
    char *end;
    long v = strtol(s, &end, 10);            

	/* Any digits? */
    if (s == end) return false;

	/* Iutside valid range for an int? */
    if (v < INT_MIN || v > INT_MAX) return false;

	/* Anything other than trailing whitespace is not valid */
    while (isspace((unsigned char)*end)) end++;
    if (*end != '\0') return false;

    *out = (unsigned char)v;
    return true;
}


bool loadLocationRecord()
{
	/* DEBUG Info */
	printf(p"Found Loc rec\n");

	/* 1st Line is the location number */
	char line[80];
	size_t lineLen = krnio_gets(FILE_NUM, line, sizeof(line));
 	int loc_num;
    if (!parse_int(line, &loc_num)) 
	{
        printf(p"Error: '%s' is not a valid int.\n", line);
        return false;
    }
	else
	{
		printf(p"Location number (%d) found.\n", loc_num);
	}
	/* 2nd Line is the 'exits' line */
	/* 3rd Line is the short desc */
	/* 4th Line is the long desc */

	while (lineLen > 0)
	{
		stripTrailingCR(line, lineLen);
		if (strcmp(line, REC_LOC_END) == 0)
		{
			/* We're done*/
			break;
		}

		/* Just print the line for now */
		printf("%s\n", line);

		/* Read the next line from the file */
		lineLen = krnio_gets(FILE_NUM, line, sizeof(line));
	}

	return true;
}

bool loadObjRecord()
{
	printf(p"Found Obj rec\n");

	char line[80];
	size_t lineLen = krnio_gets(FILE_NUM, line, sizeof(line));
	while (lineLen > 0)
	{
		stripTrailingCR(line, lineLen);
		if (strcmp(line, REC_OBJ_END) == 0)
		{
			/* We're done*/
			break;
		}

		/* Just print the line for now */
		printf("%s\n", line);

		/* Read the next line from the file */
		lineLen = krnio_gets(FILE_NUM, line, sizeof(line));
	}

	return true;
}

bool loadGameData(void)
{
	/* Set name for file and open it on drive 9 */
	krnio_setnam("GAMEDATA,S,R");	
	if (krnio_open(FILE_NUM, DEVICE_NUM, CHANNEL_NUM))
	{
		char line[80];
		int lineCount = 0;

		/* Read a line from the file */
		size_t lineLen = krnio_gets(FILE_NUM, line, sizeof(line));
		while (lineLen > 0)
		{
			stripTrailingCR(line, lineLen);
			if (strcmp(line, REC_LOC_START) == 0)
			{
				loadLocationRecord();
			}
			else if (strcmp(line, REC_OBJ_START) == 0)
			{
				loadObjRecord();
			}
			else if (strcmp(line, GAMEDATA_END) == 0)
			{
				/* All done */
				break;
			}
			else
			{
				/* We should not get here with a well formed gamedata file */
				printf(p"Bad token in Gamedata file at line %d. Expected %s or %s", lineCount, REC_LOC_START, REC_OBJ_START);
				return false;
			}

			/* Read a line from the file */
			lineLen = krnio_gets(FILE_NUM, line, sizeof(line));
		}

		/* Close the file */
		krnio_close(1);
	}

	/* all good **/
	return true;
}

void set_uppercase(void) 
{
    *(unsigned char *)0xD018 &= ~0x02;
}

void set_lowercase(void) 
{
    *(unsigned char *)0xD018 |= 0x02;
}

int main(void)
{
	/* Switch to the alternate character set */
	set_lowercase();
	printf(p"This is a test string\n");

	loadGameData();
	return 0;
}


