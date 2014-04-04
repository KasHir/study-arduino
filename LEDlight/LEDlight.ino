int ser = 2;
int sck = 3;
int rck = 4;

//ser:シリアル通信　sck: クロック　rck:ラッチ

void dataout(int serial,int clock){
  int i;
  for(i=0; i<8; i++){
    digitalWrite(serial,HIGH);
    
  
    digitalWrite(clock,HIGH);
    digitalWrite(clock,LOW);
  }
  
}


void setup(){
  pinMode(ser,OUTPUT);
  pinMode(sck,OUTPUT);
  pinMode(rck,OUTPUT);
}



void loop(){
  
  int i;
  
  for(i=0; i<8; i++){
    digitalWrite(rck,LOW);
    
    dataout(ser,sck);
    
    digitalWrite(rck,HIGH);
    delay(70);
  }
  
}
