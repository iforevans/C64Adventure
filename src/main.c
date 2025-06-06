#include <stdio.h>
#include <string.h>
#include <c64/kernalio.h>

/* Some useful defines */
#define FILE_NUM	1
#define DEVICE_NUM	8
#define CHANNEL_NUM	2

#define REC_LOC		"loc"
#define REC_OBJ		"obj"

struct LOCATION
{
	unsigned char id;
	char short_desc[16];
	char long_desc[160];
	unsigned char exits[8];
};

int loadLocationRecord()
{
	printf ("Found Loc rec");
	return 0;
}

int loadObjRecord()
{
	printf ("Found Obj rec");
	return 0;
}

int loadGameData(void)
{
	char oneLine[80];
	int lineCount = 0;

	// Set name for file and open it on drive 9
	krnio_setnam("GAMEDATA,S,R");	
	if (krnio_open(FILE_NUM, DEVICE_NUM, CHANNEL_NUM))
	{
		while (krnio_gets(FILE_NUM, oneLine, sizeof(oneLine)))
		{
			lineCount++;
			if (strcmp(REC_LOC, oneLine) == 0)
			{
				loadLocationRecord();
			}
			else if (strcmp(REC_OBJ, oneLine) == 0)
			{
				loadObjRecord();
			}
			else
			{
				/* We should not get here with a well formed gamedata file */
				printf("Bad token in Gamedata file at line %d. Expected %s or %s", REC_LOC, REC_OBJ);
				break;
			}
			
			printf("%s\n", oneLine);
		}

		// Close the file
		krnio_close(1);
	}

	return 0;
}

int main(void)
{

	return 0;
}


