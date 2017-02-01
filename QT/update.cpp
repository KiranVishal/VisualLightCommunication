#include "update.h"

void* thread_play(void* thread_arg)
{ 
	char write_line[10];
	short data;
	int value;
	int success_read;
	int* play_track = (int*)thread_arg;
	struct timespec timer_wake, timer_now;
	SNDFILE* infile;
	SF_INFO sfinfo;
	/* Check to see if the mytimer successfully has mknod run
	 *        Assumes that mytimer is tied to /dev/mytimer*/
	int pFile;

	printf("Thread Started playing:%d\n", *play_track);

	if (*play_track == 0)
	{
		return NULL;
	}
	printf("trying to open device\n");
	pFile = open("/dev/mygpio", O_RDWR);
	if (pFile <= 0)
	{
		printf("file pointer:%d\n", pFile);
		return NULL;
	}
	printf("trying to open wav file\n");
	if (*play_track == 1)
	{
		printf("trying to open wav file 1\n");
		if (! (infile = sf_open("/media/card/wav/input.wav", SFM_READ, &sfinfo)))
		{
			printf ("Can not open wave file\n");
			return NULL;
		}
	}
	else
	{
		if (*play_track == 2)
		{
			if (! (infile = sf_open("/media/card/wav/input2.wav", SFM_READ, &sfinfo)))
			{
				printf ("Can not open wave file\n");
				return NULL;
			}
		}
		else
		{
			if (! (infile = sf_open("/media/card/wav/input3.wav", SFM_READ, &sfinfo)))
			{
				printf ("Can not open wave file\n");
				return NULL;
			}
		}
	}
	printf("file opened\n");
	printf("Channels:%d, SampleRate:%d, Format:%d\n",
			sfinfo.channels,
			sfinfo.samplerate,
			sfinfo.format);

	//timer_sleep.tv_sec = 0;
	//timer_sleep.tv_nsec = 125000L;
	timer_wake.tv_sec = 0;
	timer_wake.tv_nsec = 0;

	value = 0;

	int counter = 0;
	while(*play_track != 0)
	{
		counter = 0;
		clock_gettime(CLOCK_MONOTONIC, &timer_now);
		timer_wake.tv_sec = timer_now.tv_sec;
		timer_wake.tv_nsec = timer_now.tv_nsec + 125000L; 
		if (timer_wake.tv_nsec < timer_now.tv_nsec)
			timer_wake.tv_sec++;
		success_read = sf_read_short(infile, &data, 1);
		if (success_read == 0)
			sf_seek (infile, 0, SEEK_SET);
		value = (data >> 8) & 0xFF;
		sprintf(write_line, "%d", value);
		write(pFile, write_line, strlen(write_line));
		while (timer_now.tv_sec <= timer_wake.tv_sec && 
				timer_now.tv_nsec < timer_wake.tv_nsec)
		{
			clock_gettime(CLOCK_MONOTONIC, &timer_now);
		}
	}

	sf_close(infile);

	write(pFile, "0", 1);

	close(pFile);

	return NULL;
}

void UpdateInfo::updateUI()
{
	char start_stop[] = {"-----------------------------------------------------------------------"};

	if (playing >= 1)
	{
		int before = arrow_pos - 1;
		before = (before < 0) ? strlen(start_stop)-1: before;
		int after = (arrow_pos + 1) % strlen(start_stop);

		start_stop[before] = '>';
		start_stop[arrow_pos] = '>';
		start_stop[after] = '>';
		arrow_pos = (arrow_pos + 1) % strlen(start_stop);
	}	
	else
		arrow_pos = 0;

	le_state->setText(start_stop);
}

void UpdateInfo::playTrack1()
{
	printf("playTrack1 playing:%d\n", playing);
	track_name->setText("Now Playing: Track 1");
	if (playing != 0)
	{
		playing = 0;
		pthread_join(play, NULL);
	}
	playing = 1;
	pthread_create(&play, NULL, &thread_play, &playing); 
}

void UpdateInfo::playTrack2()
{
	printf("playTrack2 playing:%d\n", playing);
	track_name->setText("Now Playing: Track 2");
	if (playing != 0)
	{
		playing = 0;
		pthread_join(play, NULL);
	}
	playing = 2;
	pthread_create(&play, NULL, &thread_play, &playing); 
}

void UpdateInfo::playTrack3()
{
	printf("playTrack3 playing:%d\n", playing);
	track_name->setText("Now Playing: Track 3");
	if (playing != 0)
	{
		playing = 0;
		pthread_join(play, NULL);
	}
	playing = 3;
	pthread_create(&play, NULL, &thread_play, &playing); 
}

void UpdateInfo::stopPlayback()
{
	printf("stopPlayback playing:%d\n", playing);
	track_name->setText("Stopped Playback");
	if (playing != 0)
	{
		playing = 0;
		pthread_join(play, NULL);
	}
}
