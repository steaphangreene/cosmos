#include "player.h"

int local_player;

Player::Player() {
  }

void Player::TakeTurn() {
  }

Player::Player(FILE *f) {
  LoadFrom(f);
  }

void Player::SaveTo(FILE *f) {
  fprintf(f, "%d\n", color);
  }

void Player::LoadFrom(FILE *f) {
  fscanf(f, "%d\n", &color);
  }

