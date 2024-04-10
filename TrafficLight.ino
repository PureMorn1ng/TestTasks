/*
После последних уточнений задачи, логика работы светофоров была изменена на R->Y->G->Y, что 
позволило совершенно безболезненно разнести работу светофоров по схеме:
R1->Y1->G1->Y1 - режим работы светофора для коров, время цикла = 40 сек
G2->Y2->R2->Y2 - режим работы светофора для овец, время цикла = 40 сек
Иначе говоря, система может находиться в 3 состояниях
1: R1 G2
2: Y1 Y2
3: G1 R2
Условием перехода является достижение времени работы
@Petrov Ivan 2024
*/


int State = 1;
unsigned long CowsClosedSheepsOpened = 15000;
unsigned long BothTrafficLightsYellow_firstTime = 20000;
unsigned long CowsOpenedSheepsClosed = 35000;
unsigned long BothTrafficLightsYellow_secondTime = 40000;

unsigned long t_init = 0;

short int R_PIN1 = 8;
short int G_PIN1 = 9;
short int Y_PIN1 = 10;

short int R_PIN2 = 11;
short int G_PIN2 = 12;
short int Y_PIN2 = 13;



void setup() {
  pinMode(R_PIN1, OUTPUT);
  pinMode(G_PIN1, OUTPUT);
  pinMode(Y_PIN1, OUTPUT);

  pinMode(R_PIN2, OUTPUT);
  pinMode(G_PIN2, OUTPUT);
  pinMode(Y_PIN2, OUTPUT);
  Serial.begin(9600);
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
      controlTrafficLigths(R_PIN1, G_PIN1, Y_PIN1, 1, 0, 0);
      controlTrafficLigths(R_PIN2, G_PIN2, Y_PIN2, 0, 1, 0);
      break;

    case 2:
      controlTrafficLigths(R_PIN1, G_PIN1, Y_PIN1, 0, 0, 1);
      controlTrafficLigths(R_PIN2, G_PIN2, Y_PIN2, 0, 0, 1);
      break;

    case 3:
      controlTrafficLigths(R_PIN1, G_PIN1, Y_PIN1, 0, 1, 0);
      controlTrafficLigths(R_PIN2, G_PIN2, Y_PIN2, 1, 0, 0);
      break;
  }
}

void controlTrafficLigths(short int R_PIN, short int G_PIN, short int Y_PIN, short int valR, short int valG, short int valY) {

  digitalWrite(R_PIN, valR);  // Red pin signal
  digitalWrite(G_PIN, valG);  // Green pin signal
  digitalWrite(Y_PIN, valY);  // Blue pin signal , можно отказаться, т.к. не используется
}

unsigned long checkState() {

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
