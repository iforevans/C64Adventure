#include <stdio.h>
#include <c64/kernalio.h>

int main(void)
{
	char line[80];

	// Set name for file and open it on drive 9
	krnio_setnam("GAMEDATA,S,R");	
	if (krnio_open(1, 8, 2))
	{
		while (krnio_gets(1, line, sizeof(line)))
		{
			printf("%s\n", line);
		}

		// Close the file
		krnio_close(1);
	}

	return 0;
}
