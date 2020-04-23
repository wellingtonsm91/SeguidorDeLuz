
#include <Servo.h>       //Inclui a biblioteca do Servo
Servo ServoHori;                  ////Inclui um objeto Servo para controlar o ServoMotor
Servo ServoVert;

//Pinos analógicos dos LDRS
int LSD = 0;      //Valor do LDR superior Direito
int LSE = 1;      //Valor do LDR superior Esquerdo
int LID = 2;      //Valor do LDR inferior Direito
int LIE = 3;      //Valor do LDR inferior Esquerdo
int MLE;          //Média do lado Esquerdo
int MLD;          //Média do lado direito
int MLS;          //Média do lado Superior
int MLI;          //Média do lado Inferior
int tolerancia;   //Valor da tolerância do Servo Motor da placa
int pos = 0;      //Variavel para guarda posicao do Servo
int servo_inf = 90;    //Variável para movimentação do Servo
int servo_sup = 90;    //Variável para movimentação do Servo
//Limites dos valores
//lim_high_hori = 180;
//lim_low_hori = 0;
//lim_high_vert = 135;
//lim_low_vert = 45;

//Inicializaçao do sistema



void setup() {
  ServoHori.attach(9);
  ServoVert.attach(10);
  pinMode(13, OUTPUT); // Pino do LED de confirmação

}

void loop() {
  //Ler os valores dos LDRS

INICIALIZACAO:

//Servo horizontal
for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 90 degrees
    // in steps of 1 degree
    ServoHori.write(pos);              // tell servo to go to position in variable 'pos'
    delay(20);
}

//Servo Vertical
for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 90 degrees
    // in steps of 1 degree
    ServoVert.write(pos);              // tell servo to go to position in variable 'pos'
    delay(20);
}

digitalWrite(13, HIGH);               //Indicação de que acabou a inicialização

LEITURA:
  int  VLSD = analogRead(LSD);
  int  VLSE = analogRead(LSE);
  int  VLID = analogRead(LID);
  int  VLIE = analogRead(LIE);

  //Média dos quatros conjuntos

  MLE = (VLSE + VLIE) / 2;
  MLD = (VLSD + VLID) / 2;
  MLS = (VLSE + VLSD) / 2;
  MLI = (VLIE + VLID) / 2;

  //Movimetação do motor do Disco(horizontal)
  int dif_E_D = MLE - MLD;              //Diferença entre lado esquerdo e direito;
  int dif_S_I = MLS - MLI;              //Diferença entre lado Superior e Inferior;

  //Caso em que o lado esquerdo é menor, ou seja, tem mais luz sobre ele
  //Placa deve ir pra esquerda-> motor deve gira horario com limite em zero
  if (dif_E_D < 0) {                    //Se for negativa
    dif_E_D = dif_E_D * (-1);           //Torna positivo
    if (dif_E_D > tolerancia && servo_inf>0) {   // Deve estar acima da tolerância e acima de zero
      //***colocar código d movimentação do motor aqui
       servo_inf = servo_inf - 1;
       ServoHori.write(servo_inf);
       delay(50);
    
    }
    else if (dif_E_D <= tolerancia) {
      goto MOTOR2; //**Voltar ao programa principal
    }
  }
  //Caso em que o lado direito é menor, ou seja, tem mais luz sobre ele
  //Placa deve ir pra esquerda-> motor deve gira anti- horario com limite em 180
  else                                 //Se for positiva
    if (dif_E_D > tolerancia && servo_inf<90) {        //Compara com tolerancai do Disco
      //***colocar código do movimentação do Servo motor Horizontal aqui
      servo_inf = servo_inf + 1;
      ServoHori.write(servo_inf);
      delay(50);
      //Vai para o motor 2
      goto MOTOR2;
    }
    else if (dif_E_D <= tolerancia) {
      goto MOTOR2;//**Vai para o motor vertical
    }





  //Movimentação do Motor da Placa(vertical)
MOTOR2:

 //Caso em que o lado superior é menor, ou seja, tem mais luz sobre ele
  //Placa deve ir girar para trás-> motor deve gira horario com limite em 45 graus
  if (dif_S_I < 0) {                        //Se for Negativo
    dif_S_I = dif_S_I * (-1);
    if (dif_S_I > tolerancia && servo_sup > 45) {     //// Compara com tolerancia do Disco e verifica limite Inferior
      //***colocar código d movimentação do motor aqui
      servo_sup = servo_sup - 1;
      ServoVert.write(servo_sup);
      delay(50);
      goto LEITURA;
    }
    else if (dif_S_I <= tolerancia) {
      goto LEITURA;
    }
  }
  
  //Se for positiva
  //Caso em que o lado inferior é menor, ou seja, tem mais luz sobre ele
  //Placa deve ir girar para Frente-> motor deve gira anti-horario com limite em 135 graus                          
   else if (dif_S_I > tolerancia && servo_sup < 135) {        // Compara com tolerancia do Disco e verifica limite superior
      //***colocar código do movimentação do Servomotor Vertical aqui
      servo_sup = servo_sup + 1;
      ServoVert.write(servo_sup);
      delay(50);
      goto LEITURA;
 
    }
    else if (dif_S_I <= tolerancia) {
      goto LEITURA;//**Voltar ao programa principal
    }
}





