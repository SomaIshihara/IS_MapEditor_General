//======================================================
//
//�I�u�W�F�N�g�i�r���{�[�h�j�����̃w�b�_[objectBillboard.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _OBJECT_BILLBOARD_H_	//���d�C���N���[�h�΍�
#define _OBJECT_BILLBOARD_H_

#include "manager.h"
#include "object.h"
#include "texture.h"

//�I�u�W�F�N�g�N���X
class CObjectBillboard : public CObject
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CObjectBillboard(int nPriority = PRIORITY_DEFAULT);		//�f�t�H���g
	virtual ~CObjectBillboard();

	//��{����
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//����
	static CObjectBillboard* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const int nIdxTexture);

	//�e�N�X�`���ݒ�
	void BindTexture(int nIdx) { m_nIdxTexture = nIdx; }

	//�擾
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return CManager::VEC3_ZERO; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return CManager::FLT_ZERO; }
	float GetDepth(void) { return m_fHeight; }

	//�ݒ�
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }	//�ʒu
	HRESULT SetCol(const D3DXCOLOR col);	//�F
	void SetSize(const float fWidth, const float fDepth);
	void SetTexType(const CTexture::TYPE type) { m_texType = type; }

	//���O
	virtual void Exclusion(void);

private:
	int m_nIdxTexture;					//�e�N�X�`���ԍ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxbuff;	//���_�o�b�t�@
	D3DXMATRIX m_mtxWorld;				//���[���h�}�g��
	CTexture::TYPE m_texType;			//�e�N�X�`���^�C�v

	//���X�g
	static CObjectBillboard* m_pTop;	//�擪�I�u�W�F�N�g
	static CObjectBillboard* m_pCur;	//�Ō���I�u�W�F�N�g
	CObjectBillboard* m_pNext;			//���̃I�u�W�F�N�g
	CObjectBillboard* m_pPrev;			//�O�̃I�u�W�F�N�g
	static int m_nNumAll;		//����

	D3DXVECTOR3 m_pos;	//�ʒu
	float m_fWidth;		//��(X)
	float m_fHeight;		//���s(Z)
};

#endif	//���d