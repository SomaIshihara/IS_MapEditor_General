//======================================================
//
//�G�f�B�^����p�I�u�W�F�N�g�����̃w�b�_[manipulation.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _MANIPULATION_H_
#define _MANIPULATION_H_
#include "main.h"
#include "manager.h"
#include "object.h"
#include "userdef.h"

//�O���錾
class CXModel;
class CVariable;
__interface IManipulation;

//�I�u�W�F�N�g�N���X
class CManipulationObj : public CObject
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CManipulationObj(int nPriority = PRIORITY_DEFAULT);
	virtual ~CManipulationObj();

	//��{����
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//����
	static CManipulationObj* Create(IManipulation* face);

	//�擾
	static CManipulationObj* GetTop(void) { return m_pTop; }
	CManipulationObj* GetNext(void) { return m_pNext; }
	IManipulation* GetInterface(void) { return m_interface; }

	//�I�u�W�F���E��
	void Exclusion(void);

private:
	//���f��
	D3DXMATRIX mtxWorld;	//���[���h�}�g��

	//���X�g
	static CManipulationObj* m_pTop;	//�擪�I�u�W�F�N�g
	static CManipulationObj* m_pCur;	//�Ō���I�u�W�F�N�g
	CManipulationObj* m_pNext;			//���̃I�u�W�F�N�g
	CManipulationObj* m_pPrev;			//�O�̃I�u�W�F�N�g
	static int m_nNumAll;		//����

	//����C���^�[�t�F�[�X
	IManipulation* m_interface;
};

#endif // !_OBJECT_H_
