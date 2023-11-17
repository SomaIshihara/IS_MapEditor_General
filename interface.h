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

//�\���I�u�W�F�N�g����C���^�[�t�F�[�X
__interface IManipulation
{
	//��ޗ�
	enum TYPE
	{
		TYPE_OBJX = 0,
		TYPE_CAMERA,
		TYPE_LIGHT,
		TYPE_MAX
	};

	//���ʁi���ׂĂɎ����j
	D3DXVECTOR3 GetPos(void) = 0;
	void SetPos(const D3DXVECTOR3 pos) = 0;
	D3DXVECTOR3 GetRot(void) = 0;
	void SetRot(const D3DXVECTOR3 rot) = 0;
	TYPE GetType(void) = 0;
	void Delete(void) = 0;		//�I�u�W�F�N�g�폜

	//����X�̂݁i����ȊO�K���ɕԂ��΂�����j
	CVariable** GetVariable(void) = 0;
	CXModel* GetModel(void) = 0;

	//���󃉃C�g�̂݁i����ȊO�K���ɕԂ��΂�����j
	D3DXCOLOR GetDiffuse(void) = 0;
	void SetDiffuse(const D3DXCOLOR col) = 0;
};

#endif // !_INTERFACE_H_
