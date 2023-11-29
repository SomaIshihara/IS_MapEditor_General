//======================================================
//
//���b�V���t�B�[���h�����̃w�b�_[meshField.h
//Author:�Ό��D�n
//
//======================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "object.h"
#include "manager.h"
#include "texture.h"
#include "collision.h"
#include "interface.h"

//�O���錾
class CManipulationObj;

//���b�V���t�B�[���h�N���X
class CMeshField : public CObject, public IManipulation
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CMeshField();
	~CMeshField();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//����
	static CMeshField* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fDepth,
		const int nBlockWidth,const int nBlockDepth);

	//�e�N�X�`���ݒ�
	void BindTexture(int nIdx) { m_nIdxTexture = nIdx; }

	//�ݒ�
	void SetTexType(const CTexture::TYPE type) { m_texType = type; }

	//�擾
	int GetIdxTexture(void) { return m_nIdxTexture; }
	int GetModelIdx(void) { return -1; }
	CManipulationObj* GetManipulationObj(void) { return m_pManipObj; }
	static CMeshField* GetTop(void) { return m_pTop; }
	CMeshField* GetNext(void) { return m_pNext; }

	//�C���^�[�t�F�[�X��
	//����
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	IManipulation::TYPE GetType(void) { return m_type; }
	CCollision* GetCollision(void) { return &m_collision; }
	void Delete(void) { Uninit(); }
	//X�̂݁i�����K�v�Ȃ��j
	CVariable** GetVariable(void) { return nullptr; }
	CXModel* GetModel(void) { return nullptr; }
	//���C�g�̂݁i�����K�v�Ȃ��j
	D3DXCOLOR GetDiffuse(void) { return D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); }
	void SetDiffuse(const D3DXCOLOR col) {}
	//���b�V���t�B�[���h�ƃr���{�[�h��X�̂�
	void SetSize(const float fWidth, const float fHeight, const float fDepth) { m_fWidth = fWidth; m_fDepth = fDepth; SetVtxBuff(); }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return 0.0f; }
	float GetDepth(void) { return m_fDepth; }
	//���b�V���t�B�[���h�̂�
	void SetBlockNum(const int nWidth, const int nDepth) { m_nBlockWidth = nWidth; m_nBlockDepth = nDepth; SetVtxNum(); }
	int GetBlockWidth(void) { return m_nBlockWidth; }
	int GetBlockDepth(void) { return m_nBlockDepth; }

	//�������߂�
	float GetHeight(D3DXVECTOR3 posNew);

	//���O
	void Exclusion(void);
private:
	//�֐�
	void CreateBuff(void);
	void ReleaseBuff(void);
	void SetVtxBuff(void);
	void SetVtxNum(void);
	void SetIdxBuff(void);
	WORD* SetIdxSwaingField(int nCntWidth, WORD *pIdx);

	//�ϐ�
	int m_nIdxTexture;					//�e�N�X�`���ԍ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxbuff;	//���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//�C���f�b�N�X�o�b�t�@
	D3DXMATRIX m_mtxWorld;				//���[���h�}�g��
	CTexture::TYPE m_texType;			//�e�N�X�`���^�C�v
	IManipulation::TYPE m_type;			//���
	CCollision m_collision;				//�����蔻��

	//����C���^�[�t�F�[�X
	CManipulationObj* m_pManipObj;

	//�ʒu��
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_rot;	//����
	float m_fWidth;		//��
	float m_fDepth;		//���s
	int m_nBlockWidth;	//�u���b�N��
	int m_nBlockDepth;	//�u���b�N���s


	//���X�g
	static CMeshField* m_pTop;	//�擪�I�u�W�F�N�g
	static CMeshField* m_pCur;	//�Ō���I�u�W�F�N�g
	CMeshField* m_pNext;			//���̃I�u�W�F�N�g
	CMeshField* m_pPrev;			//�O�̃I�u�W�F�N�g
	static int m_nNumAll;		//����
};

#endif // !_MESHFIELD_H_
