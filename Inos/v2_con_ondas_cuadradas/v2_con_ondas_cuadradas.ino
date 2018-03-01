
static unsigned char stot = 0;
static unsigned char mtot = 0;

static char unsigned t = 0;
static char unsigned cntF = 0;
static bool act = true;

#define MID 0X80
#define SAM 50
static const unsigned char sine[SAM] = {0x00, 0x07, 0x0f, 0x17, 0x1f, 0x27, 0x2e, 0x36, 0x3d, 0x44, 0x4a, 0x50, 0x56, 0x5c, 0x61, 0x66, 0x6b, 0x6f, 0x72, 0x76, 0x78, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x7e, 0x7d, 0x7c, 0x7b, 0x78, 0x76, 0x72, 0x6f, 0x6b, 0x66, 0x61, 0x5c, 0x56, 0x50, 0x4a, 0x44, 0x3d, 0x36, 0x2e, 0x27, 0x1f, 0x17, 0x0f, 0x07};
static char unsigned cnt = 0;

void setup() {

  //DDRD =  0xE0;
  //PORTD = 0x00;
  DDRD  = 0xFF;
  PORTD = 0x80;
  f10000hzT2();
}

void loop() {
  // put your main code here, to run repeatedly:

}


ISR(TIMER0_COMPA_vect){ //using pin 5 of PORTD
        t0routine();
}

ISR(TIMER1_COMPA_vect){
        t1routine();
}

ISR(TIMER2_COMPA_vect){
        t2routine();
}


void t0routine() {
//PORTD = ((~PORTD & 0x20) | (0xDF & PORTD));
TRENRUSAWrite();
}

void t1routine() {
//PORTD = ((~PORTD & 0x40) | (0xBF & PORTD));
TENSWrite();
}

void t2routine() {
//PORTD = ((~PORTD & 0x80) | (0x7F & PORTD));
//DIADGALVWriter();
//MODUWrite();
EXPOWrite();
}

static bool SAMdone = false;
static const unsigned char tT [6] = {1,2,3,4,5,6};
static unsigned char tTmode = 0;

/////////////////////////////////////
/////////////EXPO////////////////////
/////////////////////////////////////
static const unsigned char EXPO = 100;
static unsigned char Erel = 10;
//static unsigned char rels[10] = {10, };
static bool isEXPO = true;

void EXPOWrite(){ //a una frecc de 10000Hz
    if (cntF<=20)
        Erel = 21 - cntF;
    if (cntF>=80)
        Erel = 21 - (EXPO-cntF);

    DIADGALVWriter();
    
    if (SAMdone)
    {
    SAMdone = false;
    cntF++;
    if (cntF == EXPO) {cntF = 0;  t++; timing();}
    if (t == tT[tTmode]) {act = !act; t = 0;}
    }
}

//###################################
//###################################

//////////////////////////////
/////////GALV y DIAD//////////
//////////////////////////////
static bool isGALV = true;
static bool sinePol = true;
static unsigned char val = 0;

void DIADGALVWriter(){
  if (isEXPO)
    val = sine[cnt]/Erel;
  else
    val = sine[cnt];
  
  if (act)
  if ((sinePol || !isGALV)) PORTD = MID + val;
  else PORTD = MID - val;
  else PORTD = MID;
  cnt++;
  if(cnt==SAM)
  {
    if(!sinePol) {timing(); SAMdone = true;}
    cnt = 0;
    sinePol = !sinePol;
  }
}
//########################################
//########################################


/////////////////////////////////////
/////////////MODU////////////////////
/////////////////////////////////////
static const unsigned char MODU = 55;
static unsigned char MODUtcnt = 0;
static const unsigned char MODUtarray[8] = {4, 6, 4, 5, 9, 3, 14, 1};  

void MODUWrite() //a una frecc de 5500Hz
{
  DIADGALVWriter();
  if (SAMdone)
  {   
      SAMdone = false;
      cntF++;
      if (cntF == 27) {t++;}
      if (cntF == MODU) {cntF = 0;  t++; timing();}
      if (t == MODUtarray[MODUtcnt]) {act = !act; t = 0; MODUtcnt++;}
      if (MODUtcnt == 8) MODUtcnt = 0;
  }
}
//###################################
//###################################

/////////////////////////////////////
/////////////TENS////////////////////
/////////////////////////////////////

void TENSWrite()
{ //un clk en base 50 osease 50Hz para una tens de 1 Hz
  if (cnt == 0) PORTD = 0xFF;
  else
  if (cnt ==23) PORTD = 0x00;
  else PORTD = 0x80;
  cnt++;
  if (cnt == 50) {cnt = 0; timing();}
}
//###################################
//###################################

/////////////////////////////////////
/////////////RUSATREN////////////////
/////////////////////////////////////
static const unsigned char TRENRUSA[2] = {15,35};
static bool isRUSA = true;

void TRENRUSAWrite() //a una frecc de 3500Hz
{
  DIADGALVWriter();
  
  if (SAMdone)
  {
    SAMdone = false;
    cntF++;
    if (cntF == TRENRUSA[isRUSA]) {cntF = 0;  t++; timing();}
    if ((isRUSA && t == 11) || (!isRUSA && t == tT[tTmode]) ) {act = !act; t = 0;}
  }
}
//###################################
//###################################



/////////////////////////////////////
/////////TIMING_FUNCTIONS////////////
/////////////////////////////////////
void timing()
{
  stot++;
  if (stot == 60)
  {mtot++; stot=0;}
}

void setTimer0(char CTC, char CS)
{
    cli();
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0  = 0;
    OCR0A = CTC;
    TCCR0A |= (1 << WGM01);
    TCCR0B |= CS & 0x07;    
    TIMSK0 |= (1 << OCIE0A);
    sei();
}

void setTimer1(int CTC, char CS)
{
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = CTC;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= CS & 0x07;  
  TIMSK1 |= (1 << OCIE1A);
  sei();
}

void setTimer2(char CTC, char CS)
{
  cli();
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;
  OCR2A  = CTC; 
  TCCR2A |= (1 << WGM21);
  TCCR2B |= CS & 0x07;   
  TIMSK2 |= (1 << OCIE2A);
  sei();
}

void f5500hzT2(){setTimer2(89,0x03);}
void f20000hzT2(){setTimer2(11,0x04);}
void f10000hzT2(){setTimer2(24,0x04);}
//###################################
//###################################


//Notes:

//GALV 200Hz  constant signal
//TENS 100Hz 
//TREN 15Hz + 200Hz
//EXPO 100Hz
//MODU 55Hz
//RUSA 35Hz

//interrupt frequency (Hz) = (Arduino clock speed 16,000,000Hz) / (prescaler * (compare match register + 1))
//compare match register = [ 16,000,000Hz/ (prescaler * desired interrupt frequency) ] - 1

