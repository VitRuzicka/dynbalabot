/*
#include<Arduino.h>
unsigned long int soucasnaProdleva,predchoziProdleva,prodleva;
int x[15],ch1[15],ch[7],ind;

uint clip(int vstup, int max, int min){
  if(vstup > max){
    return max;
  }
  else if(vstup < min){
    return min;
  }
  else{
    return vstup;
  }

}

void cteni_signalu()  {
soucasnaProdleva=micros();                        //doba od poslední padající hrany
prodleva=soucasnaProdleva-predchoziProdleva;      //výpočet té doby
predchoziProdleva=soucasnaProdleva;               // stará hrana je nová
x[ind]=prodleva;                                //uchování 15 hodnot v poli proměnných
ind=ind+1;                                  
if(ind==15){                 //pokud je promenna plna, nasype se obsah do dalsi docasne promenne ch1
  for(int j=0;j<15;j++) {
    ch1[j]=x[j];}              //přepis z docasne proměnné do dalsi docasne promenne, kvuli formatovani
  ind=0;
}
}
void zpracovani_signalu(){
int i,j,k=0;
  for(k=14;k>-1;k--){
    if(ch1[k]>10000){         //detekce 10 000ms dlouhé pauzy v proměnné a rozdělení na pakety s časy  
    j=k;}   
  }                   
  for(i=1;i<=6;i++){
    ch[i]=(ch1[i+j]-1000);}   //dosad sest hodnot bez separacni pauzy
  }

void setup() {
Serial.begin(115200);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(18), cteni_signalu, FALLING); //ve skutecnosti se jedna o vzestupnou hranu protoze pin je v PULLUP rezimu
}

void loop() {
zpracovani_signalu(); //nutno zavolat pred pouzitim signalu z promenne pro "update udaju"

Serial.print(clip(ch[1], 1500, 0));Serial.print("\t");
Serial.print(ch[2]);Serial.print("\t");
Serial.print(ch[3]);Serial.print("\t");
Serial.print(ch[4]);Serial.print("\t");
Serial.print(ch[5]);Serial.print("\t");
Serial.print(ch[6]);Serial.print("\n");

delay(100);
}
*/