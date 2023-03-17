#include<SoftwareSerial.h>
#include<LiquidCrystal_I2C.h>
//#define RX 5
//#define TX 6
#define RXaudio 2
#define TXaudio 3
LiquidCrystal_I2C lcd(0x27,20,4);
//SoftwareSerial BlueT(RX,TX);
SoftwareSerial mySerial(RXaudio,TXaudio);
static int8_t Send_buf[8] = {0} ;//The MP3 player undestands orders in a 8 int string 

                                 //0X7E FF 06 command 00 00 00 EF;(if command =01 next song order)  


int ENB=10; //sortie en PWM permettant de contrôler la vitesse de rotation des roues
int IN3=4;  //sortie pour contrôler le moteur des roues, si sa tension est supérieure à IN4 les roues avancent
int IN4=7;  //sortie pour contrôler le moteur des roues, si sa tension est supérieure à IN3 les roues font tourner le robot dans le sens anti-horaire
int IN1=9;  //sortie pour contrôler le moteur de danse, en LOW pour faire danser
int IN2=8;  //sortie pour contrôler le moteur de danse, en HIGH pour faire danser
int ENA=11; //sortie en PWM permettant de contrôler la vitesse de rotation du moteur pour la danse
char commande;
int puissance_roue; //variable permettant de contrôler la vitesse de rotation des roues
int puissance_danse; //variable permettant de contrôler la vitesse de danse
boolean boolDanse=false; //quand =true , la commande danse(puissance_danse) est envoyé à chaque boucle
boolean boolAvancer=false; //quand =true , la commande avancer(puissance_roue) est envoyé à chaque boucle
boolean boolTourner=false; //quand =true , la commande tourner(puissance_roue) est envoyé à chaque boucle
#define NEXT_SONG 0X01  
#define PREV_SONG 0X02  
#define CMD_PLAY_W_INDEX 0X03 //DATA IS REQUIRED (number of song) 
#define VOLUME_UP_ONE 0X04 
#define VOLUME_DOWN_ONE 0X05 
#define CMD_SET_VOLUME 0X06//DATA IS REQUIRED (number of volume from 0 up to 30(0x1E)) 
#define SET_DAC 0X17 
#define CMD_PLAY_WITHVOLUME 0X22 //data is needed  0x7E 06 22 00 xx yy EF;(xx volume)(yy number of song) 
#define CMD_SEL_DEV 0X09 //SELECT STORAGE DEVICE, DATA IS REQUIRED 
#define DEV_TF 0X02 //HELLO,IM THE DATA REQUIRED 
#define SLEEP_MODE_START 0X0A 
#define SLEEP_MODE_WAKEUP 0X0B 
#define CMD_RESET 0X0C//CHIP RESET 
#define CMD_PLAY 0X0D //RESUME PLAYBACK 
#define CMD_PAUSE 0X0E //PLAYBACK IS PAUSED 
#define CMD_PLAY_WITHFOLDER 0X0F//DATA IS NEEDED, 0x7E 06 0F 00 01 02 EF;(play the song with the directory \01\002xxxxxx.mp3 
#define STOP_PLAY 0X16 
#define PLAY_FOLDER 0X17// data is needed 0x7E 06 17 00 01 XX EF;(play the 01 folder)(value xx we dont care) 
#define SET_CYCLEPLAY 0X19//data is needed 00 start; 01 close 
#define SET_DAC 0X17//data is needed 00 start DAC OUTPUT;01 DAC no output 
#define SINGLE_PLAY 0X08//Single play(without folder)


void affiche() {
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Salut,");
  lcd.setCursor(2,1);
  lcd.print("C'est Wall-ED !");
}

void tourner(int power) {
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(ENB,power);
}

void arret() {
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(ENB,0);
}

void avancer(int power) {
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(ENB,power);
}

void danser(int power) {
  digitalWrite(IN2,HIGH);
  digitalWrite(IN1,LOW);
  analogWrite(ENA,power);
}

void stopDanser() {
  digitalWrite(IN2,HIGH);
  digitalWrite(IN1,LOW);
  analogWrite(ENA,0);
}
void startChore() {
  {
    for(int i=0;i<4;i++){
      avancer(255-i*20);
      delay(300);
      arret();
      delay(200);
    }
    for(int i=0;i<4;i++){
      danser(165+i*30);
      delay(500);
      stopDanser();
      delay(200);
    }
    for(int i=0;i<3;i++){
      tourner(255-i*15);
      delay(600);
      arret();
      danser(150);
      delay(300);
    }
    for(int i=0;i<3;i++){
      tourner(255-i*15);
      delay(600);
      arret();
      danser(150);
      delay(300);
    }
    for(int i=0;i<3;i++){
      tourner(255-i*15);
      delay(600);
      arret();
      danser(150);
      delay(300);
    }
  }
}
void sendCommand(int8_t command, int16_t dat) { 
  delay(20); 
  Send_buf[0] = 0x7e; //starting byte 
  Send_buf[1] = 0xff; //version 
  Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte 
  Send_buf[3] = command; // 
  Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback 
  Send_buf[5] = (int8_t)(dat >> 8);//datah 
  Send_buf[6] = (int8_t)(dat); //datal 
  Send_buf[7] = 0xef; //ending byte 
  for(uint8_t i=0; i<8; i++){ 
    mySerial.write(Send_buf[i]) ; 
  } 
} 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(500);
  //BlueT.begin(9600);
  delay(500);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(ENA, LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN1,LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  delay(500);
  delay(200);
  lcd.init();
  affiche(); //affiche "Salut c'est Wall-ED" sur l'écran 
  puissance_roue=200;
  puissance_danse=200;
  mySerial.begin(9600);//Start our Serial coms for our serial monitor! 
  delay(100);//Wait chip initialization is complete 
  sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card   
  delay(200);
  sendCommand(SINGLE_PLAY, 0X0001);//Premier chanson sans repertoire
}
void loop(){
  while (Serial.available()) {
    //Serial.println("Bonjour");
    char commande = char(Serial.read());
    //Serial.print(commande);
    if (commande=='A'){boolAvancer=true;} 
    if (commande=='D'){boolDanse=true;}
    if (commande=='a'){boolAvancer=false;boolTourner=false; arret();}
    if (commande=='d'){boolDanse=false; stopDanser();}
    if (commande=='T'){boolTourner=true;}
    if (commande=='G'){ startChore();} //ne fonctionne pas
    if (commande=='K'){puissance_roue=Serial.parseInt();} //lis l'entier qui suit le caractère 'K'
    if (commande=='L'){puissance_danse=Serial.parseInt();} //lis l'entier qui suit le caractère 'L'
    if (commande=='1'){sendCommand(STOP_PLAY,0X0002);sendCommand(SINGLE_PLAY, 0X0001);} //ne fonctionne pas
    if (commande=='2'){sendCommand(STOP_PLAY,0X0002);sendCommand(SINGLE_PLAY, 0X0002);} //ne fonctionne pas
    if (commande=='3'){sendCommand(STOP_PLAY,0X0002);sendCommand(SINGLE_PLAY, 0X0003);} //ne fonctionne pas
    if (commande=='p'){sendCommand(CMD_PAUSE,0X0001);}
    if(commande=='c'){sendCommand(CMD_PLAY,0X0001);}
    if (boolTourner){tourner(puissance_roue);}
    if (boolDanse){danser(puissance_danse);}
    if (boolAvancer){avancer(puissance_roue);}
  }
}
