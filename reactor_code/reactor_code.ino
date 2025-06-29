int BUT1 = D2;
int BUT2 = D3;
int BUT3 = D7;
int BUT4 = D8;

int LED1 = D0;
int LED2 = D1;
int LED3 = D10;
int LED4 = D9;

void setup() {
  pinMode(BUT1, INPUT_PULLUP);
  pinMode(BUT2, INPUT_PULLUP);
  pinMode(BUT3, INPUT_PULLUP);
  pinMode(BUT4, INPUT_PULLUP);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  long startTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:

}
