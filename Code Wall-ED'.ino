const int MoteurTete; //fil jaune 5V
const int MoteurRoue1 = 2; //Si on met le 5v ici les deux roues vont dans le même sens
const int MoteurRoue2 = 3; //Si on met le 5v ici le robot tourne vers la droite

int ENB=10;
int IN3=6;
int IN4=7;

void avancer() {
  digitalWrite(MoteurRoue1,HIGH);
  digitalWrite(MoteurRoue2,LOW);
}

void tourner() {
  digitalWrite(MoteurRoue2,HIGH);
  digitalWrite(MoteurRoue1,LOW);
}

void arret() {
  digitalWrite(MoteurRoue1,HIGH);
  digitalWrite(MoteurRoue2,HIGH);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(MoteurRoue1, OUTPUT);
  pinMode(MoteurTete, OUTPUT);
  pinMode(MoteurRoue2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  digitalWrite(ENB, LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(ENB,255);
  delay(1000);
}