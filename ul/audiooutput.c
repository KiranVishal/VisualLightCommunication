#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>
#include <include/sndfile.h>

/******************************************************
Usage:
./ktimer [-l:#] [-s:time name]

-l (list)
-s (start timer)

 ******************************************************/
pthread_t thread_id;
int counter;
int pFile;
SNDFILE* infile;

void* thread_output(void* threadarg);

int main(int argc, char **argv) {
	int rc;
	int sleep_time = 1;
	char write_line[10];
	struct timespec timer_sleep, timer_rem;

	/* Check to see if the mytimer successfully has mknod run
	   Assumes that mytimer is tied to /dev/mytimer*/
	pFile = open("/dev/mygpio", O_RDWR);
	if (pFile <= 0) 
	{
		fputs("mygpio module isn't loaded\n",stderr);
		return -1;
	}


	rc = pthread_create(&thread_id, NULL, thread_output, NULL);
	if (rc)
	{
		printf("ERROR; trying to create thread\n");
		return -1;
	}

	pthread_join(thread_id, NULL);

	close(pFile);
	return 0;
}

void* thread_output(void* threadarg)
{
	char write_line[10];
	short data;
	int value;
	int i;
	struct timespec timer_sleep, timer_rem;
	SF_INFO sfinfo;
	
	if (! (infile = sf_open("input.wav", SFM_READ, &sfinfo)))
	{
		printf ("Can not open wave file\n");
		return NULL;
	}
	printf("Channels:%d, SampleRate:%d, Format:%d\n", 
			sfinfo.channels, 
			sfinfo.samplerate,
			sfinfo.format);

	timer_sleep.tv_sec = 0;
	timer_sleep.tv_nsec = 125000L;
	
	value = 0;

	while(true)
	{	
		sf_read_short(infile, &data, 1);
		value = (data >> 8) & 0xFF;
		sprintf(write_line, "%d", value);
		write(pFile, write_line, strlen(write_line));
		nanosleep(&timer_sleep, &timer_rem);
	}
}
