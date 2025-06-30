#include <Bounce2.h>

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
const int[] led_array = [LED1, LED2, LED3, LED4];

bool start = false;
int difficulty = 0; // 0-9

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

  randomSeed();
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
  if (button1 && button4) {
    start = true;
  }
  if (button2) {
    difficulty--;
  }
  if (button3) {
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
const int[10] delay_table = [800, 760, 720, 668, 611, 540, 400, 325, 320, 300]
int get_delay(int difficulty) {
  return difficulty < 10 ? delay_table[difficulty] : delay_table[9];
}


bool loose = false;
int level = 0;
bool lights_on = false;

Bounce2::Button[4] button_array = [button1, button2, button3, button4];

/* 
return the round the player reaches

random amount of time before showing player the lights
*/
void game_loop() {
  int indicator = int(random(4));
  long prev_time = millis();
  long curr_time = millis();
  int delay = get_delay(difficulty);
  if (!lights_on) {
    lights_on = true;
    digitalWrite(indicator, HIGH);
  }
  if (!loose && curr_time - prev_time < delay)
  {
    for (int i = 0; i < 4; i++) 
    {
      if (button_array[i].pressed()) {
        if (indicator) {
          difficulty++;
          level++;
          digitalWrite(indicator, LOW);
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

  for (int i = level % 10; i >= 0; i--) {
    digitalWrite(LED1, )
  }

  for (int i = level / 10; i >= 0; i--) {
    
  }

  level = 0;
  

}

void loop() {
  button1.update();
  button2.update();
  button3.update();
  button4.update();

  start ? game_loop() : game_menu();
}
