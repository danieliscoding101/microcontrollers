//-------------------------
// Subrutinas para llamada
//-------------------------
extern "C"
{
  void ajustes();
  void verificaBotones();
}
//----------------------------------------------------
void setup()
{
    ajustes();
}
//----------------------------------------------------
void loop()
{
    verificaBotones();
}

// Instrucciones de Assembly en http://www.avr-asm-tutorial.net/avr_en/micro_beginner/instructions.html