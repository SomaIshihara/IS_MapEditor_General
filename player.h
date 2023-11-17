//======================================================
//
//�v���C���[�����̃w�b�_[player.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//�O���錾
class CObjectX;
class CManipulationObj;

//�v���C���[�N���X
class CPlayer
{
public:
	//�ÓIconst
	static const float MOVE_SPEED;
	static const float ROTATE_SPEED;

	//�R���X�g���N�^�E�f�X�g���N�^
	CPlayer();
	~CPlayer();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//�I������
	//void UnsetSelObj(void) { m_pObject = nullptr; }
	void UnsetSelObj(void) { m_pObject = nullptr; }

	//�擾����
	//CObjectX* GetSelObj(void) { return m_pObject; }
	CManipulationObj* GetSelObj(void) { return m_pObject; }

private:
	//�֐�
	void Move(void);		//�J�����ړ�����
	void Rotate(void);		//�J������]����
	void Select(void);		//�I�u�W�F�N�g�I��

	D3DXVECTOR3 m_cursorPos;	//�N���b�N�O�̃J�[�\���ʒu
	//CObjectX* m_pObject;		//�I�����Ă���I�u�W�F�i��
	CManipulationObj* m_pObject;	//�I�����Ă���I�u�W�F
};

#endif