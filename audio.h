class Sound;

void audio_init(void);
void audio_play(int snd, int vol, int pan);
Sound *audio_loop(int snd, int vol, int pan);
void audio_stop(Sound *);
int audio_loadsound(const char *);
