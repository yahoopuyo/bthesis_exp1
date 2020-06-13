//====================================================================
//  �^�C�}�N���X�t�@�C��
//  2007/7/27 Pana  ���{����쐬�I���W�i������傫���ύX
//                  Core2Duo����SpeedStep�ɂ��K�؂Ȏ��g�����擾�ł��Ȃ���������
//====================================================================
#include "CPUTimer.h"


CPUTimer::CPUTimer(void)
{
	::QueryPerformanceFrequency( (LARGE_INTEGER*)&CPU_Freq ); // ������\�n�[�h�E�F�A�J�E���^�̎��g�����擾

	::QueryPerformanceCounter( (LARGE_INTEGER*)&StartTime); // ������\�n�[�h�E�F�A�J�E���^��ǂ�(�J�E���^�����l�ɐݒ�)

	LastTime = StartTime; // Count�֐��̂��߂̃J�E���g�l�̕ۑ�
}


CPUTimer::~CPUTimer(){}


void CPUTimer::Start(void)
{
	::QueryPerformanceCounter( (LARGE_INTEGER*)&StartTime); // ������\�n�[�h�E�F�A�J�E���^��ǂ�(�J�E���^�����l�̍X�V)
}


__int64 CPUTimer::Elapsed(void)
{
	__int64 PresentTime;

	::QueryPerformanceCounter( (LARGE_INTEGER*)&PresentTime); // ������\�n�[�h�E�F�A�J�E���^��ǂ�

	return (PresentTime - StartTime)*1000000/CPU_Freq; // ���g����p���Čv�Z���ꂽ�J�E���^�����l����̌o�ߎ���[��sec]��Ԃ�
}


__int64 CPUTimer::Count(void)
{
	__int64 PresentTime;
	__int64 retval;

	::QueryPerformanceCounter( (LARGE_INTEGER*)&PresentTime); // ������\�n�[�h�E�F�A�J�E���^��ǂ�

	retval = (PresentTime - LastTime)*1000000/CPU_Freq; // ���g����p���Čv�Z���ꂽ�O��Count����̌o�ߎ���[��sec]��Ԃ�
	LastTime = PresentTime; // Count�֐��̂��߂̃J�E���g�l�̕ۑ�

	return retval;
}


void CPUTimer::Wait(__int64 time)
{
	__int64 PresentTime;

	::QueryPerformanceCounter( (LARGE_INTEGER*)&PresentTime); // ������\�n�[�h�E�F�A�J�E���^��ǂ�
	LastTime = PresentTime + (__int64)time*CPU_Freq/1000000; // ���݂̃J�E���^�l�Ƀ��[�U�w��҂�����time�𑫂�����

	do{
		::QueryPerformanceCounter( (LARGE_INTEGER*)&PresentTime); // ������\�n�[�h�E�F�A�J�E���^��ǂ�
	}while(LastTime > PresentTime); // ���݂̃J�E���^�l���w��҂����ԂɂȂ�܂ŌJ��Ԃ�

}


void CPUTimer::CountAndWait(__int64 time)
{
	__int64 TempTime;

	TempTime = Count(); // �o�ߎ��Ԃ��J�E���g����
	if(TempTime > time) return;	// �o�ߎ��Ԃ��w�莞�Ԃ���������Α҂����ɏ�����Ԃ�
	Wait(time - TempTime); // �w�莞�ԁ|�o�ߎ��Ԃ����҂�
}


__int64 CPUTimer::ReadCPU_Freq(void)
{
	return CPU_Freq; // ���݃N���X�����o�ɕۑ�����Ă�����g����Ԃ�
}


void CPUTimer::SetCPU_Freq(__int64 freq)
{
	CPU_Freq = freq; // ���g�����N���X�����o�ɐݒ肷��
}