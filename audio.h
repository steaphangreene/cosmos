class Sound;

void audio_init(void);
void audio_play(int snd, int vol, int pan);
Sound *audio_loop(int snd, int vol, int pan);
void audio_stop(Sound *);
void audio_setvol(Sound *, int);
void audio_setpan(Sound *, int);
int audio_buildsound(const unsigned char *, unsigned long);
int audio_loadsound(const char *);
int audio_loadmusic(const char *);
