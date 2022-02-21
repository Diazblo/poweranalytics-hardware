#define V_SERIAL Serial

#ifdef V_SERIAL
  #define ANSI_S V_SERIAL.print( "\e[1;32m" );
  #define ANSI_E V_SERIAL.print( "\e[1;37m" );

  #define V_PRINT(...)  ANSI_S;V_SERIAL.print( __VA_ARGS__ );ANSI_E
  #define V_PRINTln(...) V_SERIAL.println( __VA_ARGS__ )
  #define V_PRINTf(...) V_SERIAL.printf( __VA_ARGS__ )
#else
  #define V_PRINT(...)
  #define V_PRINTln(...)
  #define V_PRINTf(...)
#endif