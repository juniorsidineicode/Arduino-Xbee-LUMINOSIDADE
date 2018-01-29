//RECEPTOR
/* -> Vamos ler o sinal de luminosidade via sensor (BH 1750) e enviar via radio para o
 *    arduino emissor, que enviou o sinal de pedido de leitura
 *    Vamos usar protocolos de 20 bytes.
*/
//RX: Arduino pin 2, XBee pin DOUT. TX: Arduino pin 3, XBee pin DIN;

#include "SoftwareSerial.h"
#include <Wire.h>
#include <BH1750.h>

//define o sensor BH 1750 (Luminosidade)
BH1750 lightMeter;

SoftwareSerial XBee(2,3);

//VARIAVEIS
byte rx_data[16];
byte tx_data[16];
byte EMISSOR_LUM[16] = {0x0A, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                        0x00, 0x00, 0x00, 0x00, 0x00, 0xAB};
byte RECEPTO_LUM[16] = {0x0A, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                        0x00, 0x00, 0x00, 0x00, 0x00, 0xAB};
int n = 0;
const int amostras = 50;
 uint16_t lux[amostras];
uint16_t total = 0;
uint16_t media = 0;
int indice = 0;
void setup(){
  XBee.begin(9600);
  Serial.begin(9600);
  Wire.begin();
  lightMeter.begin();
  n = 0;
  for(int j = 0; j < amostras; j++){
    lux[j] = 0;
  }
}

void loop (){
//receber comando de leitura de luminosidade
//uint16_t lux = lightMeter.readLightLevel();
//Serial.println(lux);
//delay(1000);
  if(XBee.available () > 0){

  rx_data[n] = XBee.read(); //ler da serial
  //Serial.println(rx_data[n]);
  n++;
  delay(100);
    if(n > 15){

      if((rx_data[0] == EMISSOR_LUM[0]) && (rx_data[1] == EMISSOR_LUM[1]) && (rx_data[2] == EMISSOR_LUM[2]) && (rx_data[3] == EMISSOR_LUM[3]) && 
         (rx_data[4] == EMISSOR_LUM[4]) && (rx_data[5] == EMISSOR_LUM[5]) && (rx_data[6] == EMISSOR_LUM[6]) && (rx_data[7] == EMISSOR_LUM[7]) && 
         (rx_data[8] == EMISSOR_LUM[8]) && (rx_data[9] == EMISSOR_LUM[9]) && (rx_data[10] == EMISSOR_LUM[10]) && (rx_data[11] == EMISSOR_LUM[11]) && 
         (rx_data[12] == EMISSOR_LUM[12]) && (rx_data[13] == EMISSOR_LUM[13]) && (rx_data[14] == EMISSOR_LUM[14]) && (rx_data[15] == EMISSOR_LUM[15]))
         {
          
          for(indice = 0; indice < amostras; indice++){
           total = total - lux[indice];
           lux[indice] = lightMeter.readLightLevel();
           delay(50);
           total = total + lux[indice];
           }

         media = total/amostras;
         
          tx_data[0] = 0x0A;
          tx_data[1] = 0x01;
          tx_data[2] = 0x02;
          tx_data[3] = (byte)((media >> 8) & 0xFF); //MSB
          tx_data[4] = (byte)(media & 0xFF); //LSB
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

          //limpa buffer tx
          for(int i = 0; i < 15; i++){
            tx_data[i] = 0; 
            }

          //limpa buffer rx
          for(int i = 0; i < 15; i++){
            rx_data[i] = 0;  
            }
            
         }
         
       n = 0;
      }
    
    }
  
}

