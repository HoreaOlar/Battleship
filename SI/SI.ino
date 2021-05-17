#include <LedControl.h>
#define battle_delay 500
int printed=0;

int UD = 0;
int LR = 0;
int OK = 0;

int current_x=0;
int current_y=0;

int DIN = 12;
int CS =  11;
int CLK = 10;

int Board[8][8];

byte X[8]=     {0x81,0x42,0x24,0x18,0x18,0x24,0x42,0x81};
byte NO[8]=    {0x90,0xd7,0xb5,0x97,0x00,0x00,0x00,0x00};
byte B[8]=     {0x7c,0x66,0x66,0x7c,0x7c,0x66,0x66,0x7c};
byte A[8]=     {0x18,0x3c,0x66,0x66,0x7e,0x7e,0x66,0x66};
byte T[8]=     {0x7e,0x7e,0x18,0x18,0x18,0x18,0x18,0x18};
byte L[8]=     {0x60,0x60,0x60,0x60,0x60,0x60,0x7e,0x7e};   
byte E[8]=     {0x7C,0x7C,0x60,0x7C,0x7C,0x60,0x7C,0x7C};
byte H[8]=     {0x66,0x66,0x66,0x7e,0x7e,0x66,0x66,0x66};
byte I[8]=     {0x3c,0x18,0x18,0x18,0x18,0x18,0x18,0x3c};
byte P[8]=     {0x7c,0x62,0x62,0x62,0x7c,0x60,0x60,0x60};
byte d[8]=     {0x78,0x7C,0x66,0x66,0x66,0x66,0x7C,0x78};
byte u[8]=     {0x66,0x66,0x66,0x66,0x66,0x66,0x7E,0x7E};
byte c[8]=     {0x7E,0x7E,0x60,0x60,0x60,0x60,0x7E,0x7E};
byte S[8]=     {0x7E,0x7C,0x60,0x7C,0x3E,0x06,0x3E,0x7E};
byte dot[8]=   {0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18};
byte o[8]=     {0x7E,0x7E,0x66,0x66,0x66,0x66,0x7E,0x7E};
byte m[8]=     {0xE7,0xFF,0xFF,0xDB,0xDB,0xDB,0xC3,0xC3};

LedControl lc=LedControl(DIN,CLK,CS,0);

void setup(){
 Serial.begin(9600);
 lc.shutdown(0,false);       //The MAX72XX is in power-saving mode on startup
 lc.setIntensity(0,0);      // Set the brightness to maximum value
 lc.clearDisplay(0);         // and clear the display
 for(int i=0;i<8;i++)
    for(int j=0;j<8;j++){
     Board[i][j]=0;
  }

  //Board[0][2]=1;
 
}

void loop(){ 
  if(printed <1)
    printBattleship();
  //move();
  if(printed==0)
    placeShipsPlayer1();
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
      if(Board[i][j]==1);
        //lc.setLed(0,i,j,true);
   //move();
  
   
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
      if(Board[i][j]==1);
        //lc.setLed(0,i,j,true);
   
}


void move(){
  UD = analogRead(A1);
  LR = analogRead(A0);
  OK = digitalRead(A2);

  if(OK==1){
    if(Board[current_x][current_y]==1){
      printByte(X);
      delay(1000);
    }
    else{
      printByte(NO);
      delay(1000);
    }
  }
  
  char x_translate = map(LR, 1021, 0, 7, 0);

  char y_translate = map(UD, 1021, 0, 7, 0);

  if(y_translate==3 || y_translate==4);
  else if(y_translate < 3){
    if(current_x==0)
      current_x=3;
    else if(current_x==4)
      current_x=7;
    else
      current_x--;
  }
  else if(y_translate>4){
    if(current_x==3)
      current_x=0;
    else if(current_x==7)
      current_x=4;
    current_x++;
  }

  if(x_translate==3 || x_translate==4);
  else if(x_translate < 3){
    if(current_y==0)
      current_y=7;
    else
      current_y--;
  }
  else if(x_translate>4){
    if(current_y==7)
      current_y=0;
    else
      current_y++;
  }

  lc.clearDisplay(0);

  lc.setLed(0,current_x,current_y,true);
  //lc.setLed(0,x_translate,y_translate,true);

  //Serial.print(" UD = ");Serial.print(UD, DEC);Serial.print(", LR = ");Serial.print(LR, DEC);Serial.print(", x = ");Serial.print(x_translate, DEC); Serial.print(", y = ");Serial.println(y_translate, DEC); 
  //Serial.print(" current_x= ");Serial.print(current_x);Serial.print("current_y= ");Serial.print(current_y);
  delay(150);
}

void placeShipsPlayer1(){
  int x=random(0,4);
  int y=random(0,7);
  if(y<=1){
    for(int i=0;i<4;i++)
      Board[x][i]=1;
  }
  else if(y>1&&y<6){
    for(int i=y-1;i<y+2;i++)
      Board[x][i]=1;
  }
  else if(y>=6){
    for(int i=4;i<8;i++)
      Board[x][i]=1;
  }
  printed++;
}

void printBattleship(){
    printByte(B);
     
    delay(battle_delay);

    printByte(A);
    
    delay(battle_delay);

    printByte(T);    

    delay(battle_delay);

    printByte(T);

    delay(battle_delay);

    printByte(L);

    delay(battle_delay);

    printByte(E);

    delay(battle_delay);

    printByte(S);

    delay(battle_delay);

    printByte(H);

    delay(battle_delay);

    printByte(I);

    delay(battle_delay);

    printByte(P);

    delay(battle_delay);
   
    lc.clearDisplay(0);
    
    delay(battle_delay);

    printed++;
  
}

void printByte(byte character [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }
}
