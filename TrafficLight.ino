/*
  Задание 3.
  В данной задаче я выделил 6 состояний системы
  1: G1 R2 R3
  2: R1 G2 R3
  3: R1 R2 G3
  4: Y1 R2 R3
  5: R1 Y2 R3
  6: R1 R2 Y3
  Переход из состояний 1-3 в состояния 4-6 происходит по нажатию кнопки и выполнению условия горения зеленого света
  Переход из состояний 4-6 в состояния 1-3 происходит по нажатой кнопке и выполнению условия горения желтого света

  @Petrov Ivan 2024
*/



short int ButtonCowsPin = A1;  // пины кнопок для 1,2 и 3 светофора
short int ButtonSheepsPin = A2;
short int ButtonGoatsPin = A3;

short int R_PINcows  = 8;//пины светофоров для животных
short int G_PINcows  = 9;
short int Y_PINcows  = 10;

short int R_PINsheeps  = 11;
short int G_PINsheeps  = 12;
short int Y_PINsheeps  = 13;

short int R_PINgoats = 5;
short int G_PINgoats = 6;
short int Y_PINgoats = 7;

short int gate4Cows = 2;//пины заслонок для животных
short int gate4Sheeps = 3;
short int gate4Goats = 4;

short int St_last = 1;            //предыдущее состояние системы от 1 до 6
short int St_new = 1;             //новое состояние системы от 1 до 6
long unsigned int tlast = 0;      //время начала текущего состояния
long unsigned int tYlast = 0;     //время наступления желтого режима светофора в текущем состоянии
bool GreenFlag, YellowFlag; //флаги, указывающие на то, проработал ли текущий зеленый\желтый свет нужное время

long unsigned int firstTimeY = 0;  //счетчик одноразового включения желтого света

void setup() {
  pinMode(R_PINcows , OUTPUT);
  pinMode(G_PINcows , OUTPUT);
  pinMode(Y_PINcows , OUTPUT);

  pinMode(R_PINsheeps , OUTPUT);
  pinMode(G_PINsheeps , OUTPUT);
  pinMode(Y_PINsheeps , OUTPUT);

  pinMode(R_PINgoats, OUTPUT);
  pinMode(G_PINgoats, OUTPUT);
  pinMode(Y_PINgoats, OUTPUT);

  pinMode(gate4Cows, OUTPUT);
  pinMode(gate4Sheeps, OUTPUT);
  pinMode(gate4Goats, OUTPUT);

  pinMode(ButtonCowsPin, INPUT);
  pinMode(ButtonSheepsPin, INPUT);
  pinMode(ButtonGoatsPin, INPUT);

  digitalWrite(gate4Cows, 0);  // сначала работы программы все заслонки
  digitalWrite(gate4Sheeps, 0);  // в закрытом положении
  digitalWrite(gate4Goats, 0);
}

void loop() {


  readButtons();
  doState();
  gatesControl();
  checkTrafficLigths();
  stateTransfer();
}

void controlTrafficLigths(short int R_PIN, short int G_PIN, short int Y_PIN, short int valR, short int valG, short int valY) {

  digitalWrite(R_PIN, valR);  // Red pin signal
  digitalWrite(G_PIN, valG);  // Green pin signal
  digitalWrite(Y_PIN, valY);  // Yellow pin signal
}

bool buttonInput(short int ButtonPin) {
  int buttonVal = analogRead(ButtonPin); //читаем с заданного аналогового пина
  if (buttonVal > 0) { //если значение не ноль -> то присваиваем единицу , в других случаях получится ноль
    buttonVal = 1;
  }
  return bool(buttonVal);
}

void gatesControl() {
  short int State = St_last;
  switch (State) {
    case 1:
      digitalWrite(gate4Cows, 1); //при единице на воротах, заслонка открыта
      digitalWrite(gate4Sheeps, 0);
      digitalWrite(gate4Goats, 0);
      break;

    case 2:
      digitalWrite(gate4Cows, 0);
      digitalWrite(gate4Sheeps, 1);
      digitalWrite(gate4Goats, 0);
      break;

    case 3:
      digitalWrite(gate4Cows, 0);
      digitalWrite(gate4Sheeps, 0);
      digitalWrite(gate4Goats, 1);
      break;
  }
}

void readButtons() {

  bool ButtonCowsVal = buttonInput(ButtonCowsPin);
  if (ButtonCowsVal == 1 && St_last != 1) {  //проверка повторного нажатия кнопки из загона
    St_new = 1;                       //получение нового состояния
  }
  bool ButtonSheepsVal = buttonInput(ButtonSheepsPin);
  if (ButtonSheepsVal == HIGH && St_last != 2) {
    St_new = 2;
  }
  bool ButtonGoatsVal = buttonInput(ButtonGoatsPin);
  if (ButtonGoatsVal == HIGH && St_last != 3) {
    St_new = 3;
  }
}

void doState() {
  short int State = St_last; // выполнение текущего состояния системы
  switch (State) {
    case 1:
      controlTrafficLigths(R_PINcows , G_PINcows , Y_PINcows , 0, 1, 0);
      controlTrafficLigths(R_PINsheeps , G_PINsheeps , Y_PINsheeps , 1, 0, 0);
      controlTrafficLigths(R_PINgoats, G_PINgoats, Y_PINgoats, 1, 0, 0);
      break;

    case 2:
      controlTrafficLigths(R_PINcows , G_PINcows , Y_PINcows , 1, 0, 0);
      controlTrafficLigths(R_PINsheeps , G_PINsheeps , Y_PINsheeps , 0, 1, 0);
      controlTrafficLigths(R_PINgoats, G_PINgoats, Y_PINgoats, 1, 0, 0);
      break;

    case 3:
      controlTrafficLigths(R_PINcows , G_PINcows , Y_PINcows , 1, 0, 0);
      controlTrafficLigths(R_PINsheeps , G_PINsheeps , Y_PINsheeps , 1, 0, 0);
      controlTrafficLigths(R_PINgoats, G_PINgoats, Y_PINgoats, 0, 1, 0);
      break;

    case 4:
      controlTrafficLigths(R_PINcows , G_PINcows , Y_PINcows , 0, 0, 1);
      controlTrafficLigths(R_PINsheeps , G_PINsheeps , Y_PINsheeps , 1, 0, 0);
      controlTrafficLigths(R_PINgoats, G_PINgoats, Y_PINgoats, 1, 0, 0);
      break;

    case 5:
      controlTrafficLigths(R_PINcows , G_PINcows , Y_PINcows , 1, 0, 0);
      controlTrafficLigths(R_PINsheeps , G_PINsheeps , Y_PINsheeps , 0, 0, 1);
      controlTrafficLigths(R_PINgoats, G_PINgoats, Y_PINgoats, 1, 0, 0);
      break;

    case 6:
      controlTrafficLigths(R_PINcows , G_PINcows , Y_PINcows , 1, 0, 0);
      controlTrafficLigths(R_PINsheeps , G_PINsheeps , Y_PINsheeps , 1, 0, 0);
      controlTrafficLigths(R_PINgoats, G_PINgoats, Y_PINgoats, 0, 0, 1);
      break;
  }
}

void checkTrafficLigths() {
  GreenFlag = 0; YellowFlag = 0;
  if (millis() - tlast > 15000) {
    GreenFlag = 1;
  }

  if (GreenFlag == 1 && St_new != St_last) { //зеленый отсветил и новое состояние не равно текущему

    if (firstTimeY == 0) {                              // определяем время начала желтого света
      tYlast = millis();
      firstTimeY = 1;
    }

    if (millis() - tYlast > 5000) { //если желтый свет горел больше 5 секунд, флагу присваивается единица
      YellowFlag = 1;
    }
  }
}

void stateTransfer() {

if (St_last == 1 && St_new != 1 && GreenFlag== 1){ //условие перехода из 1 состояния в 4-е
  St_last=4; 
}
if (St_last == 4 && YellowFlag== 1){ //условие перехода из 4 состояния во 2 или 3
  if (St_new==2) {St_last=2; tlast= millis(); firstTimeY=0;}
  if (St_new==3) {St_last=3; tlast= millis(); firstTimeY=0;}
}
if (St_last == 2 && St_new != 2 && GreenFlag== 1){ //условие перехода из 2 состояния в 5-е
  St_last=5; 
}
if (St_last == 5 && YellowFlag== 1){ //условие перехода из 5 состояния в 1 или 3
  if (St_new==1) {St_last=1; tlast= millis(); firstTimeY=0;}
  if (St_new==3) {St_last=3; tlast= millis(); firstTimeY=0;}
}
if (St_last == 3 && St_new != 3 && GreenFlag== 1){ //условие перехода из 3 состояния в 6-е
  St_last=6; 
}
if (St_last == 6 && YellowFlag== 1){ //условие перехода из 5 состояния в 1 или 2-е
  if (St_new==1) {St_last=1; tlast= millis(); firstTimeY=0;}
  if (St_new==2) {St_last=2; tlast= millis(); firstTimeY=0;}
}

}