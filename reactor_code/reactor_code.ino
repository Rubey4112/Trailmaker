#include <Bounce2.h>
#define TESTMODE

const int BUT1 = D2;
const int BUT2 = D3;
const int BUT3 = D7;
const int BUT4 = D8;
const int button_pin_array[4] = {BUT1, BUT2, BUT3, BUT4};

Bounce2::Button* button_array = new Bounce2::Button[4];

const int LED1 = D0;
const int LED2 = D1;
const int LED3 = D10;
const int LED4 = D9;
const int led_array[4] = {LED1, LED2, LED3, LED4};

bool start = false;
int difficulty = 0; // 0-9
bool loose = false;
int level = 0;
bool lights_on = false;


typedef struct {
  int indicator; // indicate the button the player must press to move on
  unsigned long prev_time;
  int duration;
}RoundData;

/* 
indicator, prev_time, duration
*/
RoundData round_data = {0, 0, 0};


void setup() {
  Serial1.begin(9600);
  for (int i = 0; i < 4; i++) {
    button_array[i].attach(button_pin_array[i], INPUT_PULLUP);
    button_array[i].interval(5);
    button_array[i].setPressedState(LOW);
  }

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  randomSeed(analogRead(A0));
}

/*
  Buttons layout
  1 2 3 4
*/


/*
  1 + 4 = Start game
  2 decrease difficulty
  3 increase difficulty

  1 view previous high score
  4 change game mode
*/
void game_menu() {
  Serial1.println("menu");
  if (button_array[0].isPressed() && button_array[3].isPressed()) {
    start = true;
  }
  if (button_array[1].pressed()) {
    difficulty--;
  }
  if (button_array[2].pressed()) {
    difficulty++;
  }
}


/*
  if button pressed and indicator on -> next round
  if button pressed and indicator off -> you loose
  if timer ran out -> you loose
  every level the difficulty increase
*/

/*
  delay decreases slowly but eventually get fast and then slow again
  using a precalculated table
*/
const int duration_table[10] = {800, 760, 720, 668, 611, 540, 400, 325, 320, 300};
int get_duration(int difficulty) {
  return difficulty < 10 ? duration_table[difficulty] : duration_table[9];
}

int get_delay() {
  return int(random(200, 1600));
}


/*
  This function should only be called once at the start of every round
*/
void new_round(int difficulty, bool has_run) {
  if (!has_run) {
    new_round_helper(difficulty);
    has_run = true;
  }
}

void new_round_helper(int difficulty) {
  round_data = {random(4), millis(), get_duration(difficulty)};
}

/*
  return the round the player reaches

  random amount of time before showing player the lights
*/
void game_loop() {
  Serial1.println("game loop");
  unsigned long curr_time = millis();


  if (!lights_on) {
    lights_on = true;
    digitalWrite(led_array[round_data.indicator], HIGH);
  }
  if (!loose && curr_time - round_data.prev_time < round_data.duration)
  {
    for (int i = 0; i < 4; i++)
    {
      if (button_array[i].pressed()) {
        if (i == round_data.indicator) {
          difficulty++;
          level++;
          digitalWrite(round_data.indicator, LOW);
        }
        else {
          loose = true;
        }
      }
    }
  }
  else {
    game_loose();
  }
}

/*
  reset global val and display score
*/
void game_loose() {
  Serial1.println("game loose");
  // start = false;
  loose = false;
  difficulty = 0;

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    delay(500);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    delay(500);
  }
  delay(500);

  for (int i = level % 10; i >= 0; i--) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    delay(350);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    delay(350);
  }

  for (int i = level / 10; i >= 0; i--) {
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    delay(350);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    delay(350);
  }

  level = 0;


}

void test_mode() {
  for (int i = 0; i < 4; i++) {
    if (button_array[i].isPressed()) {
      digitalWrite(led_array[i], HIGH);
    }
    else {
      digitalWrite(led_array[i], LOW);
    }
  }
}

void led_test_mode() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(led_array[i], HIGH);
    delay(1000);
    digitalWrite(led_array[i], LOW);
    delay(1000);
  }
}

void update_buttons() {
  for (int i = 0; i < 4; i++) {
    button_array[i].update();
  }
}

void loop() {
  Serial1.println(start);
  update_buttons();
  delay(1); // for wokwi simulation so it doesn't lag


#ifdef TESTMODE
  test_mode();
#endif

  #ifndef TESTMODE
    if (start) {
      game_loop();
    } else {
      game_menu();
    }
  #endif
}