/*
   PIZARRABOT
   Es un robot dibujador de pizarra.
   ISET 57
   Alumnos de 2 año 2022
*/
#define EN 8      /* Enable pin for all stepper outputs */
#define X_DIR 5   /* Direction pin for X axis */
#define X_STEP 2  /* Step pin for X axis */
#define Y_DIR 6   /* Direction pin for Y axis */
#define Y_STEP 3  /* Step pin for Y axis */

// vel max=20
const float VEL_MOTORES = 50;
// area de dibujo 1 metro x 1 metro
const int ANCHO = 1000;
const int ALTO = 1000;
const float ARCO = (35 * 3.1415926) / 200;
//
float h1, h2,  pasosMotorX, pasosMotorY, h1ant, h2ant,  T1, delayMotorX, delayMotorY;
bool dirM1, dirM2;

void MoverMotoresXY(float x, float y)
{
  // calculo de hipotenusas (hilo a usar) por c/motor.
  // la diferencia entre la hipotenusa anterior y la actual me va a dar
  // el offset al nuevo X,Y.

  h1 = sqrt(x * x + (ALTO - y) * (ALTO - y));
  h2 = sqrt((ANCHO - x) * (ANCHO - x) + (ALTO - y) * (ALTO - y));
  dirM1 = (h1ant < h1);
  dirM2 = (h2ant < h2);
  pasosMotorX = abs(h1ant - h1) / ARCO;
  pasosMotorY = abs(h2ant - h2) / ARCO;
  h1ant = h1;
  h2ant = h2;

  // calcular delays para c/motor
  // T1 ES EL TIEMPO TOTAL PARA AMBOS MOTORES (COMIENZAN Y TERMINAN JUNTOS)
  // por eso cada delay es diferente (logrando velocidades diferentes)
  if (pasosMotorX > pasosMotorY)
  {
    T1 = VEL_MOTORES * pasosMotorX;
    delayMotorX = VEL_MOTORES;
    delayMotorY = T1 / pasosMotorY;
  }
  else
  {
    T1 = VEL_MOTORES * pasosMotorY;
    delayMotorY = VEL_MOTORES;
    delayMotorX = T1 / pasosMotorX;
  }

  /*Serial.print("h1:");
    Serial.print(h1);
    Serial.print(" h2:");
    Serial.print(h2);
    Serial.print(" pasosM1:");
    Serial.print(pasosM1);
    Serial.print(" pasosM2:");
    Serial.print(pasosM2);
    Serial.print(" dirM1:");
    Serial.print(dirM1);
    Serial.print(" dirM2:");
    Serial.println(dirM2);*/

  /* en la inicializacion no muevo el motor, solo calculo valor anterior.*/
  if (x < 0 || y < 0) return;

  // si se va a mover enciendo los motores
  digitalWrite(EN, LOW); // Low to enable

  /* seteo las direcciones */
  digitalWrite(X_DIR, dirM1);
  digitalWrite(Y_DIR, dirM2);

  int contx = 0, conty = 0;
  int DirectionY = 0, DirectionX = 0;
  for (long i = 0; i < T1 * 2; i++)
  {
    if (++contx >= delayMotorX)
    {
      //                                            _
      // aca genero el pulso del avance del motor _| |_
      digitalWrite(X_STEP, DirectionX % 2 == 0);
      DirectionX++;
      contx = 0;
    }
    if (++conty >= delayMotorY) {
      //                                            _
      // aca genero el pulso del avance del motor _| |_
      digitalWrite(Y_STEP, DirectionY % 2 == 0);
      DirectionY++;
      conty = 0;
    }
  }
  digitalWrite(EN, HIGH); // apago motores
  delay(500);
}


/* test (x1,y1 => x2,y2) y se repite...*/
void Test()
{
  // cuadrado
  MoverMotoresXY(480, 400);
  MoverMotoresXY(480, 30);
  MoverMotoresXY(870, 30);
  MoverMotoresXY(870, 400);
  MoverMotoresXY(870, 400);
}

void setup() {
  Serial.begin(9600);

  pinMode(EN, OUTPUT);
  pinMode(X_DIR, OUTPUT);
  pinMode(X_STEP, OUTPUT);
  pinMode(Y_DIR, OUTPUT);
  pinMode(Y_STEP, OUTPUT);

  /* Enable the X, Y stepper outputs */
  digitalWrite(EN, LOW); //Low to enable

  /* inicializar valores anteriores...*/
  MoverMotoresXY(-1, -1);

  /* MoverMotoresXY(0, 0);
    delay(1000);
    Test();
    delay(5000);
    MoverMotoresXY(0, 0);*/
}

void loop()
{
  MoverMotoresXY(0, 0);
  MoverMotoresXY(100, 500);
  MoverMotoresXY(500, 100);
}
