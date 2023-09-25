//======================================================
//
//�v���C���[����[player.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "object.h"
#include "input.h"
#include "camera.h"
#include "objectX.h"
#include "xmodel.h"

//�ÓI�����o�ϐ�
const float CPlayer::MOVE_SPEED = 10.0f;		//�J�����ړ����x
const float CPlayer::ROTATE_SPEED = 0.0012f;	//�}�E�X�ړ��ł̉�]���x

//=================================
//�R���X�g���N�^
//=================================
CPlayer::CPlayer()
{
	m_cursorPos = CManager::VEC3_ZERO;
}

//=================================
//�f�X�g���N�^
//=================================
CPlayer::~CPlayer()
{}

//=================================
//������
//=================================
HRESULT CPlayer::Init(void)
{
	return S_OK;
}

//=================================
//�I��
//=================================
void CPlayer::Uninit(void)
{
	
}

//=================================
//�X�V
//=================================
void CPlayer::Update(void)
{
	CInputMouse* pMouse = CManager::GetInputMouse();	//�}�E�X�擾
	Move();

	if (pMouse->GetPress(CInputMouse::CLICK_RIGHT) == true)
	{//��]
		Rotate();
	}
	else
	{//�}�E�X�J�[�\���ʒu�ۑ�
		m_cursorPos = pMouse->GetPos();
	}

	ImGuiIO& io = ImGui::GetIO();
	if (pMouse->GetTrigger(CInputMouse::CLICK_LEFT) == true && io.WantCaptureMouse == false)
	{//�ʒu����
		Select();
	}
}

//=================================
//�ړ�
//=================================
void CPlayer::Move(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	//�L�[�{�[�h�擾
	CCamera* pCamera = CManager::GetCamera();					//�J�����擾
	D3DXVECTOR3 move = CManager::VEC3_ZERO;
	D3DXVECTOR3 rot = pCamera->GetRot();
	//�ړ�
	if (pKeyboard->GetPress(DIK_A) == true)
	{
		move.x += -cosf(rot.y) * MOVE_SPEED;
		move.z += -sinf(rot.y) * MOVE_SPEED;
	}
	else if (pKeyboard->GetPress(DIK_D) == true)
	{
		move.x += cosf(rot.y) * MOVE_SPEED;
		move.z += sinf(rot.y) * MOVE_SPEED;
	}

	if (pKeyboard->GetPress(DIK_W) == true)
	{
		move.x += -sinf(rot.y) * MOVE_SPEED;
		move.z += cosf(rot.y) * MOVE_SPEED;
	}
	else if (pKeyboard->GetPress(DIK_S) == true)
	{
		move.x += sinf(rot.y) * MOVE_SPEED;
		move.z += -cosf(rot.y) * MOVE_SPEED;
	}

	//�ړ�
	pCamera->SetCameraPos(move);
}

//=================================
//��]
//=================================
void CPlayer::Rotate(void)
{
	CCamera* pCamera = CManager::GetCamera();
	CInputMouse* pMouse = CManager::GetInputMouse();
	D3DXVECTOR3 rot = CManager::VEC3_ZERO;

	D3DXVECTOR3 move = CManager::VEC3_ZERO;
	move.x = pMouse->GetMove().x;
	move.y = pMouse->GetMove().y;

	rot.y -= move.x * ROTATE_SPEED;
	rot.x -= move.y * ROTATE_SPEED;

	pCamera->SetCameraRot(rot);

	//�J�[�\�������̈ʒu�ɖ߂�
	POINT setCursorpos;
	setCursorpos.x = (LONG)m_cursorPos.x;
	setCursorpos.y = (LONG)m_cursorPos.y;
	ClientToScreen(FindWindowA(CLASS_NAME, nullptr), &setCursorpos);
	SetCursorPos(setCursorpos.x, setCursorpos.y);
}

//=================================
//�I��
//=================================
void CPlayer::Select(void)
{
	CInputMouse* mouse = CManager::GetInputMouse();	//�}�E�X�擾

	//�I�u�W�F�N�g�I���i0.0�`1.0�j
	D3DXVECTOR3 posNear = mouse->ConvertClickPosToWorld(0.0f);
	D3DXVECTOR3 posFar = mouse->ConvertClickPosToWorld(1.0f);

	//�߂�����
	CObjectX* pObjectNear = nullptr;
	float fLengthNear = CManager::FLT_ZERO;

	CObjectX* pObject = CObjectX::GetTop();	//���X�g�̍ŏ��������Ă���

	while (pObject != nullptr)
	{//�ʂ�ʂ�ɂȂ�܂ł��
		CObjectX* pObjectNext = pObject->GetNext();	//���̃I�u�W�F�ۑ�
		
		//�{����
		if (pObject->GetModel()->GetCollision().CollisionCheck(posNear, posFar, pObject->GetPos(), pObject->GetRot()))
		{
			float fLength = D3DXVec3Length(&(pObject->GetPos() - posNear));

			if (pObjectNear == nullptr || fLengthNear > fLength)
			{//�߂�
				pObjectNear = pObject;
				fLengthNear = fLength;
			}
		}

		pObject = pObjectNext;	//��������
	}

	if (pObjectNear != nullptr)
	{//��������I���ł���
		m_pObject = pObjectNear;	//�N���b�N�����I�u�W�F�����Ă���
	}
	else
	{//�����I�����ĂȂ�
		m_pObject = nullptr;
	}
}
