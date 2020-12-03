/* 
 * Author: Akila Uyanwatta
 *
 * Created on December 21, 2017, 3:41 PM
 */
#define _XTAL_FREQ 8000000
#include <xc.h>
#include <pic16f877a.h>
//#include "UART.h"
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
char UART_Init(const long int baudrate) {
    unsigned int x;
   // x = (_XTAL_FREQ - baudrate * 64) / (baudrate * 64); //SPBRG for Low Baud Rate
   // if (x > 255) //If High Baud Rage Required
   // {
        x = (_XTAL_FREQ - baudrate * 16) / (baudrate * 16); //SPBRG for High Baud Rate
        BRGH = 1; //Setting High Baud Rate
    //}
    if (x < 256) {
        SPBRG = x; //Writing SPBRG Register
        SYNC = 0; //Setting Asynchronous Mode, ie UART
        SPEN = 1; //Enables Serial Port
        TRISC7 = 1; //As Prescribed in Datasheet
        TRISC6 = 1; //As Prescribed in Datasheet
        CREN = 1; //Enables Continuous Reception
        TXEN = 1; //Enables Transmission
        return 1; //Returns 1 to indicate Successful Completion
    }
    return 0; //Returns 0 to indicate UART initialization failed
}
void UART_Write(char data) {
    while (!TRMT);
    TXREG = data;
}
char UART_TX_Empty() {
    return TRMT;
}
void UART_Write_Text(char *text) {
    int i;
    for (i = 0; text[i] != '\0'; i++)
        UART_Write(text[i]);
}
char UART_Data_Ready() {
    return RCIF;
}

char UART_Read() {
    while (!RCIF);
    return RCREG;
}
void UART_Read_Text(char Output1,char Output2)
{
  char m;
  int i4=0;
  for(int i4=0;i4<14;i4++)
  {   
      if((i4!=0)&&(!RCIF)){break;}
      else {m = UART_Read();
            if(i4==0){Output1 = m; }
            else if(i4==1){Output2 = m;}
            UART_Write(Output1);
            __delay_ms(30);}
  }
  UART_Write(Output2);
  while(1){if(RCIF){ m=UART_Read();}
  else{break;}
           __delay_ms(50);}
}
char data='0';
int target[2];
int hl[32]; //hit_list
int hc=0;//hit count
int dir; // 1=up,2=right,3=down,4=left
unsigned int stat=0; //0=checker,1=searching for direction
int ckc=0; //ckeck_board_counter
char si1;
char si2;
char *place="5,1-6,1-7,1-8,1-9,1:5,9-6,9-7,9:8,4-8,5-8,6:2,4-2,5-2,6:2,7-2,8\n";
char aed[78]="00";
char aed2[78]="00";
unsigned int temp;//to search in checker board
int q1=1;
char *att =”9249077098035810945445656343343656767472523223252747678785838161412112141618386901907829058950960930”
int mainco;
int counter;
int a=0;
int sl( char a , char b , char list[]){
    int at=0;
    while(1){
        if(list[at]=='\0'){return 0;}
        else if((a==list[at])&&(b==list[at+1])){return 1;}
        else{at=at+2;}
    } 
    return 0;
}
void attack(char u, char i){
    while(1){
    if(u=='0'){PORTD = 0x3F;
            break;}
    else if(u=='1'){PORTB = 0x30;
            break;}
    else if(u=='2'){PORTB = 0x5B;
            break;}
    else if(u=='3'){PORTB = 0x79;
            break;}
    else if(u=='4'){PORTB = 0x74;
            break;}
    else if(u=='5'){PORTB = 0x6D;
            break;}
    else if(u=='6'){PORTB = 0x6F;
            break;}
    else if(u=='7'){PORTB = 0x38;
            break;}
    else if(u=='8'){PORTB = 0x7F;
            break;}
    else if(u=='9'){PORTB = 0x7C;
            break;}
    }        
    while(1){
    if(i=='0'){PORTD = 0x3F;
            break;}
    else if(i=='1'){PORTD = 0x30;
            break;}
    else if(i=='2'){PORTD = 0x5B;
            break;}
    else if(i=='3'){PORTD = 0x79;
            break;}
    else if(i=='4'){PORTD = 0x74;
            break;}
    else if(i=='5'){PORTD = 0x6D;
            break;}
    else if(i=='6'){PORTD = 0x6F;
            break;}
    else if(i=='7'){PORTD = 0x38;
            break;}
    else if(i=='8'){PORTD = 0x7F;
            break;}
    else if(i=='9'){PORTD = 0x7C;
            break;}
    }               
    char tempo[]="ATTACK 0,0\n";
    tempo[7]=u;
    tempo[9]=i;
    int counter2=0;
    for(counter2=0;counter2<50;counter2++){
        while(!TRMT);
        TXREG = tempo[counter2];
        if(tempo[counter2]=='\n'){
        break;
            }
        }
}
void main() {
    TRISB = 0x00; //B as Output PIN
    TRISD = 0x00;
    UART_Init(19200);
    PORTB=0x00;
    PORTD=0x00;
    while(1) {
        counter=0;
        if(data=='0'){
            counter=0;
            for (counter=0;counter<14;counter++){
                data= UART_Read();
                if (data=='\n'){
                    break;
                } else if (counter==0){
                    si1=data;    
                } else if (counter==1){
                    si2=data;
                }
            }
        }
        if(si1=='S' && si2=='E'){
            int counter2=0;
            for(counter2=0;counter2<69;counter2++){
                while(!TRMT);
                TXREG = place[counter2];
                if(place[counter2]=='\n'){
                    break;
                }
            }
            data='0';
            si1='0';
            si2='0';
        }else if(si1=='A' && si2=='T'){
            data='0';
            si1='0';
            si2='0';
            //mainco++;
        }else if(si1=='N' && si2=='E'){
            if(stat==0){ 
                temp=0;
                int temp1;
                
                if(a<78){
                    while(1){temp1=sl(att[ckc],att[ckc+1],aed);
                            if (temp1==0){break;}
                            else{ckc=ckc+2;}
                            }
                        attack(att[ckc],att[ckc+1]);
                        aed[a]=att[ckc];
                        aed[a+1]=att[ckc+1];
                        }
                else{temp=0;
                    int temp2;
                    while(1){temp1=sl(att[ckc],att[ckc+1],aed);
                             temp2=sl(att[ckc],att[ckc+1],aed2);
                             if(temp1 || temp2){ckc=ckc+2;}
                             else{break;}
                            }
                                attack(att[ckc],att[ckc+1]);
                                aed2[a-78]=att[ckc];
                                aed2[a-77]=att[ckc+1];      
                          }
                ckc=ckc+2;
            }
            if(stat==1){
                int fire1;
                int fire2;
                //fire in direction
                //up direction
                while(dir==1){
                    fire1=target[0]-q1;
                    fire2=target[1];
                    int rule1=sl(fire1,fire2,hl);
                    int rule2=sl(fire1,fire2,aed);
                    int rule3=sl(fire1,fire2,aed2);
                    if(target[0]-q1+1=='0'){
                        dir=2;
                        q1=1;
                        break;
                    }
                    else if(rule1){q1++;}
                    else if((rule2||rule3) && (!rule1)){dir++; 
                                                        q1=1;}
                    else{
                        attack(target[0]-q1,target[1]);
                        if(a<78){
                            aed[a]=target[0]-q1;
                            aed[a+1]=target[1];
                                }
                        else{
                            aed2[a-78]=target[0]-q1;
                            aed2[a-77]=target[1];
                                }
                            q1++;
                            break;
                        }
                }
                //right direction 
                while(dir==2){
                    fire1=target[0];
                    fire2=target[1]+q1;
                    int rule1=sl(fire1,fire2,hl);
                    int rule2=sl(fire1,fire2,aed);
                    int rule3=sl(fire1,fire2,aed2);
                    
                    if(target[1]+q1-9=='1'){
                        dir=3;
                        q1=1;
                        break;
                    }
                    else if(rule1){q1++;}
                    else if((rule2||rule3) && (!rule1)){dir++;
                                                        q1=1;}
                    else{
                        attack(target[0],target[1]+q1);
                        if(a<78){
                            aed[a]=target[0];
                            aed[a+1]=target[1]+q1;
                                }
                        else{
                            aed2[a-78]=target[0];
                            aed2[a-77]=target[1]+q1;
                                }
                        q1++;
                        break;
                        }
                }
                //bottom direction
                while(dir==3){
                    fire1=target[0]+q1;
                    fire2=target[1];
                    int rule1=sl(fire1,fire2,hl);
                    int rule2=sl(fire1,fire2,aed);
                    int rule3=sl(fire1,fire2,aed2);
                    
                    if(target[0]+q1-9=='1'){
                        dir=4;
                        q1=1;
                        break;
                    }
                    else if(rule1){q1++;}
                    else if((rule2||rule3) && (!rule1)){dir++;
                                                        q1=1;}
                    else{
                        attack(target[0]+q1,target[1]);
                        if(a<78){
                            aed[a]=target[0]+q1;
                            aed[a+1]=target[1];
                            }
                        else{
                            aed2[a-78]=target[0]+q1;
                            aed2[a-77]=target[1];
                            }
                        q1++;
                        break;
                        }
                }
                //left direction
                while(dir==4){
                    if(target[1]-q1+1=='0'){
                        //UART_Write('a');
                        dir=3;
                        q1=1;
                        break;
                    }
                    else{
                        attack(target[0],target[1]-q1);
                        if(a<78){
                            aed[a]=target[0];
                            aed[a+1]=target[1]-q1;
                        }
                        else{
                            aed2[a-78]=target[0];
                            aed2[a-77]=target[1]-q1;
                        }
                        q1++;
                        break;
                        }
                }
            }
            data='0';
            si1='0';
            si2='0';
            a=a+2;
        
        } else if(si1=='H' && si2=='I'){
            if(stat==0){//changed stat==0
                stat=1;
                if(a<88){
                    target[0]=aed[a-2];
                    target[1]=aed[a-1];
                    dir=1;
                    hl[hc]=aed[a-2];
                    hl[hc+1]=aed[a-1];
                    }
                else{
                    target[0]=aed2[a-80];
                    target[1]=aed2[a-79];
                    dir=1;
                    hl[hc]=aed[a-80];
                    hl[hc+1]=aed[a-79];
                    }
            }
            
            data='0';
            si1='0';
            si2='0';
            hc=hc+2;
            
            //mainco++;
        } else if(si1=='M' && si2=='I'){
            if(stat==1){
            dir++;
            q1=1;}
            
            data='0';
            si1='0';
            si2='0';
            
            //mainco++;
        } else if(si1=='S' && si2=='U'){
            stat=0;
            q1=1;
            
            data='0';
            si1='0';
            si2='0';
       }
}
}
