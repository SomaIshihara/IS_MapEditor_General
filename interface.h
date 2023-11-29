//======================================================
//
//�C���^�[�t�F�[�X�ꗗ[interface.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _INTERFACE_H_
#define _INTERFACE_H_

//�O���錾
class CVariable;
class CXModel;
class CCollision;

//�\���I�u�W�F�N�g����C���^�[�t�F�[�X
__interface IManipulation
{
	//��ޗ�
	enum TYPE
	{
		TYPE_OBJX = 0,
		TYPE_CAMERA,
		TYPE_LIGHT,
		TYPE_MESHFIELD,
		TYPE_BILLBOARD,
		TYPE_MAX
	};

	//���ʁi���ׂĂɎ����j
	D3DXVECTOR3 GetPos(void) = 0;
	void SetPos(const D3DXVECTOR3 pos) = 0;
	D3DXVECTOR3 GetRot(void) = 0;
	void SetRot(const D3DXVECTOR3 rot) = 0;
	TYPE GetType(void) = 0;
	CCollision* GetCollision(void) = 0;
	void Delete(void) = 0;		//�I�u�W�F�N�g�폜

	//����X�̂݁i����ȊO�K���ɕԂ��΂�����j
	CVariable** GetVariable(void) = 0;
	CXModel* GetModel(void) = 0;

	//���󃉃C�g�̂݁i����ȊO�K���ɕԂ��΂�����j
	D3DXCOLOR GetDiffuse(void) = 0;
	void SetDiffuse(const D3DXCOLOR col) = 0;

	//���b�V���t�B�[���h�ƃr���{�[�h��X�̂݁i����ȊO�K���ɕԂ��΂�����j
	void SetSize(const float fWidth, const float fHeight, const float fDepth) = 0;
	float GetWidth(void) = 0;
	float GetHeight(void) = 0;
	float GetDepth(void) = 0;

	//���󃁃b�V���t�B�[���h�̂݁i����ȊO�K���ɕԂ��΂�����j
	void SetBlockNum(const int nWidth, const int nDepth) = 0;
	int GetBlockWidth(void) = 0;
	int GetBlockDepth(void) = 0;
};

#endif // !_INTERFACE_H_
