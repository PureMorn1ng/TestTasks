/*
После последних уточнений задачи, логика работы светофоров была изменена на R->Y->G->Y, что 
позволило совершенно безболезненно разнести работу светофоров по схеме:
R1->Y1->G1->Y1 - режим работы светофора для коров, время цикла = 40 сек
G2->Y2->R2->Y2 - режим работы светофора для овец, время цикла = 40 сек
Иначе говоря, система может находиться в 3 состояниях
1: R1 G2
2: Y1 Y2
3: G1 R2
4: Y1 Y2
Условием перехода является достижение времени работы
@Petrov Ivan 2024
*/


int State = 1; // начальное состояние системы
unsigned long CowsClosedSheepsOpened = 15000; //постоянные времени работы светофоров
unsigned long BothTrafficLightsYellow_firstTime = 20000;
unsigned long CowsOpenedSheepsClosed = 35000;
unsigned long BothTrafficLightsYellow_secondTime = 40000;

unsigned long t_init = 0; //время начала работы

short int R_PINcows = 8; //пины светофоров для коров
short int G_PINcows = 9;
short int Y_PINcows = 10;

short int R_PINsheeps = 11;//пины светофоров для овец
short int G_PINsheeps = 12;
short int Y_PINsheeps = 13;



void setup() {
  pinMode(R_PINcows, OUTPUT);
  pinMode(G_PINcows, OUTPUT);
  pinMode(Y_PINcows, OUTPUT);

  pinMode(R_PINsheeps, OUTPUT);
  pinMode(G_PINsheeps, OUTPUT);
  pinMode(Y_PINsheeps, OUTPUT);
}

void loop() {
  checkState();  
  doState();
  Serial.println(millis() - t_init);
    Serial.println(State);
}

void doState() {

  switch (State) {
    case 1:
      controlTrafficLigths(R_PINcows, G_PINcows, Y_PINcows, 1, 0, 0);
      controlTrafficLigths(R_PINsheeps, G_PINsheeps, Y_PINsheeps, 0, 1, 0);
      break;

    case 2:
      controlTrafficLigths(R_PINcows, G_PINcows, Y_PINcows, 0, 0, 1);
      controlTrafficLigths(R_PINsheeps, G_PINsheeps, Y_PINsheeps, 0, 0, 1);
      break;

    case 3:
      controlTrafficLigths(R_PINcows, G_PINcows, Y_PINcows, 0, 1, 0);
      controlTrafficLigths(R_PINsheeps, G_PINsheeps, Y_PINsheeps, 1, 0, 0);
      break;
  }
}

void controlTrafficLigths(short int R_PIN, short int G_PIN, short int Y_PIN, short int valR, short int valG, short int valY) {

  digitalWrite(R_PIN, valR);  // Red pin signal
  digitalWrite(G_PIN, valG);  // Green pin signal
  digitalWrite(Y_PIN, valY);  // Blue pin signal
}

unsigned long checkState() { //проверка состояния системы по прошедшему времени

  if (millis() - t_init < BothTrafficLightsYellow_secondTime) 
  {
    State = 2;    
  }

  if (millis() - t_init < CowsOpenedSheepsClosed) {
    State = 3;
  }

  if (millis() - t_init < BothTrafficLightsYellow_firstTime)
  {
    State = 2;
  }

  if (millis() - t_init < CowsClosedSheepsOpened) 
  {
    State = 1;
  }
  if (millis() - t_init > BothTrafficLightsYellow_secondTime)
  {
    t_init = millis();
  }
  return 0;
}
