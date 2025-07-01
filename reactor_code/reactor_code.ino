#include <Bounce2.h>
#define TESTMODE

const int BUT1 = D2;
const int BUT2 = D3;
const int BUT3 = D7;
const int BUT4 = D8;

Bounce2::Button button1 = Bounce2::Button();
Bounce2::Button button2 = Bounce2::Button();
Bounce2::Button button3 = Bounce2::Button();
Bounce2::Button button4 = Bounce2::Button();


const int LED1 = D0;
const int LED2 = D1;
const int LED3 = D10;
const int LED4 = D9;
const int led_array[4] = {LED1, LED2, LED3, LED4};

bool start = false;
int difficulty = 0; // 0-9

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
  button1.attach(BUT1, INPUT_PULLUP);
  button2.attach(BUT2, INPUT_PULLUP);
  button3.attach(BUT3, INPUT_PULLUP);
  button4.attach(BUT4, INPUT_PULLUP);

  button1.interval(5);
  button2.interval(5);
  button3.interval(5);
  button4.interval(5);

  button1.setPressedState(LOW);
  button2.setPressedState(LOW);
  button3.setPressedState(LOW);
  button4.setPressedState(LOW);

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
  if (button1.pressed() && button4.pressed()) {
    start = true;
  }
  if (button2.pressed()) {
    difficulty--;
  }
  if (button3.pressed()) {
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

bool loose = false;
int level = 0;
bool lights_on = false;

Bounce2::Button button_array[4] = {button1, button2, button3, button4};

/* 
return the round the player reaches

random amount of time before showing player the lights
*/
void game_loop() {

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
  start = false;
  loose = false;
  difficulty = 0;

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    delay(200);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
  }

  for (int i = level % 10; i >= 0; i--) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    delay(350);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
  }

  for (int i = level / 10; i >= 0; i--) {
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    delay(350);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
  }

  level = 0;
  

}

void test_mode() {
  for(int i = 0; i<4; i++) {
    if(button_array[i].pressed()) {
      digitalWrite(led_array[i], HIGH);
    }
    else {
      digitalWrite(led_array[i], LOW);
    }
  }
}

void loop() {
  button1.update();
  button2.update();
  button3.update();
  button4.update();

  #ifdef TESTMODE
    test_mode();
  #endif

  #ifndef TESTMODE
    start ? game_loop() : game_menu();
  #endif
}
