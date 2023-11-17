//======================================================
//
//�G�f�B�^����p�I�u�W�F�N�g����[manipulation.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "manipulation.h"
#include "xmodel.h"
#include "interface.h"
#include <assert.h>

//�ÓI�����o�ϐ�
CManipulationObj* CManipulationObj::m_pTop = nullptr;
CManipulationObj* CManipulationObj::m_pCur = nullptr;
int CManipulationObj::m_nNumAll = 0;

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CManipulationObj::CManipulationObj(int nPriority) : CObject(nPriority)
{
	if (m_pCur == nullptr)
	{//�Ō�������Ȃ��i���Ȃ킿�擪�����Ȃ��j
		m_pTop = this;		//�����擪
		m_pPrev = nullptr;		//�O��N�����Ȃ�
		m_pNext = nullptr;
	}
	else
	{//�Ō��������
		m_pPrev = m_pCur;		//�Ō���������̑O�̃I�u�W�F
		m_pCur->m_pNext = this;	//�Ō���̎��̃I�u�W�F������
		m_pNext = nullptr;			//�����̎��̃I�u�W�F�͂��Ȃ�
	}
	m_pCur = this;				//�����Ō��

	m_nNumAll++;
}

//=================================
//�f�X�g���N�^
//=================================
CManipulationObj::~CManipulationObj()
{
}

//========================
//����������
//========================
HRESULT CManipulationObj::Init(void)
{
	return S_OK;
}

//========================
//�I������
//========================
void CManipulationObj::Uninit(void)
{
	//�������g�j��
	Release();
}

//========================
//�X�V����
//========================
void CManipulationObj::Update(void)
{
	
}

//========================
//�`�揈��
//========================
void CManipulationObj::Draw(void)
{
	
}

//========================
//��������
//========================
CManipulationObj* CManipulationObj::Create(IManipulation* face)
{
	CManipulationObj* pManip = nullptr;

	if (pManip == nullptr)
	{
		//�I�u�W�F�N�g�̐���
		pManip = new CManipulationObj;

		//������
		pManip->Init();
		pManip->m_interface = face;

		return pManip;
	}
	else
	{
		return nullptr;
	}
}

//========================
//���O����
//========================
void CManipulationObj::Exclusion(void)
{
	if (m_pPrev != nullptr)
	{//�O�ɃI�u�W�F������
		m_pPrev->m_pNext = m_pNext;	//�O�̃I�u�W�F�̎��̃I�u�W�F�͎����̎��̃I�u�W�F
	}
	if (m_pNext != nullptr)
	{
		m_pNext->m_pPrev = m_pPrev;	//���̃I�u�W�F�̑O�̃I�u�W�F�͎����̑O�̃I�u�W�F
	}

	if (m_pCur == this)
	{//�Ō���ł���
		m_pCur = m_pPrev;	//�Ō���������̑O�̃I�u�W�F�ɂ���
	}
	if (m_pTop == this)
	{
		m_pTop = m_pNext;	//�擪�������̎��̃I�u�W�F�ɂ���
	}

	//����
	m_nNumAll--;	//�������炷
}