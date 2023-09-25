//======================================================
//
//���[�U�[��`�̏����̃w�b�_[userdef.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _USERDEF_H_
#define _USERDEF_H_

#define VARIABLE_NUM	(32)	//�ϐ���`�ő吔

class CUserDefinition
{
public:
	//�ϐ��\����
	struct Variable
	{
		char* pName;	//�ϐ����i���I�m�ہj
		int nUseByte;	//�ϐ��g�p�o�C�g��
		char* pData;	//�f�[�^�i���I�m�ہj
	};

	//�R���X�g���N�^�E�f�X�g���N�^
	CUserDefinition();
	~CUserDefinition();

	//�e�폈��
	void Get(const char* pName, void* pData);	//�擾�i�ʏ�j
	void Get(const int nDefNum, void* pData);	//�擾�i�S�擾�ȂǗp�j
	void Set(const char* pName, void* pData);	//�ݒ�i�ʏ�j
	void Set(const int nDefNum, void* pData);	//�ݒ�i�S�ݒ�ȂǗp�j
	void Declaration(const char* pName, void* pData = nullptr);		//�錾
	static void Analysis(char* pCode);								//1�s�ǂݍ���ŕ���
	static void Definition(const char* pName, const int nUseByte);	//��`
	int Variable_Num(void) { return m_nVariableNum; }				//��`�ςݕϐ��̐�

private:
	static Variable* m_definedVariable[VARIABLE_NUM];	//��`�ς݂̕ϐ�
	Variable* m_variable[VARIABLE_NUM];					//�ϐ�
	static int m_nVariableNum;							//��`�ςݕϐ��̐�
	static int m_nUsingNum;								//���̃N���X���g�p���Ă����
};

#endif // !_USERDEF_H_
