//==========================================
//
//���C�g�֌W�v���O����[light.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"

//�ÓI�����o�ϐ�
CLight* CLight::m_pTop = nullptr;
CLight* CLight::m_pCur = nullptr;
int CLight::m_nNumAll = 0;

//========================
//�R���X�g���N�^
//========================
CLight::CLight()
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
	m_nID = m_nNumAll;
	m_nNumAll++;

	//���C�g���N���A
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));
}

//========================
//�f�X�g���N�^
//========================
CLight::~CLight()
{

}

//========================
//����������
//========================
HRESULT CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�̎擾
	D3DXVECTOR3 vecDir;	//�ݒ�p

	//���C�g���N���A
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));

	//���C�g��ސݒ�
	m_light.Type = D3DLIGHT_DIRECTIONAL;

	//���C�g�g�U���ݒ�
	m_light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���C�g�̕���
	vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	D3DXVec3Normalize(&vecDir, &vecDir);

	m_light.Direction = vecDir;

	//���C�g�ݒ�
	pDevice->SetLight(m_nID, &m_light);

	//���C�g�L����
	pDevice->LightEnable(m_nID, TRUE);

	//�ł���
	return S_OK;
}

//========================
//�I������
//========================
void CLight::Uninit(void)
{
	//��
}

//========================
//�X�V����
//========================
void CLight::Update(void)
{
	//��
}

//========================
//�F�ύX����
//========================
void CLight::SetDiffuse(const D3DXCOLOR col)
{
	m_light.Diffuse = col;
	SetEnable(true);
}

//========================
//�����ύX����
//========================
void CLight::SetDirection(const D3DXVECTOR3 dir)
{
	m_light.Direction = dir;
	SetEnable(true);
}

//========================
//�L���E��������
//========================
void CLight::SetEnable(const bool bEnable)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X�̎擾
	if (bEnable == true)
	{
		//���C�g�ݒ�
		pDevice->SetLight(m_nID, &m_light);

		//���C�g�L����
		pDevice->LightEnable(m_nID, TRUE);
	}
	else
	{
		//���C�g������
		pDevice->LightEnable(m_nID, FALSE);
	}
}

//========================
//���ׂĔj��
//========================
void CLight::ReleaseAll(void)
{
	CLight* pObject = m_pTop;	//�擪������

	while (pObject != nullptr)
	{//�Ō���܂ŉ񂵑�����
		CLight* pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�ۑ�

		if (pObject->m_pPrev != nullptr)
		{//�O�ɃI�u�W�F������
			pObject->m_pPrev->m_pNext = pObject->m_pNext;	//�O�̃I�u�W�F�̎��̃I�u�W�F�͎����̎��̃I�u�W�F
		}
		if (pObject->m_pNext != nullptr)
		{
			pObject->m_pNext->m_pPrev = pObject->m_pPrev;	//���̃I�u�W�F�̑O�̃I�u�W�F�͎����̑O�̃I�u�W�F
		}

		if (m_pCur == pObject)
		{//�Ō���ł���
			m_pCur = pObject->m_pPrev;	//�Ō���������̑O�̃I�u�W�F�ɂ���
		}
		if (m_pTop == pObject)
		{
			m_pTop = pObject->m_pNext;	//�擪�������̎��̃I�u�W�F�ɂ���
		}

		//����
		pObject->Uninit();
		delete pObject;

		pObject = pObjectNext;	//��������
	}
}

//========================
//����
//========================
CLight* CLight::Create(const D3DXVECTOR3 dir, const D3DXCOLOR col)
{
	CLight* pLight = nullptr;

	if (pLight == nullptr)
	{
		//�I�u�W�F�N�g�̐���
		pLight = new CLight;

		//������
		pLight->Init();
		pLight->SetDirection(dir);
		pLight->SetDiffuse(col);

		return pLight;
	}
	else
	{
		return nullptr;
	}
}
