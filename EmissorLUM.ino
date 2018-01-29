//EMISSOR
/* -> Vamos mandar um sinal para um arduino (receptor) com o sensor de luminosidade
 *    para medir a luminosidade do ambiente, o receptor deve mandar o quanto de lumi-
 *    nosidade para o emissor!
 *    Vamos usar protocolos de 20 bytes.
*/
//RX: Arduino pin 2, XBee pin DOUT. TX: Arduino pin 3, XBee pin DIN;

#include "SoftwareSerial.h"
#include <Wire.h>
#include <BH1750.h>

SoftwareSerial XBee(2,3);

//VARIAVEIS
byte rx_data[16];
byte tx_data[16];
byte EMISSOR_LUM[16] = {0x0A, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                        0x00, 0x00, 0x00, 0x00, 0x00, 0xAB};
byte RECEPTO_LUM[16] = {0x0A, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                        0x00, 0x00, 0x00, 0x00, 0x00, 0xAB};
int n = 0;
int luminosidade = 0;
int lumMSB = 0;
int lumLSB = 0;


void setup(){
//inicializar comunicação xbee e serial
  XBee.begin(9600);
  Serial.begin(9600);
  n = 0;
  }

void loop(){
//espera comando do teclado para mandar sinal de leitura
  if(Serial.available() > 0){
     char c = Serial.read();
    if( c == 'L'){

       tx_data[0] = 0x0A;
       tx_data[1] = 0x01;
       tx_data[2] = 0x01;
       tx_data[3] = 0x00;
       tx_data[4] = 0x00;
       tx_data[5] = 0x00;
       tx_data[6] = 0x00;
       tx_data[7] = 0x00;
       tx_data[8] = 0x00;
       tx_data[9] = 0x00;
       tx_data[10] = 0x00;
       tx_data[11] = 0x00;
       tx_data[12] = 0x00;
       tx_data[13] = 0x00;
       tx_data[14] = 0x00;
       tx_data[15] = 0xAB;
      /* tx_data[16] = 0x00;
       tx_data[17] = 0x00;
       tx_data[18] = 0x00;
       tx_data[19] = 0xAB;*/
       XBee.write(tx_data, sizeof(tx_data));
      }
    
   }

   //espera o dado do receptor
  if(XBee.available() > 0){

    rx_data[n] = XBee.read(); //le do receptor
    n++;
    delay(100);
    if(n > 15){

       lumMSB = (byte)(((rx_data[3]) << 8) & 0xFF); //+(byte)(rx_data[4] & 0xFF);
       lumLSB = (byte)(rx_data[4] & 0xFF);
       luminosidade = lumMSB + lumLSB;
      Serial.print("Luminosidade:");
      Serial.print(luminosidade);
      Serial.println(""); 

      Serial.print("OK!");
         //limpa o buffer tx
                for(int i = 0; i < 16; i++){
                  tx_data[i] = 0;
                 }
              
                //limpa o buffer rx
                 for(int i = 0; i < 16; i++){
                  rx_data[i] = 0;
                 }
          n = 0;
     }
    
  }

    
  }

