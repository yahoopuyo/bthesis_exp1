//
//�����Ȃ��Ƃ�
//�@�g���Ă�|�[�g���Ⴄ�i�V���A���ʐM���ł��ĂȂ��I�j
//���̃R�[�h�̍Ō�̂ق��ɂ���uhCom = new COMPORT( "COM4", 19200);�v��COM�`�̐�����ς���K�v������܂�
//�����̊m�F�̓R���g���[���p�l��>�n�[�h�E�F�A�ƃT�E���h>�f�o�C�X�}�l�[�W���[>�|�[�g
//�̂Ƃ�����CUSB�����������Ċm�F���Ă݂Ă��������iWindows7�����̐����j
//�A�ݒ肪�Ⴄ
//(1)VS2010cpp���g���Ă��������C2013�Ƃ��g���Ă����߂��ۂ��C�����܂�
//(2)VS�㕔�̃^�u�̃v���W�F�N�g>(�v���W�F�N�g��)�̃v���p�e�B>�\���v���p�e�B>�S��>�v���W�F�N�g�̊���l>�����Z�b�g
//���C�u�}���`�o�C�g�����Z�b�g�����p����v�ɕύX���Ă�������

#include <stdio.h>
#include "stdafx.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <math.h>
#include <conio.h>
#include <time.h>

#include "CPUTimer.h"
#include "CPUTimer.cpp"
#include <mmsystem.h>

#include "serial.h"

#define PI 3.141592


COMPORT	*hCom; //COM�̎g�p
COMPORT	*hCom2; //COM�̎g�p



CPUTimer myTimer;
LARGE_INTEGER g_liFreq;
LARGE_INTEGER g_liStart;
LARGE_INTEGER g_liTmp;
LARGE_INTEGER g_liEnd;



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

   /* 10�i����2�i���ɕϊ� */
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
		/*GvsOutPut('1',val);
		GvsOutPut('2',val);
		GvsOutPut('3',val);
		GvsOutPut('4',val);
		GvsOutPut('5',val);*/
}

void OutPutP(int val)	//no print
{
		GvsOutPut('1',val);
		
}


int PEST()
{
	int Count = 0;
	double i_r=0; //bottom limit
	int point = 500;	//starts from 500
	int pre_point=0;
	int pw = 1000000; //1 sec
	int tol = 300;	//tolerance starts from 600
	double tol_dec_rate = 0.8;
	int threshold = 58;
	char pp=0;

	while (tol > threshold)
	{
		Count++;
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
			printf("%d\n",point);
		}
		if(pp == 'y'){
			if (point > pre_point) //switched direction
			{
				tol *= tol_dec_rate;
			}
			pre_point = point;
			point -= tol;
			printf("%d\n",point);
		}
		if(pp == '\033'){	//escape reset
			printf("keyboard interruption at point = %d",point);
			OutPut(0);
			break;
		}
		if(pp == 't'){
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
		if(pp == 'y'){	//for debug
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
	i_r = (point + pre_point) / 2;
	return int(i_r);
}



void AC()
{	
	
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
				val=amp;
				printf("Back\n");
				OutPut(val*(-1));
			}
			else if (pp == 'F'){
				val=amp;
				printf("Forward\n");
				OutPut(val);
			}
		}	
	}
}

void main(void)
{

	//COM�|�[�g�̃I�[�v��
	hCom = NULL;
	hCom = new COMPORT( "\\\\.\\COM3", 9600);//�񌅂̂Ƃ���\\\\.\\COMXX�ɂ�����I
	//hCom = new COMPORT( "COM3", 9600);//�񌅂̂Ƃ���\\\\.\\COMXX�ɂ�����I
	int i_r;
	i_r = PEST();
	printf("i_r = %d\n",i_r);
	
	AC();
}
