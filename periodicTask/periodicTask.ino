//Double Integrator Development Platform
//Performed by Carlos Xavier Rosero C.
//Microprocessor-based systems

const int v1Pin = A0;  //Voltage at the output of the second integrator
const int v2Pin = A1; //Voltage at the output of the first integrator
const int uPin = 9; // Control voltage
unsigned int countRef = 0; //counter for reference change


unsigned long sysTime = 0;
float v1 = 0.0; //states
float v2 = 0.0;
float u = 0.0;  //control
float offset = 0.06;//OPAMP offset

const float v_max = 3.3;

unsigned char *pointer_v1 = (unsigned char *)&v1;
unsigned char *pointer_v2 = (unsigned char *)&v2;
unsigned char *pointer_u = (unsigned char *)&u;
unsigned char *pointer_sysTime = (unsigned char *)&sysTime;

#define ledPin 13 //toggles whenever reference changes

//variables for error handling 

float ek = 0;
float ek_1 = 0;
float uk_1 = 0;

void readStates(void)
{
  v1 = (float)(analogRead(v1Pin));
  v1 = (v1*5/1024) - v_max/2 - offset;
  
  v2 = (float)(analogRead(v2Pin));
  v2 = (v2*5/1024) - v_max/2 - offset;
}

void writeControl(void)
{
  float accum;
  //255 --> 5V
  //168 --> 3.3V
  //84 --> 3.3V/2

  if (u > (v_max/2)) //output limiting, to adjust to the hardware used
    u = v_max/2;
  if (u < -(v_max/2))
    u = -v_max/2;
 
  accum = (u/v_max)*168 + 84; //sets the PWM duty cycle, puts an offset of the half of the maximum voltage
  
  if (accum > 168)
    accum = 168;
    
  if (accum < 0)
    accum = 0;
  
  OCR1AL = (unsigned char)(accum);
}

#define topCountRef 250 //reference with period: Tref = 2*topCountRef*sampling
//Reference setting: comment one at a time in order to choose between square or step reference
#define refPos -1.0f //positive and negative reference values, change these values if you
#define refNeg 1.0f //believe appropriate

#define squareSig 1
#define stepSig 2
unsigned char typeRef = squareSig; //square signal 
//unsigned char typeRef = stepSig; //step signal
////////////////////////////////////////////////
float r = refNeg; //reference
unsigned char *pointer_r = (unsigned char *)&r;

void changeReference(void)
{
  countRef++;
  
  if (countRef >= topCountRef)
  {
      if (typeRef == squareSig)
      {
        digitalWrite(ledPin, digitalRead(ledPin)^1);
        countRef = 0;
        if (r == refPos) //toggle reference
        {
          r = refNeg;
        }
        else if (r == refNeg)
        {
          r = refPos;
        }      
      }
      else if (typeRef == stepSig)
      {
        r = refPos;
      }
  }
}

void calculateControl(void)
{
  ///////////////////////////////////
  u = r; //reference on the input
         //comment it once the controller has been developed
  /////////////////////////////////// 

}

void sendSerial(void)
{
  int i;
  
  Serial.write(0x01);//header
  
  for (i=3; i>=0; i--)
    Serial.write(pointer_sysTime[i]);

  for (i=3; i>=0; i--)
    Serial.write(pointer_r[i]);
      
  for (i=3; i>=0; i--)
    Serial.write(pointer_v1[i]);

  for (i=3; i>=0; i--)
    Serial.write(pointer_v2[i]);
      
  for (i=3; i>=0; i--)
    Serial.write(pointer_u[i]);
}

void setup()
{
  Serial.begin(115200);
  
  pinMode(ledPin, OUTPUT);
  pinMode(9, OUTPUT); //PWM output (OC1A)

  // Initialize Timer2
  // normal mode
  // T = (1/Tosc)*(2^resolution-TCNT2)*prescaler
  // T = (1/16Mhz)*(2^8-99)*1024= 10.048ms
  noInterrupts(); // disable all interrupts
  TCCR2A = (0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (0<<WGM20);   
  TCCR2B = (0<<WGM22) | (1<<CS22) | (1<<CS21) | (1<<CS20);

  TCNT2 = 0x63; //99
  OCR2A = 0x00;
  OCR2B = 0x00;

  TIMSK2 = (0<<OCIE2B) | (0<<OCIE2A) | (1<<TOIE2); // enable timer2 overflow interrupt
  
  // Initialize Timer1
  // fast PWM mode with top=0xff
  // OC1A output, non inverted PWM
  // Freq_PWM = Fosc / (prescale * TOP)
  // Freq_PWM = 16Mhz / (8*256) = 7.812Khz
  
  TCCR1A = (1<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (1<<WGM10);   
  TCCR1B = (0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);

  TCNT1H = 0x00;
  TCNT1L = 0x00;
  OCR1AH = 0x00;
  OCR1AL = 0x00;
  
  interrupts(); // enable all interrupts
}

ISR(TIMER2_OVF_vect) // interrupt service routine 
{
  TCNT2 = 0x63; // preload timer (change this value
                // to change sampling time)
  changeReference();
  sysTime = millis(); //count system time in milliseconds
  readStates();
  calculateControl();
  writeControl();
  sendSerial();
}

void loop()
{
  //nothing to do here
}
