//====================================================================
//  �^�C�}�N���X�w�b�_
//  2007/7/27 Pana  ���{����쐬�I���W�i������傫���ύX
//                  Core2Duo����SpeedStep�ɂ��K�؂Ȏ��g�����擾�ł��Ȃ���������
//====================================================================

#ifndef __CPUTimer
#define __CPUTimer
#include<windows.h>

///-----[CPUTimer]-----------------------------------------------------
class CPUTimer
{
protected:
  __int64       CPU_Freq; // CPU���g��
  __int64		StartTime; // �J�E���g�����l
  __int64		LastTime; // �O��J�E���g���̃J�E���g�l

public:
  CPUTimer(void); // �R���X�g���N�^�C�쐬���Ɏ��g���ǂݍ���
  ~CPUTimer(); // �f�X�g���N�^�C�������Ȃ�

  void Start(void); // Elapsed�֐��̂��߂̃J�E���g�����l�̍X�V
  __int64 Elapsed(void); // �J�E���g�����l����̌o�ߎ��Ԃ�Ԃ�
  __int64 Count(void); // �O��J�E���g(Count or Wait�֐�)����̌o�ߎ��Ԃ�Ԃ�

  void Wait(__int64 time); // �����w��ɂ��time[usec]�����҂�
  void CountAndWait(__int64 time); // �����w��ɂ��O��J�E���g����̌o�ߎ��Ԃ��擾���S�̂�time[usec]�����҂�

  __int64 ReadCPU_Freq(void); // �R���X�g���N�^���Ɏ擾����CPU���g����Ԃ�
  void SetCPU_Freq(__int64 freq); // CPU���g�����蓮�Őݒ�

};
#endif