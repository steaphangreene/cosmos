#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <limits.h>

#include <assert.h>

using namespace std;

#include <SDL.h>

SDL_AudioSpec audio;

static int audio_initialized = 0;

#define SOUND_LOOP	1
#define SOUND_TERMINATE	2
#define SOUND_STEREO	4

struct Sound {
  Uint32 pos;
  Uint32 vol;
  Uint32 pan;
  Uint8 *data;
  Uint32 length;
  Uint32 flags;
  Sound *next;
  };

Sound SFX[128];
int num_sounds = 0;
Sound *free_blocks = NULL;
Sound *play_blocks = NULL;

void audio_callback(void *userdata, Uint8 *stream, int len) {

  if(play_blocks == NULL)  {
    memset(stream, 0, len);
    return;
    }

  for(int ctr=0; ctr<len; ctr+=4) {
    long buf_l = 0, buf_r = 0;
    Sound **sptr = &play_blocks;
    while((*sptr) != NULL) {
      if((*sptr)->flags & SOUND_TERMINATE) {
	Sound *it = (*sptr);
	(*sptr) = (*sptr)->next;
	it->next = free_blocks;
	free_blocks = it;
	continue;
	}
      if((*sptr)->flags & SOUND_STEREO) { // Stereo Source
	if((*sptr)->pos+ctr >= (*sptr)->length) {
	  sptr = &((*sptr)->next);
	  continue;
	  }
	long val_l = Sint8((*sptr)->data[(*sptr)->pos+ctr+1]);
	val_l <<= 8;
	val_l |= (*sptr)->data[(*sptr)->pos+ctr+0] & 0xFF;
	buf_l += val_l * (*sptr)->vol * 16;
	long val_r = Sint8((*sptr)->data[(*sptr)->pos+ctr+3]);
	val_r <<= 8;
	val_r |= (*sptr)->data[(*sptr)->pos+ctr+2] & 0xFF;
	buf_r += val_r * (*sptr)->vol * 16;
	}
      else { // Mono Source with software panning
	if((*sptr)->pos+(ctr>>1) >= (*sptr)->length) {
	  sptr = &((*sptr)->next);
	  continue;
	  }
	long val = Sint8((*sptr)->data[(*sptr)->pos+(ctr>>1)+1]);
	val <<= 8;
	val |= (*sptr)->data[(*sptr)->pos+(ctr>>1)+0] & 0xFF;
	buf_l += val * (*sptr)->vol * (16-(*sptr)->pan);
	buf_r += val * (*sptr)->vol * (16+(*sptr)->pan);
	}

      sptr = &((*sptr)->next);
      }
    if(buf_l > SHRT_MAX<<8) buf_l = SHRT_MAX<<8;
    if(buf_l < SHRT_MIN<<8) buf_l = SHRT_MIN<<8;
    if(buf_r > SHRT_MAX<<8) buf_r = SHRT_MAX<<8;
    if(buf_r < SHRT_MIN<<8) buf_r = SHRT_MIN<<8;
    stream[ctr+0] = (buf_l >> 8);
    stream[ctr+1] = (buf_l >> 16);
    stream[ctr+2] = (buf_r >> 8);
    stream[ctr+3] = (buf_r >> 16);
    }

  Sound **sptr = &play_blocks;
  while((*sptr) != NULL) {
    if((*sptr)->flags & SOUND_STEREO) (*sptr)->pos += len;
    else (*sptr)->pos += len>>1;
    if((*sptr)->pos >= (*sptr)->length) {
      if((*sptr)->flags & SOUND_LOOP) (*sptr)->pos = 0;
      else {
	Sound *it = (*sptr);
	(*sptr) = (*sptr)->next;
	it->next = free_blocks;
	free_blocks = it;
	continue;
	}
      }
    sptr = &((*sptr)->next);
    }
  }

int audio_buildsound(const unsigned char *data, unsigned long len) {
  if(!audio_initialized) return 0;
  SFX[num_sounds].data = (unsigned char *)data;
  SFX[num_sounds].length = len;
  ++num_sounds;
  return num_sounds;
  }

int audio_loadsound(const char *fn) {
  if(!audio_initialized) return 0;
  SDL_AudioSpec spec;
  if(!SDL_LoadWAV(fn, &spec, &SFX[num_sounds].data, &SFX[num_sounds].length)) {
    fprintf(stderr, "Warning: Can't load \"%s\"!\n", fn);
    return 0;
    }
  ++num_sounds;
  return num_sounds;
  }

int audio_loadmusic(const char *fn) {
  int ret = audio_loadsound(fn);
  SFX[ret-1].flags |= SOUND_STEREO;
  return ret;
  }

Sound *get_block() {
  Sound *s;
  if(free_blocks) { s = free_blocks; free_blocks = s->next; }
  else { s = new Sound; }
  return s;
  }

void audio_play(int snd, int vol, int pan) {
  if((!audio_initialized) || snd < 1) return;
  Sound *s = get_block();
  *s = SFX[snd-1];
  s->pos = 0;
  s->vol = vol;
  s->pan = pan;
  s->flags = 0;
  s->next = play_blocks;
  play_blocks = s;
  }

Sound *audio_loop(int snd, int vol, int pan) {
  if((!audio_initialized) || snd < 1) return NULL;
  Sound *s = get_block();
  *s = SFX[snd-1];
  s->pos = 0;
  s->vol = vol;
  s->pan = pan;
  s->flags |= SOUND_LOOP;
  s->next = play_blocks;
  play_blocks = s;
  return s;
  }

void audio_stop(Sound *s) {
  if(!audio_initialized) return;
  if(s) s->flags |= SOUND_TERMINATE;
  }

void audio_init() {
  audio.freq = 44100;
  audio.format = AUDIO_S16;
  audio.samples = 2048;
  audio.channels = 2;
  audio.callback = audio_callback;
  audio.userdata = NULL;
  if(SDL_OpenAudio(&audio, NULL) < 0) {
    fprintf(stderr, "Error initializing SDL Audio!\n");
    fprintf(stderr, "Continuing without sound.\n");
    return;
    }
  SDL_PauseAudio(0);
  audio_initialized = 1;
  }
