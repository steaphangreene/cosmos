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
  static long mix_buf_l[4096];
  static long mix_buf_r[4096];
  assert(len <= 8192);

  if(play_blocks == NULL)  {
    memset(stream, 0, len);
    return;
    }

  Sound **sptr = &play_blocks;
  memset(mix_buf_l, 0, len*2);
  memset(mix_buf_r, 0, len*2);
  while((*sptr) != NULL) {
    if((*sptr)->flags & SOUND_TERMINATE) {
      Sound *it = (*sptr);
      (*sptr) = (*sptr)->next;
      it->next = free_blocks;
      free_blocks = it;
      continue;
      }
    for(int ctr=0; ctr<(len>>1) && (*sptr)->pos+ctr < (*sptr)->length; ctr+=2) {
      long val = Sint8((*sptr)->data[(*sptr)->pos+ctr+1]);
      val <<= 8;
      val |= (*sptr)->data[(*sptr)->pos+ctr+0] & 0xFF;
      mix_buf_l[ctr>>1] += val * (*sptr)->vol * (16-(*sptr)->pan);
      mix_buf_r[ctr>>1] += val * (*sptr)->vol * (16+(*sptr)->pan);
      }
    (*sptr)->pos += len>>1;
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
  for(int ctr=0; ctr<len; ctr+=4) {
    if(mix_buf_l[ctr>>2] > SHRT_MAX<<8) mix_buf_l[ctr>>2] = SHRT_MAX<<8;
    if(mix_buf_l[ctr>>2] < SHRT_MIN<<8) mix_buf_l[ctr>>2] = SHRT_MIN<<8;
    if(mix_buf_r[ctr>>2] > SHRT_MAX<<8) mix_buf_r[ctr>>2] = SHRT_MAX<<8;
    if(mix_buf_r[ctr>>2] < SHRT_MIN<<8) mix_buf_r[ctr>>2] = SHRT_MIN<<8;
    stream[ctr+0] = (mix_buf_l[ctr>>2] >> 8);
    stream[ctr+1] = (mix_buf_l[ctr>>2] >> 16);
    stream[ctr+2] = (mix_buf_r[ctr>>2] >> 8);
    stream[ctr+3] = (mix_buf_r[ctr>>2] >> 16);
    }
  }

int audio_loadsound(const char *fn) {
  if(!audio_initialized) return 0;
  SDL_AudioSpec spec;
  if(!SDL_LoadWAV(fn, &spec, &SFX[num_sounds].data, &SFX[num_sounds].length)) {
    fprintf(stderr, "Error loading \"%s\"!\n", fn);
    exit(1);
    }
  ++num_sounds;
  return num_sounds;
  }

Sound *get_block() {
  Sound *s;
  if(free_blocks) { s = free_blocks; free_blocks = s->next; }
  else { s = new Sound; }
  return s;
  }

void audio_play(int snd, int vol, int pan) {
  if(!audio_initialized) return;
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
  if(!audio_initialized) return NULL;
  Sound *s = get_block();
  *s = SFX[snd-1];
  s->pos = 0;
  s->vol = vol;
  s->pan = pan;
  s->flags = SOUND_LOOP;
  s->next = play_blocks;
  play_blocks = s;
  return s;
  }

void audio_stop(Sound *s) {
  if(!audio_initialized) return;
  if(s) s->flags |= SOUND_TERMINATE;
  }

void audio_init() {
  audio.freq = 22050;
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
