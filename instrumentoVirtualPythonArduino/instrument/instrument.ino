float x1 = 0.0;
unsigned char *pointer_x1 = (unsigned char *)&x1;

float x2 = 0.0;
unsigned char *pointer_x2 = (unsigned char *)&x2;


unsigned long sysTime = 0;
unsigned char *pointer_sysTime = (unsigned char *)&sysTime;

unsigned char counter=0;


#define ledPin 13

void sendSerial(void)
{
  int i;
  
  for (i=0; i<=3; i++)
    Serial.write(pointer_sysTime[i]); //envia 4 datos desde la memoria de la variable sysTime

  for (i=0; i<=3; i++)
    Serial.write(pointer_x1[i]);

  for (i=0; i<=3; i++)
    Serial.write(pointer_x2[i]);

  Serial.write("aBcD");//End of line
}

void readValue(void)
{
  x1 = (float)(analogRead(A0)); //analog to digital conversion
  x1 = (x1*5/1024); //signal processing

  x2 = (float)(analogRead(A1)); //analog to digital conversion
  x2 = (x2*5/1024); //signal processing
  
}


void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

// Initialize Timer 2
// normal mode
// T = 1/(Fosc)*(2^resolution - TCNT2)*preescaler //tiempo en el que ocurre una interrupción
// T = 1/(16Mhz)*(2^8 - 99)*1024=10.048ms  

  noInterrupts(); //las interrupciones no se ejecutan automáticamente, estas se programan
  TCCR2A = (0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (0<<WGM20);   
  TCCR2B = (0<<WGM22) | (1<<CS22) | (1<<CS21) | (1<<CS20);

  TCNT2 = 0x63; //99
  OCR2A = 0x00;
  OCR2B = 0x00;

  TIMSK2 = (0<<OCIE2B) | (0<<OCIE2A) | (1<<TOIE2); // enable timer2 overflow interrupt //permiso local para las interrupciones
  interrupts();

}

ISR(TIMER2_OVF_vect) // interrupt service routine 
{
  TCNT2 = 0x63; // preload timer (change this value //cargamos el 99 de nuevo para que siga contando
                // to change sampling time)
  sysTime = millis(); //count system time in milliseconds
  readValue(); //read sensor

  counter++;
  if(counter>10) //10*10ms = 100ms
  {
    counter = 0;
    sendSerial(); //send data over USB
  }
  
  
}

void loop()
{
  //nothing to do here
}
