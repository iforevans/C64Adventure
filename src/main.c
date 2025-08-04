#include <stdio.h>
#include <string.h>
#include <c64/kernalio.h>

/* Some useful defines */
#define FILE_NUM	1
#define DEVICE_NUM	8
#define CHANNEL_NUM	2

#define REC_LOC_START	"loc"
#define REC_LOC_END		"endloc"
#define REC_OBJ_START	"obj"
#define REC_OBJ_END		"endobj"

struct LOCATION
{
	unsigned char id;
	char short_desc[16];
	char long_desc[160];
	unsigned char exits[8];
};

int loadLocationRecord()
{
	printf("Found Loc rec");
	return 0;
}

int loadObjRecord()
{
	printf("Found Obj rec");
	return 0;
}

void stripTrailingCR(char *line, size_t lineLen) 
{
    if (lineLen > 0 && line[lineLen - 1] == '\r') 
	{
        line[lineLen - 1] = '\0';
    }
}

int loadGameData(void)
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
			else
			{
				/* We should not get here with a well formed gamedata file */
				printf(p"Bad token in Gamedata file at line %d. Expected %s or %s", lineCount, REC_LOC_START, REC_OBJ_START);
				break;
			}

			/* Read a line from the file */
			size_t lineLen = krnio_gets(FILE_NUM, line, sizeof(line));
		}

		/* Close the file */
		krnio_close(1);
	}

	return 0;
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


