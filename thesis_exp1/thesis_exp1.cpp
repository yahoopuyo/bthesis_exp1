//
//動かないとき
//①使ってるポートが違う（シリアル通信ができてない！）
//このコードの最後のほうにある「hCom = new COMPORT( "COM4", 19200);」のCOM～の数字を変える必要があります
//数字の確認はコントロールパネル>ハードウェアとサウンド>デバイスマネージャー>ポート
//のところを，USB抜き差しして確認してみてください（Windows7準拠の説明）
//②設定が違う
//(1)VS2010cppを使ってください，2013とか使ってもだめっぽい気がします
//(2)VS上部のタブのプロジェクト>(プロジェクト名)のプロパティ>構成プロパティ>全般>プロジェクトの既定値>文字セット
//を，「マルチバイト文字セットを私用する」に変更してください

#include <stdio.h>
#include "stdafx.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <math.h>
#include <conio.h>
#include <time.h>
#include <vector>
#include <random>
#include <numeric>
#include <fstream>

#include "CPUTimer.h"
#include "CPUTimer.cpp"
#include <mmsystem.h>

#include "serial.h"

using namespace std;

#define PI 3.141592


COMPORT	*hCom; //COMの使用
COMPORT	*hCom2; //COMの使用



CPUTimer myTimer;
LARGE_INTEGER g_liFreq;
LARGE_INTEGER g_liStart;
LARGE_INTEGER g_liTmp;
LARGE_INTEGER g_liEnd;

ofstream ofs;

int get_rand( int min_val, int max_val );

void GvsOutPut(char chan, int Value);

void OutPut(int val);

void OutPutP(int val);	//no print

void OutPutTrap(int amp,int pw,int dir);	//台形波チック

void shuffle( int* array, int size );	//配列のシャッフル


int PEST(bool write)	//閾値求める１
{
	printf("PEST mode\n");
	double i_r_l=0; //bottom limit
	int point = 500;	//starts from 500
	int pre_point=0;
	int pw = 2000000; //1 sec
	int tol = 300;	//tolerance starts from 600
	double tol_dec_rate = 0.8;
	int threshold = 58;
	char pp=0;

	while (tol > threshold)
	{
		if( _kbhit() ){
			pp = _getch();
		}
		
		if(pp == 'n'){
			if (point < pre_point) //switched direction
			{
				tol *= tol_dec_rate;
			}
			pre_point = point;
			point += tol;
			if(write) ofs << pre_point << "," << 'n' << endl;
			printf("%d\n",point);
		}
		if(pp == 'y'){
			if (point > pre_point) //switched direction
			{
				tol *= tol_dec_rate;
			}
			pre_point = point;
			point -= tol;
			if(write) ofs << pre_point << "," << 'y' << endl;
			printf("%d\n",point);
		}
		if(pp == 'q'){	//escape reset
			printf("keyboard interruption at point = %d",point);
			OutPut(0);
			break;
		}
		if(pp == 't'){	//
			int val = 0;
			while(val < point){
				val = val + 100;
				if(val > point) val = point;
				OutPut(val);
				}
			myTimer.Start();
			myTimer.Wait(pw);
			OutPut(0);
		}
		if(pp == 'Y'){	//for debug, opposite
			int val = 0;
			while(val < point){
				val = val + 100;
				if(val > point) val = point;
				OutPut(-val);
				}
			myTimer.Start();
			myTimer.Wait(pw);
			OutPut(0);
		}
		pp = 0;
	}
	//i_r_l = (point + pre_point) / 2;
	i_r_l = point;
	if(write) ofs << i_r_l << "," << "threshold" << '\n' << endl;
	return int(i_r_l);
}

int threshold2()
{
	printf("threshold mode\n");
	int lowerbound=500; //bottom limit
	int upperbound=3000;
	int tol = 300;	//tolerance starts from 600
	int threshold_inc=0;
	int threshold_dec=0;
	int pw = 2000000;
	char pp=0;

	int point = lowerbound;

	printf("upper threshold\n");
	while (1)	//increase
	{
		if( _kbhit() ){
			pp = _getch();
		}
		
		if (pp == 'n'){
			printf("%d\n",point);
			point += tol;
		}
		if(pp == 'y'){
			threshold_inc=point;
			printf("upper threshold=%d\n",point);
			break;
		}
		if(pp == 'q'){	//escape reset
			printf("keyboard interruption at point = %d\n",point);
			OutPut(0);
			break;
		}
		if(pp == 't'){	//
			int val = 0;
			while(val < point){
				val = val + 100;
				if(val > point) val = point;
				OutPut(val);
				}
			myTimer.Start();
			myTimer.Wait(pw);
			OutPut(0);
		}
		if(pp == 'Y'){	//for debug, opposite
			int val = 0;
			while(val < point){
				val = val + 100;
				if(val > point) val = point;
				OutPut(-val);
				}
			myTimer.Start();
			myTimer.Wait(pw);
			OutPut(0);
		}
		pp = 0;
	}
	
	printf("lower threshold\n");
	point=upperbound;
	pp=0;
	while (1)	//decrease
	{
		if( _kbhit() ){
			pp = _getch();
		}
		
		if (pp == 'n'){
			threshold_dec=point;
			printf("lower threshold=%d\n",point);
			break;
		}
		if(pp == 'y'){
			printf("%d\n",point);
			point -= tol;
		}
		if(pp == 'q'){	//escape reset
			printf("keyboard interruption at point = %d\n",point);
			OutPut(0);
			break;
		}
		if(pp == 't'){	//
			int val = 0;
			while(val < point){
				val = val + 100;
				if(val > point) val = point;
				OutPut(val);
				}
			myTimer.Start();
			myTimer.Wait(pw);
			OutPut(0);
		}
		if(pp == 'Y'){	//for debug, opposite
			int val = 0;
			while(val < point){
				val = val + 100;
				if(val > point) val = point;
				OutPut(-val);
				}
			myTimer.Start();
			myTimer.Wait(pw);
			OutPut(0);
		}
		pp = 0;
	}

	printf("upper threshold=%d,lower threshold=%d\n",threshold_inc,threshold_dec);
	return threshold_dec;
}

void PWM()	//pwmでためす
{
	printf("pwm mode\n");
	int freq = 10;
	int pw = 50000;//0.05s
	int freq_tol=1;
	int pw_tol = 10000; //0.01sずつ
	int forward=1;
	int amp = 2000;
	char pp = 0;

	while(1){
		if(_kbhit()){
			pp = _getch();

			if(pp=='q'){
				OutPut(0);
				printf("exit pwm mode\n");
				break;
			}
			if(pp=='i'){
				freq+=freq_tol;
				printf("freq = %d\n",freq);
			}
			if(pp=='k'){
				freq-=freq_tol;
				printf("freq = %d\n",freq);
			}
			if(pp=='I'){
				pw+=pw_tol;
				double pw_s = pw * 1.0 / 1000000;
				printf("pw = %lf[s]\n",pw_s);
			}
			if(pp=='K'){
				pw-=pw_tol;
				double pw_s = pw * 1.0 / 1000000;
				printf("pw = %lf[s]\n",pw_s);
			}
			if(pp=='d'){
				forward *= -1;
				printf("direction:%d\n",forward);
			}
			if(pp=='r'){
				double rate;
				cout << "rate" << endl;cin >> rate;
				pw = 1.0 / freq * rate * 1000000;
				double pw_s = pw * 1.0 / 1000000;
				printf("pw = %lf[s]\n",pw_s);
				printf("pw = %d\n",pw);
			}
			if(pp=='t'){
				int dur = 1.0 / freq* 1000000;
				char pp2=0;
				while(1){
					if(_kbhit()) pp2=_getch();
					if(pp2 == '\033') break;
					OutPut(forward*2000);
					myTimer.Start();
					myTimer.Wait(pw);
					OutPut(0);
					myTimer.Start();
					myTimer.Wait(dur-pw);
				}
			}
		}
	}
}


void AC()	//色々実験したりするよう
{	
	printf("Free test Mode\n");
	
	char pp = 0;
	int val = 0;
	int Count = 0, count=0;
	int tol = 50; //tolerance for +,-
	int amp = 2000; //amplitude
	int amp_tol = 100; //tolerance for amplitude
	int pw = 1000000; //pulse width
	int pw_tol = 100; //tolerance for pw

	while(1){
				Count++;

			if( _kbhit() ){
			pp = _getch();
			
			if(pp == 'q') {OutPut(0); printf("exit free test mode\n"); break;}//escape from AC()

			if(pp == 'p'){	//show current
				printf("%d\n",val);
			}
			else if(pp == '\033'){	//escape key set 0
				val=0;
				OutPut(val);
			}
			else if (pp == '+'){	//increase current by tol
				val=val+ tol;
				OutPut(val);
			}
			else if (pp == '-'){	//decrease current by tol
				val=val-50;
				OutPut(val);
			}
			else if (pp == 't'){	//pulse for pw[ms]
				val=amp;
				OutPut(val);
				myTimer.Start();
				myTimer.Wait(pw);
				OutPut(0);
				printf("done\n");
			}
			else if (pp == 'y'){	//oposite pulse for pw[ms]
				val=-amp;
				OutPut(val);
				myTimer.Start();
				myTimer.Wait(pw);
				OutPut(0);
				printf("done\n");
			}
			else if (pp == 'k'){	//decrease amplitude
				amp = abs(amp - amp_tol);
				printf("amplitude = %d\n",amp);
			}
			else if (pp == 'i'){	//increase amplitude
				amp = abs(amp + amp_tol);	
				printf("amplitude = %d\n",amp);
			}
			else if (pp == 'K'){	//decrease pw
				pw	= abs(pw - pw_tol);
				printf("pw = %d\n",pw);
			}
			else if (pp == 'I'){	//increase pw
				pw	= abs(pw + pw_tol);
				printf("pw = %d\n",pw);
			}
			else if (pp == 'b'){
				val=0;
				printf("Back\n");
				while(val < amp){
					val = val + 60;
					OutPut(val*(-1));
				}
				printf("Full\n");
			}
			else if (pp == 'f'){
				val=0;
				printf("Forward\n");
				while(val < amp){
				val = val + 100;
					OutPut(val);
				}
				printf("Full\n");
			}

			else if (pp == 'B'){
				val = 0;
				while(val < amp){
					val = val + 100;
					if(val > amp) val = amp;
					OutPutP(-val);
					}
				myTimer.Start();
				myTimer.Wait(pw);
				OutPut(0);
			}
			else if (pp == 'F'){
				val = 0;
				while(val < amp){
					val = val + 100;
					if(val > amp) val = amp;
					OutPutP(val);
					}
				myTimer.Start();
				myTimer.Wait(pw);
				OutPut(0);
			}
		}	
	}
}

void EXP2(int i_r_in)	//expiriment flow for specific position
{
	printf("Change the current value and record the location of sensation\n\n");
	int i_r=i_r_in;
	int val=i_r_in;
	int two_s = 2000000;
	bool ready=false;
	//①閾値再確認
	cout << "make sure sensation exists at threshold" << endl;
	while(!ready)
	{
		if(_kbhit()){
			switch(_getch())
			{
			case 't':
				OutPutTrap(i_r,two_s,1);
				break;
			case 'n':
				i_r += 10;
				printf("i_r = %d\n",i_r);
				break;
			case 'y':
				ready=true;
				printf("%d[mA] will be used as threshold from now on\n",i_r);
				break;
			case 'q':
				printf("exit experiment mode\n");
				return;
			default:
				break;
			}
		}
	}
	

	//define the order and value of current

	//i th value is defined by eq: val = a^i * i_r where a is a constant.

	const int DATA_POINTS = 4;	//number of datapoints for current
	double a=1.3;
	int current[DATA_POINTS]={0,1,2,3};
	int current_val[DATA_POINTS] = {0,0,0,0};
	string current_name[DATA_POINTS] = {
		"threshold",
		"second from threshold",
		"third from threshold",
		"largest"
	};

	shuffle(current,DATA_POINTS);
	for (int i = 0; i < DATA_POINTS; i++) current_val[i] = (int)i_r * pow(a,i);
	for (int i = 0; i < DATA_POINTS; i++) ofs << current_name[current[i]] << " : " << current_val[current[i]] <<endl;
	ofs << '\n' << endl;

	for (int i=0; i < DATA_POINTS; i++)
	{
		cout << "Stimulation number " << i << " : "<< current_name[current[i]] << "(" << current_val[current[i]]  <<"[mA])"<<endl;
		bool done = false;
		while(!done)
		{
			if(_kbhit()){
			switch(_getch())
			{
			case 't':
				OutPutTrap(current_val[current[i]],two_s,1);
				break;
			case 'n':
				cout << "moving on to next current value" << endl;
				done = true;
				break;
			case 'q':
				printf("exit experiment mode\n");
				return;
			default:
				break;
			}
		}
		}
	}
}

void EXPMAIN()
{
	cout << "Start the experiment\n" << endl;

	//define the order of position
	int pos[6]={0,1,2,3,4,5};
	string pos_name[6] = {
		"ulnar rist",
		"ulnar elbow",
		"ulnar armpit",
		"median rist",
		"median elbow",
		"median armpit"
	};
	shuffle(pos,6);
	cout << "The order of position" << endl;
	for (int i = 0; i < 6; i++) cout <<  pos_name[pos[i]] << endl;
	printf("\n");
	
	for (int i=0; i< 6; i++)
	{
		cout << "Stimulation at " << pos_name[pos[i]] << endl;
		AC();
		cout << "Threshold" << endl;
		ofs << pos_name[pos[i]] << endl;
		int i_r = PEST(true);
		cout << "threshold val = " << i_r << endl;
		EXP2(i_r);
		cout << "Experiment at this position is over\n" << endl; 
	}
	cout << "Experiment done" << endl;
}

void main(void)
{

	//COMポートのオープン
	hCom = NULL;
	hCom = new COMPORT( "\\\\.\\COM3", 9600);//二桁のときは\\\\.\\COMXXにするんや！
	//hCom = new COMPORT( "COM3", 9600);//二桁のときは\\\\.\\COMXXにするんや！
	char mode=0;
	int i_r=0;
	while(1){
		if( _kbhit() ){
			mode = _getch();

			if(mode == '0') AC();
			if(mode == '1') {i_r = PEST(false); printf("i_r = %d\n",i_r);}
			if(mode == '2') PWM();
			if(mode == '3') EXP2(10);
			if(mode == '4') {i_r = threshold2(); printf("i_r = %d\n",i_r);}
			if(mode == '9') {
				/*
				int num;
				string file;
				cout << "Subject number:";
				cin >> num;
				file << "result" << num.ToString() << ".csv";
				*/
				ofs.open("test.csv");
				EXPMAIN();
				ofs.close();
			}
			if(mode == '8') 
			{
				/*
				int num;
				string file;
				cout << "Subject number:";
				cin >> num;
				file << "result" << num << ".csv";
				*/
				ofs.open("test.csv");
				ofs << 20 << "," << 10 << endl;
				ofs.close();
    
			}

	
		}
	}
}





















int get_rand( int min_val, int max_val )
{
	int num = max_val - min_val;
	random_device rd;
	mt19937 mt(rd());
	return mt() % num + min_val;
}

void GvsOutPut(char chan, int Value){

 char buffer[5];
 char Send1[8] = {0,0,0,0,0,0,0,0};
 char Send2[8] = {0,0,0,0,0,0,0,0};
 int i;

 if(chan == '1'){Send1[7]=0;Send1[6]=0;Send1[5]=1;}
    else if(chan == '2'){Send1[7]=0;Send1[6]=1;Send1[5]=0;}
    else if(chan == '3'){Send1[7]=0;Send1[6]=1;Send1[5]=1;}
    else if(chan == '0'){Send1[7]=0;Send1[6]=0;Send1[5]=0;}
 else if(chan == '4'){Send1[7]=1;Send1[6]=0;Send1[5]=0;}
 else if(chan == '5'){Send1[7]=1;Send1[6]=0;Send1[5]=1;}
 else if(chan == '6'){Send1[7]=1;Send1[6]=1;Send1[5]=0;}


 if(Value >= 0) Send1[4] = 1;
 else Value = Value * (-1);

   /* 10進数を2進数に変換 */
 for(i=0;i<=7;i++){
  Send2[i] = Value % 2;
  Value = Value / 2; 
 }
 for(i=0;i<=3;i++){
  Send1[i] = Value % 2;
  Value = Value / 2; 
 }
 buffer[0] = 71;  
 buffer[1] = (char) (Send1[7]*128 + Send1[6]*64 + Send1[5]*32 + Send1[4]*16 + Send1[3]*8 + Send1[2]*4 + Send1[1]*2 + Send1[0]);
 buffer[2] = (char) (Send2[7]*128 + Send2[6]*64 + Send2[5]*32 + Send2[4]*16 + Send2[3]*8 + Send2[2]*4 + Send2[1]*2 + Send2[0]);
 //buffer[0] = 71;
 //buffer[1] = 81;
 
 hCom->transmit( (BYTE*)buffer, 3);
 Sleep(1);

}

void OutPut(int val)
{
		GvsOutPut('1',val);
		printf("%d\n",val);
}

void OutPutP(int val)
{
		GvsOutPut('1',val);
}

void OutPutTrap(int amp,int pw,int dir)	//台形波チック
{
	int val = 0;
	while(val < amp){
		val = val + 100;
		if(val > amp) val = amp;
		OutPutP(val*dir);
		}
	OutPut(val*dir);
	myTimer.Start();
	myTimer.Wait(pw);
	OutPut(0);
}

void shuffle( int* array, int size ) {
    for (int i = 0; i < size; i++) {
        int r = get_rand( i, size );
        int tmp = array[i];
        array[i] = array[r];
        array[r] = tmp;
    }
}