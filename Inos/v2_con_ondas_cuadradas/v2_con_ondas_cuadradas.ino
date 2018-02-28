
//Para una onda cuadrada de 1 MHz un prescaler de 2MHz
//para hacer el switcheo

//CLK 16MHz
//PRESCALER 1/8 -> 2MHz

static char t0cnt = 0;
static char t0ovr = 0;
static char t1cnt = 0;
static char t1ovr = 0;
static char t2cnt = 0;
static char t2ovr = 0;

static unsigned char stot = 0;
static unsigned char mtot = 0;

static char t = 0;
static char cntF = 0;
static bool act = true;


#define SAM 100
static const char sine[SAM] = {127, 135, 143, 151, 159, 166, 174, 181, 188, 195, 202, 208, 214, 220, 225, 230, 235, 239, 242, 246, 248, 250, 252, 253, 254, 255, 254, 253, 252, 250, 248, 246, 242, 239, 235, 230, 225, 220, 214, 208, 202, 195, 188, 181, 174, 166, 159, 151, 143, 135, 127, 119, 111, 103, 95, 88, 80, 73, 66, 59, 52, 46, 40, 34, 29, 24, 19, 15, 12, 8, 6, 4, 2, 1, 0, 0, 0, 1, 2, 4, 6, 8, 12, 15, 19, 24, 29, 34, 40, 46, 52, 59, 66, 73, 80, 88, 95, 103, 111, 119};
static char cnt = 0;

void setup() {

  //DDRD =  0xE0;
  //PORTD = 0x00;
  DDRD = 0xFF;
  PORTD = 127;
  f5500hz();
}

void loop() {
  // put your main code here, to run repeatedly:

}


ISR(TIMER0_COMPA_vect){ //using pin 5 of PORTD
    if (t0ovr == t0cnt){
        t0routine();
        t0cnt = 0x00;
    }
    else{
        t0cnt += 1;}
}

ISR(TIMER1_COMPA_vect){ //using pin 6 of PORTD
    if (t1ovr == t1cnt){
        t1routine();
        t1cnt = 0x00;
    }
    else{
        t1cnt += 1;}
}

ISR(TIMER2_COMPA_vect){ //using pin 7 of PORTD
    if (t2ovr == t2cnt){
        t2routine();
        t2cnt = 0x00;
    }
    else {
        t2cnt += 1;}
}

void sineWriter(){
  PORTD = sine[cnt];
  cnt++;
  if (cnt==SAM) cnt = 0;
}

void t0routine() {
//PORTD = ((~PORTD & 0x20) | (0xDF & PORTD));
RUSAWrite();
}

void t1routine() {
//PORTD = ((~PORTD & 0x40) | (0xBF & PORTD));
TENSWrite();
}

void t2routine() {
//PORTD = ((~PORTD & 0x80) | (0x7F & PORTD));
//sineWriter();
MODUWrite();
}

static const char MODU = 55;
static char MODUtcnt = 0;
static const char MODUtarray[8] = {4, 6, 4, 5, 9, 3, 14, 1};  

void MODUWrite() //a una frecc de 5500Hz
{
  if (act) PORTD = sine[cnt];
  else PORTD = 0x7C;
  cnt++;

if (cnt == SAM)
{
    cnt = 0;
    cntF++;
    if (cntF == 27) {t++;}
    if (cntF == MODU) {cntF = 0;  t++; timing();}
    if (t == MODUtarray[MODUtcnt]) {act = !act; t = 0; MODUtcnt++;}
    if (MODUtcnt == 8) MODUtcnt = 0;
}
}


void TENSWrite()
{ //un clk en base 50 osease 50Hz para una tens de 1 Hz
  if (cnt == 0) PORTD = 0xFF;
  else
  if (cnt ==23) PORTD = 0x00;
  else PORTD = 0x7C;
  cnt++;
  if (cnt == 50) {cnt = 0; timing();}
}

static const char RUSA = 35;

//SECUENCIA DE LA RUSA
void RUSAWrite() //a una frecc de 3500Hz
{
if (act) PORTD = sine[cnt];
else PORTD = 0x7C;
cnt++;
if (cnt == SAM)
{
    cnt = 0;
    cntF++;
    if (cntF == RUSA) {cntF =0;  t++; timing();}
    if (t==11) {act = !act; t = 0;}
}
}


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

void f2000hz(){setTimer2(124,0x04);} //GALV
void f2000000hz(){setTimer2(7,0x01);} // ULTRA
void f200hz(){setTimer1(77,0x05);}
void f100hz(){ t0ovr = 49; setTimer0(155,0x05);}

void f1500hz(){setTimer0(165,0x03);}
void f1750hz(){setTimer1(9141,0x01);}
void f3000hz(){setTimer2(165,0x03);}
void f10000hz(){setTimer2(24,0x04);}
void f20000hz(){setTimer2(11,0x04);}

void f50hz(){setTimer1(39999,0x02);}
void f3500hz(){setTimer0(70,0x03);}
void f5500hz(){setTimer2(89,0x03);}

//Notes:

//GALV 200Hz  constant signal
//TENS 100Hz 
//TREN 15Hz + 200Hz
//EXPO 100Hz
//MODU 55Hz
//RUSA 35Hz

//interrupt frequency (Hz) = (Arduino clock speed 16,000,000Hz) / (prescaler * (compare match register + 1))
//compare match register = [ 16,000,000Hz/ (prescaler * desired interrupt frequency) ] - 1

