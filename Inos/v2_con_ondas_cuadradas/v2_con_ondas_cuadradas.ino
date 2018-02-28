
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

#define SAM 100
static const char sine[SAM] = {127, 135, 143, 151, 159, 166, 174, 181, 188, 195, 202, 208, 214, 220, 225, 230, 235, 239, 242, 246, 248, 250, 252, 253, 254, 255, 254, 253, 252, 250, 248, 246, 242, 239, 235, 230, 225, 220, 214, 208, 202, 195, 188, 181, 174, 166, 159, 151, 143, 135, 127, 119, 111, 103, 95, 88, 80, 73, 66, 59, 52, 46, 40, 34, 29, 24, 19, 15, 12, 8, 6, 4, 2, 1, 0, 0, 0, 1, 2, 4, 6, 8, 12, 15, 19, 24, 29, 34, 40, 46, 52, 59, 66, 73, 80, 88, 95, 103, 111, 119};
static char sIn = 0;

void setup() {

  //DDRD =  0xE0;
  //PORTD = 0x00;
  DDRD = 0xFF;
  PORTD = 127;
  f200000hz();
  //f1500hz();
  //f1750hz();
  //f3000hz();
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
  PORTD = sine[sIn];
  sIn++;
  if (sIn==SAM) sIn = 0;
}

void t0routine() {
PORTD = ((~PORTD & 0x20) | (0xDF & PORTD));
}

void t1routine() {
PORTD = ((~PORTD & 0x40) | (0xBF & PORTD));
}

void t2routine() {
//PORTD = ((~PORTD & 0x80) | (0x7F & PORTD));
sineWriter();
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
void f200000hz(){setTimer2(9,0x02);}

//Notes:

//GALV 200Hz  constant signal
//TENS 100Hz 
//TREN 15Hz + 200Hz
//EXPO 100Hz
//MODU 55Hz
//RUSA 35Hz

//interrupt frequency (Hz) = (Arduino clock speed 16,000,000Hz) / (prescaler * (compare match register + 1))
//compare match register = [ 16,000,000Hz/ (prescaler * desired interrupt frequency) ] - 1

