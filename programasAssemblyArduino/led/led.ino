//-------------------------
// Subrutinas para llamada
//-------------------------
extern "C"
{
  void inicio();
  void led(byte);
}
//----------------------------------------------------
void setup()
{
  inicio();
}
//----------------------------------------------------
void loop()
{
  led(1);
  led(0);
}

// Instrucciones de Assembly en http://www.avr-asm-tutorial.net/avr_en/micro_beginner/instructions.html