#include <LedControl.h>
#define battle_delay 500 // delay intre afisarea literelor numelui jocului (la inceput)
int printed=0; // flag setat pentru printarea numelui (pentru a nu se afisa de mai multe ori)
int placed=0; // flag pentru plasarea navelor( se plaseaza o singura data, inainte de fiecare joc)
int turn=0; // runda in care se afla jocul
int score_p1=0; // scorul primului jucator
int score_p2=0; // scorul celui de al doilea jucator

int UD = 0; // variabila folosita pentru citirea miscarii pe axa OY a joystick-ului
int LR = 0; // variabila folosita pentru citirea miscarii pe axa OX a joystick-ului
int OK = 0; // variabila folosita pentru citirea pozitie butonului(apasat/neapasat)

int current_x=0; // variabile pentru pozitie curenta pe tabla de joc
int current_y=0; 

int DIN = 12; // pinii responsabili pentru controlul matricei cu leduri 
int CS =  11;
int CLK = 10;

int Board[8][8]; // matrice care memoreaza pozitiile navelor si starea lor
                // Conventie
                // 0 - nimic
                // 1- nava intacta
                // 2- nava lovita

byte start[8]= {0x00,0x66,0x66,0x00,0x00,0x66,0x66,0x3c};   
byte hit1[8]=  {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00};
byte hit2[8]=  {0x00,0x00,0x3c,0x24,0x24,0x3c,0x00,0x00};
byte hit3[8]=  {0x00,0x7e,0x42,0x42,0x42,0x42,0x7e,0x00};
byte hit4[8]=  {0xff,0x81,0x81,0x81,0x81,0x81,0x81,0xff};
byte tick[8]=  {0x00,0x01,0x03,0x06,0x8c,0xd8,0x70,0x20};
byte unu[8]=   {0x38,0x78,0x78,0x18,0x18,0x18,0x7e,0x7e};
byte doi[8]=   {0x38,0x7c,0xc6,0xc6,0x0c,0x18,0x38,0xff};
byte R[8]=     {0x7c,0x66,0x66,0x66,0x7c,0x6c,0x66,0x66};
byte Y[8]=     {0x66,0x66,0x66,0x24,0x18,0x18,0x18,0x18};
byte W[8]=     {0xc3,0xc3,0xc3,0xc3,0xdb,0xdb,0xe7,0xc3};
byte N[8]=     {0xc3,0xe3,0xf3,0xdb,0xcf,0xc7,0xc3,0xc3};
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
byte S[8]=     {0x7E,0x7C,0x60,0x7C,0x3E,0x06,0x3E,0x7E};

LedControl lc=LedControl(DIN,CLK,CS,0); // declararea variabilei de control a matricei (folosind biblioteca ledControl.h)

void setup(){
 Serial.begin(9600);
 lc.shutdown(0,false);      // Circuitul MAX72XX se afla in modul power-saving la inceput
 lc.setIntensity(0,5);      // Setarea intensitatii luminoase a ledurilor de pe matrice
 lc.clearDisplay(0);        // Oprirea ledurilor matricei  
 for(int i=0;i<8;i++)       // Initializarea matricei de joc (la inceput nu va fi nicio nava)
    for(int j=0;j<8;j++){
     Board[i][j]=0;
  }
}

void loop(){ 
  if(printed <1)
    printBattleship(); // afisarea numelui jocului
  if(placed==0){
    placeShipsPlayer1(); // plasarea navelor pentru primul jucator (1 nava de 4 puncte si o nava de 3 puncte)
    placeShipsPlayer2(); // plasarea navelor pentru cel de-al doilea jucator
  }
  game(); 
  displayHitShips(); // afisarea navelor lovite
  
}


void game(){ // functia principala a jocului; se ocupa de selectarea tintei, aruncarea bombei, evidenta scorului si afisarea mesajelor in timpul jocului
  UD = analogRead(A1);  // citirea valorilor joystick-ului si ale butonului
  LR = analogRead(A0);
  OK = digitalRead(A2);

  if(OK==1){  // daca butonul va fi apasat
    if(Board[current_x][current_y]==1){ // situatia in care se nimereste o nava
      Board[current_x][current_y]=2; // se marcheaza punctul lovit cu 2 
      if(turn%2==0)
        score_p1++;  // se identifica jucatorul punctat prin paritatea rundei
      else
        score_p2++;
      displayBomb(); // afisarea unei animatii cu o bomba in cazul o nava a fost lovita
      if(score_p1==7) // in cazul in care unul dintre jucatori a ajuns la punctajul maxim se apeleaza functia win cu numarul jucatorului ca parametru
        win(1);
      else if(score_p2==7)
        win(2);
      delay(1000);
    }
    else{
      printByte(NO); // afisarea mesajului "NO" in cazul in care nicio nava nu a fost lovita in runda curenta
      delay(1000);
    }
    turn++;
    if(turn%2==0)
      current_x=current_y=0;  // setarea tintei in partea de sus a tablei de joc in cazul primului jucator
    else
      current_x=current_y=7; // setarea tintei in partea de jos a tablei de joc in cazul celui de-al doilea jucator
  }
  
  char x_translate = map(LR, 1021, 0, 7, 0);  // citirea pozitiei joystick-ului pe axa OX si maparea valorilor pe 8 biti pentru a fi mai usor de prelucrat

  char y_translate = map(UD, 1021, 0, 7, 0);  // citirea pozitiei joystick-ului pe axa OY

  if(y_translate==3 || y_translate==4);       // mutarea tintei in functie de pozitia joystick-ului
  else if(y_translate < 3){
    if(current_x==0)                    
      current_x=3;
    else if(current_x==4)                   // in cazul in care tinta trece peste marginea tablei de joc, va reveni pe partea opusa 
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
  
  displayHitShips(); // afisarea navelor lovite ( se reapeleaza si aici pentru a creste frecventa aprinderii si stingerii ledurilor)
  
  delay(150);
}

int trueRandom(int mod){  // functie care returneaza un numar random modulo mod 
  while(1){
    OK = digitalRead(A2); // se citeste pozitia butonului
    if(OK==1){
      int time = millis(); // se salveaza timpul in milisecunde de la inceputul jocului in momentul apasarii butonului
      int x = time % mod;  // se imparte cu valoarea data ca parametru ( maparea pe intervalul [0,mod) )
      delay(500);
      printByte(tick);     // afisarea unui semn "bifat" pentru a anunta user-ul ca s-a oferit cu succes o valoarea aleatorie
      delay(500);
      lc.clearDisplay(0);
      return abs(x);     // se returneaza modulul valorii
    }
  }
}

void placeShipsPlayer1(){  // plasarea navelor primului jucator ( 1 nava de 4 puncte si o nava de 3 puncte)
  
  int x=trueRandom(4);  // alegerea unui punct aleator
  int y=trueRandom(5);
  for(int i=y;i<y+4;i++)   // plasarea navei de 4 puncte pe orizontala
    Board[x][i]=1;
  while(1){
    int x=trueRandom(4);
    int y=trueRandom(6);
    if(Board[x][y]==0 && Board[x][y+1]==0 && Board[x][y+2]==0){   // plasarea navei de 3 puncte pe orizontala
      for(int i=y;i<y+3;i++)
        Board[x][i]=1;
      break;
    }
  }
}

void placeShipsPlayer2(){
  
  int x=trueRandom(4)+4;  // se adauga 4 pentru plasarea navelor in partea de jos a tablei de joc
  int y=trueRandom(5);
  for(int i=y;i<y+4;i++)
    Board[x][i]=1;
  while(1){
    int x=trueRandom(4)+4;
    int y=trueRandom(6);
    if(Board[x][y]==0 && Board[x][y+1]==0 && Board[x][y+2]==0){
      for(int i=y;i<y+3;i++)
        Board[x][i]=1;
      break;
    }
  }
  placed++;
  printByte(start);    // afisarea unui zambet pentru a anunta plasarea completa a navelor si inceperea jocului
  delay(1000);
}

void displayHitShips(){ // functie care afiseaza punctele lovite de fiecare jucator
   for(int i=0;i<8;i++)
    for(int j=0;j<8;j++){
      if(Board[i][j]==2)
      lc.setLed(0,i,j,true);
    }
}

void displayBomb(){  // afisarea unei animatii corespunzatoare unei bombe ce cade in apa
  printByte(X);
  delay(200);
  printByte(hit1);
  delay(200);
  printByte(hit2);
  delay(200);
  printByte(hit3);
  delay(200);
  printByte(hit4);
  delay(200);
  printByte(X);
  delay(200);
  printByte(hit1);
  delay(200);
  printByte(hit2);
  delay(200);
  printByte(hit3);
  delay(200);
  printByte(hit4);
  delay(200);
}

void win(int x){  // functie ce afiseaza castigatorul jocului si seteaza toti parametrii necesari pentru inceperea urmatorului joc
  int timer=600;
  printByte(P);
  delay(timer);
  printByte(L);
  delay(timer);
  printByte(A);
  delay(timer);
  printByte(Y);
  delay(timer);
  printByte(E);
  delay(timer);
  printByte(R);
  delay(timer);
  if(x==1)
    printByte(unu);
  else
    printByte(doi);
  delay(timer);
  printByte(W);
  delay(timer);
  printByte(I);
  delay(timer);
  printByte(N);
  delay(timer);
  printByte(S);
  delay(timer);
  setup();
  printed=0;
  placed=0;
  turn=-1;
  current_x=0;
  current_y=0;
  score_p1=0;
  score_p2=0;
}

void printBattleship(){   // afisarea numelui jocului la inceput
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

void printByte(byte character [])  // functie care ajuta la afisarea pe matricea cu leduri
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }
}
