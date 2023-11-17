//======================================================
//
//�I�u�W�F�N�g�iX�j�����̃w�b�_[objectX.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

#include "manager.h"
#include "object.h"
#include "collision.h"
#include "userdef.h"
#include "interface.h"

//�O���錾
class CXModel;
class CManipulationObj;

//�I�u�W�F�N�g�N���X
class CObjectX : public CObject, public IManipulation
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CObjectX(int nPriority = PRIORITY_DEFAULT);																	//�f�t�H���g
	CObjectX(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel, int nPriority = PRIORITY_DEFAULT);	//�I�[�o�[���[�h�i�ʒu�����T�C�Y�j
	virtual ~CObjectX();

	//��{����
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//����
	static CObjectX* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel);

	//�擾
	static CObjectX* GetTop(void) { return m_pTop; }
	CObjectX* GetNext(void) { return m_pNext; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetDepth(void) { return m_fDepth; }
	CObject* GetObj(void) { return this; }
	D3DXVECTOR3 GetMove(void) { return CManager::VEC3_ZERO; }

	//�ݒ�
	void SetModel(CXModel* pModel);
	void SetColor(const bool bEnable, const D3DXCOLOR col = D3DXCOLOR(0xffffffff));

	//�C���^�[�t�F�[�X��
	//����
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	IManipulation::TYPE GetType(void) { return m_type; }
	void Delete(void) { Uninit(); }
	//X�̂�
	CVariable** GetVariable(void) { return &m_apVariable[0]; }
	CXModel* GetModel(void) { return m_pModel; }
	//���C�g�̂݁i�����K�v�Ȃ��j
	D3DXCOLOR GetDiffuse(void) { return D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); }
	void SetDiffuse(const D3DXCOLOR col) {}

	//�g�p���f���P�ʂŏ���
	static void Delete(CXModel* pTarget);

	//���X�g���O
	virtual void Exclusion(void);

private:
	//���f��
	D3DXMATRIX mtxWorld;		//���[���h�}�g��
	CXModel* m_pModel;			//���f��
	D3DXCOLOR m_changeColor;	//�ύX�����F
	bool m_bEnable;				//�F�ύX�L������

	//�ʒu��
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_rot;	//����
	float m_fWidth;		//��
	float m_fHeight;	//����
	float m_fDepth;		//���s
	IManipulation::TYPE m_type;	//���

	//�ǉ��ϐ�
	CVariable* m_apVariable[VARIABLE_NUM];

	//����C���^�[�t�F�[�X
	CManipulationObj* m_pManipObj;

	//���X�g
	static CObjectX* m_pTop;	//�擪�I�u�W�F�N�g
	static CObjectX* m_pCur;	//�Ō���I�u�W�F�N�g
	CObjectX* m_pNext;			//���̃I�u�W�F�N�g
	CObjectX* m_pPrev;			//�O�̃I�u�W�F�N�g
	static int m_nNumAll;		//����
};

#endif // !_OBJECT_H_
