//======================================================
//
//GUI�I�u�W�F�N�g�̃w�b�_[gui.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _GUI_H_
#define _GUI_H_

#include "object.h"

//�O���錾
class CXModel;

//�I�u�W�F�N�g�ǉ�GUI�N���X
class CGUIAddObj : public CObject
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CGUIAddObj();
	~CGUIAddObj();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGUIAddObj* Create(void);

	//�擾
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return CManager::FLT_ZERO; }
	float GetHeight(void) { return CManager::FLT_ZERO; }
	float GetDepth(void) { return CManager::FLT_ZERO; }
	int GetModelIdx(void) { return -1; }
private:
	CXModel* m_SelectObj;
	D3DXVECTOR3 m_pos, m_rot;
	bool m_bDragged;
};

//�I�u�W�F�N�g�ύXGUI�N���X
class CGUIChangeObj : public CObject
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CGUIChangeObj();
	~CGUIChangeObj();
	
	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGUIChangeObj* Create(void);

	//�擾
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return CManager::FLT_ZERO; }
	float GetHeight(void) { return CManager::FLT_ZERO; }
	float GetDepth(void) { return CManager::FLT_ZERO; }
	int GetModelIdx(void) { return -1; }
private:
	D3DXVECTOR3 m_pos, m_rot;
	bool m_bDragged;
};

#endif // !_GUI_H_
