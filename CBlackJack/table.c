#include "table.h"
#include <stdlib.h>
#include <stdio.h>

#include "card.h"
#include "cardpile.h"
#include "dealer.h"
#include "player.h"
#include "vector.h"

Table* Table__new(int num_players, int num_decks, int bet_size, int min_cards,
                  int verbose) {
  Table* t = malloc(sizeof(Table));
  t->m_card_pile = CardPile__new(num_decks);
  t->m_verbose = verbose;
  t->m_bet_size = bet_size;
  t->m_num_decks = num_decks;
  t->m_min_cards = min_cards;
  t->m_casino_earnings = 0;
  t->m_current_player = 0;
  t->m_running_count = 0;
  t->m_true_count = 0;

  t->m_players = Vector__new(num_players);
  for (int i = 0; i < num_players; i++) {
    Vector__push(t->m_players, Player__new(t, NULL));
  }
  t->m_dealer = Dealer__new();

  return t;
}

void Table__deal_round(Table* self) {
  for (int i = 0; i < self->m_players->size; i++) {
    Table__deal(self);
    self->m_current_player++;
  }
  self->m_current_player = 0;
}

void Table__evaluate_all(Table* self) {
  for (int i = 0; i < self->m_players->size; i++) {
    Player__evaluate(self->m_players->items[i]);
  }
}

void Table__deal(Table* self) {
  Card* card = Vector__last(self->m_card_pile->m_cards);
  Vector__push(
      ((Player*)self->m_players->items[self->m_current_player])->m_hand, card);
  self->m_running_count += card->m_count;
  Vector__pop(self->m_card_pile->m_cards);
}

void Table__pre_deal(Table* self) {
  for (int i = 0; i < self->m_players->size; i++) {
    Table__select_bet(self, self->m_players->items[i]);
  }
}

void Table__select_bet(Table* self, Player* player) {
  if (self->m_true_count >= 2) {
    player->m_initial_bet = self->m_bet_size * (self->m_true_count - 1);
  }
}

void Table__deal_dealer(Table* self, bool face_down) {
  Card* card = Vector__last(self->m_card_pile->m_cards);
  card->m_face_down = face_down;
  Vector__push(self->m_dealer->m_hand, card);
  if (!face_down) {
    self->m_running_count += card->m_count;
  }
  Vector__pop(self->m_card_pile->m_cards);
}

void Table__start_round(Table* self) {
    Table__clear(self);
    Table__update_count(self);
    if(self->m_verbose) {
        printf("%d cards left\n", self->m_card_pile->m_cards->size);
        printf("Running count is: %d\tTrue count is: %d\n", self->m_running_count, self->m_true_count);
    }

    Table__get_new_cards(self);
    Table__pre_deal(self);
    Table__deal_round(self);
    Table__deal_dealer(self, false);
    Table__deal_round(self);
    Table__deal_dealer(self, true);
    Table__evaluate_all(self);
    self->m_current_player = 0;
    if(Table__check_dealer_natural(self)) {
        Table__finish_round(self);
    } else {
        Table__check_player_natural(self);
        if(self->m_verbose) Table__print(self);
        Table__auto_play(self);
    }

}

void Table__get_new_cards(Table* self) {}
void Table__clear(Table* self) {}
void Table__update_count(Table* self) {}
void Table__hit(Table* self) {}
void Table__stand(Table* self){}
void Table__split(Table* self){}
void Table__split_aces(Table* self){}
void Table__double_bet(Table* self){}
void Table__auto_play(Table* self){}
void Table__action(Table* self, char* action){}
void Table__dealer_play(Table* self){}
void Table__next_player(Table* self){}
void Table__check_player_natural(Table* self){}
bool Table__check_dealer_natural(Table* self){return false;}
void Table__check_earnings(Table* self){}
void Table__finish_round(Table* self){}

void Table__print(Table* self) {
  for (int i = 0; i < self->m_players->size; i++) {
    Player__print(self->m_players->items[i]);
  }
  Dealer__print(self->m_dealer);
  printf("\n");
}