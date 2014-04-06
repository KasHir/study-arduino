#include <SPI.h>
#include <TimerOne.h>

//////////////////////////////
// work area
/////////////////////////////

// Buffer
uint8_t ledBuff0[8] = {	0,0,0,0,0,0,0,0 };
uint8_t ledBuff1[8] = {	0,0,0,0,0,0,0,0 };
uint8_t ledBuff2[8] = {	0,0,0,0,0,0,0,0 };
uint8_t ledBuff3[8] = { 0,0,0,0,0,0,0,0 };

// LED pattern
// 各LEDの明るさを0～3
uint8_t ledLv[36] = {
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0
};

///////////////////////////////
/// pin difinition
///////////////////////////////
int latchPin = 5; //PD5  74HC595-pin12 (RCK)
int testPin = 4; //デバッグピン
//digital13(SCK) 74HC595-pin11 (Serial ClocK)
//digital11(MOSI) 74HC595-pin14 (SERial data input)


#define PinOffset 0;

void LvToBuff(uint8_t prg, uint8_t order){
	uint8_t i = 0;
	uint8_t fillStart = 3-ledLv[prg];
	uint8_t real = order + PinOffset;
	uint8_t targetBit  = real % 8;
	uint8_t targetBitGroup = real / 8;
	
	switch(fillStart){
		case 0:
			ledBuff0[targetBitGroup] &= ~_BV(targetBit);
			ledBuff1[targetBitGroup] &= ~_BV(targetBit);
			ledBuff2[targetBitGroup] &= ~_BV(targetBit);
			ledBuff3[targetBitGroup] &= ~_BV(targetBit);
			break;
		case 1:
			ledBuff0[targetBitGroup] &= ~_BV(targetBit);
			ledBuff1[targetBitGroup] &= ~_BV(targetBit);
			ledBuff2[targetBitGroup] |= _BV(targetBit);
			ledBuff3[targetBitGroup] |= _BV(targetBit);
			break;
		case 2:
			ledBuff0[targetBitGroup] &= ~_BV(targetBit);
			ledBuff1[targetBitGroup] |= _BV(targetBit);
			ledBuff2[targetBitGroup] |= _BV(targetBit);
			ledBuff3[targetBitGroup] |= _BV(targetBit);
			break;
		case 3:
			ledBuff0[targetBitGroup] |= _BV(targetBit);
			ledBuff1[targetBitGroup] |= _BV(targetBit);
			ledBuff2[targetBitGroup] |= _BV(targetBit);
			ledBuff3[targetBitGroup] |= _BV(targetBit);
			break;
	}
}

//////////////////////////////////////////////////////
/// ledLvToBuff
///  ハードウェアの配線で調整できるように
//////////////////////////////////////////////////////
void ledLvToBuff(){
	uint8_t real = 0;
	// prg - real
	// 1 - 1
	LvToBuff(real, real);
	real++;
	// 2 - 2
	LvToBuff(real, real);
	real++;
	// 3 - 3
	LvToBuff(real, real);
	real++;
	// 4 - 4
	LvToBuff(real, real);
	real++;
	// 5 - 5
	LvToBuff(real, real);
	real++;
	// 6 - 6
	LvToBuff(real, real);
	real++;
	// 7 - 7
	LvToBuff(real, real);
	real++;
	// 8 - 8
	LvToBuff(real, real);
	real++;
	// 9 - 9
	LvToBuff(real, real);
	real++;
	//10 -10
	LvToBuff(real, real);
	real++;

	//11 -11
	LvToBuff(real, real);
	real++;
	//12 -12
	LvToBuff(real, real);
	real++;
	//13 -13
	LvToBuff(real, real);
	real++;
	//14 -14
	LvToBuff(real, real);
	real++;
	//15 -15
	LvToBuff(real, real);
	real++;
	//16 -16
	LvToBuff(real, real);
	real++;
	//17 -17
	//18 -18
	//19 -19
	//20 -20
}

void shiftOut(){
	PORTD |= _BV(latchPin);	//HIGH
	PORTD &= ~_BV(latchPin);//LOW
}

int setShiftReg(){
	static uint8_t buffNo =0 ;

	switch(buffNo){
		case 0:
			SPI.transfer(ledBuff0[0]);
			SPI.transfer(ledBuff0[1]);
			break;
		case 1:
			SPI.transfer(ledBuff1[0]);
			SPI.transfer(ledBuff1[1]);
			break;
		case 2:
			SPI.transfer(ledBuff2[0]);
			SPI.transfer(ledBuff2[1]);
			break;
		case 3:
			SPI.transfer(ledBuff3[0]);
			SPI.transfer(ledBuff3[1]);
			break;
	}

	if(buffNo > 3){
		buffNo=0;
	}else{
		buffNo++;
	}
	shiftOut();
	return 1;
}

volatile uint8_t timer = 0;
void T1INT(){
	PORTD |= _BV(testPin);	//HIGH, debug
	switch(timer){
		case 0:
			Timer1.initialize(11050);//7906);//6508);
			break;
		case 1:
			Timer1.initialize(5402);//5546);////4636);
			break;
		case 2:
			Timer1.initialize(1600);//3646);//3105);
			break;
		case 3:
			Timer1.initialize(200);//2183);//1900);
			break;
		case 4:
			Timer1.initialize(1000);//1126);//1009);
			break;
		case 5:
			Timer1.initialize(500);//443);//413);
			break;
		case 6:
			Timer1.initialize(500);
			break;
		}
	if(timer>3){
		timer=0;
	}else{
		timer++;
	}
	
	ledLvToBuff();
	setShiftReg();

	PORTD &= ~_BV(testPin);//LOW, debug

}



void setup(){
	pinMode(latchPin,  OUTPUT);
	pinMode(testPin,  OUTPUT);

	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.setDataMode(SPI_MODE0);

	Timer1.initialize(200000);
	Timer1.attachInterrupt(T1INT);

	Serial.begin(115200);
	
}

char c=0;
void loop(){
	if(Serial.available()){
		c = Serial.read() - '0';
	}

//	delay(1000);
		ledLv[0] = c;
		ledLv[1] = c;
		ledLv[2] = c;
		ledLv[3] = c;
		ledLv[4] = c;
		ledLv[5] = c;
		ledLv[6] = c;
		ledLv[7] = c;
	
//	delay(1000);
		ledLv[8] = c;
		ledLv[9] = c;
		ledLv[10] = c;
		ledLv[11] = c;
		ledLv[12] = c;
		ledLv[13] = c;
		ledLv[14] = c;
		ledLv[15] = c;
	/*
	delay(1000);
		ledLv[0] = c;
		ledLv[1] = c;
		ledLv[2] = c;
		ledLv[3] = c;
		ledLv[4] = c;
		ledLv[5] = c;
		ledLv[6] = c;
		ledLv[7] = c;
	delay(1000);
		ledLv[0] = 0;
		ledLv[1] = 0;
		ledLv[2] = 0;
		ledLv[3] = 0;
		ledLv[4] = 0;
		ledLv[5] = 0;
		ledLv[6] = 0;
		ledLv[7] = 0;
	*/
}
