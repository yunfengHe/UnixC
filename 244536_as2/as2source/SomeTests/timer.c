#include <stdio.h>
#include <time.h>

int main (void)
{
	time_t start = time(NULL);
	time_t current;
	while(1){
		printf("fuck me!\n");
		sleep(1);
		current = time(NULL);
		if ((current - start)>3)
		break;
	}
	return 0;
}


