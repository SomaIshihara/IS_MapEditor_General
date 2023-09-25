//======================================================
//
//���[�U�[��`�̏���[userdef.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "precompile.h"
#include "userdef.h"

//=================================
//�R���X�g���N�^
//=================================
CUserDefinition::CUserDefinition()
{
	m_nUsingNum++;
}

//=================================
//�f�X�g���N�^
//=================================
CUserDefinition::~CUserDefinition()
{
	m_nUsingNum--;
}

//=================================
//�擾�i�ʏ�j
//=================================
void CUserDefinition::Get(const char* pName, void* pData)
{
	for (int cnt = 0; cnt < m_nVariableNum; cnt++)
	{
		if (m_variable[cnt] != nullptr && strcmp(pName, m_variable[cnt]->pName) == 0)
		{//�ϐ���������
			memcpy(pData, m_variable[cnt]->pData, m_variable[cnt]->nUseByte);
			return;
		}
	}
}

//=================================
//�擾�i�S�擾�ȂǗp�j
//=================================
void CUserDefinition::Get(const int nDefNum, void * pData)
{
	if (nDefNum >= 0 && nDefNum < VARIABLE_NUM && m_variable[nDefNum] != nullptr)
	{
		memcpy(pData, m_variable[nDefNum]->pData, m_variable[nDefNum]->nUseByte);
	}
}

//=================================
//�ݒ�i�ʏ�j
//=================================
void CUserDefinition::Set(const char * pName, void * pData)
{
	for (int cnt = 0; cnt < m_nVariableNum; cnt++)
	{
		if (m_variable[cnt] != nullptr && strcmp(pName, m_variable[cnt]->pName) == 0)
		{//�ϐ���������
			memcpy(m_variable[cnt]->pData, pData, m_variable[cnt]->nUseByte);
			return;
		}
	}
}

//=================================
//�ݒ�i�S�ݒ�ȂǗp�j
//=================================
void CUserDefinition::Set(const int nDefNum, void * pData)
{
	if (nDefNum >= 0 && nDefNum < VARIABLE_NUM && m_variable[nDefNum] != nullptr)
	{
		memcpy(m_variable[nDefNum]->pData, pData, m_variable[nDefNum]->nUseByte);
	}
}

//=================================
//�ϐ��錾
//=================================
void CUserDefinition::Declaration(const char* pName, void* pData)
{
	for (int cnt = 0; cnt < m_nVariableNum; cnt++)
	{
		if (m_variable[cnt] != nullptr && strcmp(pName, m_definedVariable[cnt]->pName) == 0)
		{//�ϐ���������
			//�i�[�p�ϐ����I�m��
			m_variable[cnt] = new Variable;

			//������������΂���
			if (pData != nullptr)
			{
				Set(cnt, pData);
			}

			return;
		}
	}
}

void CUserDefinition::Analysis(char* pCode)
{
	int nUseByte = 0;
	char* pSprit = nullptr;
	char* pName = nullptr;

	//���^���m�F
	pSprit = strtok(pCode, " =\n");
	if (strcmp(pSprit, "int") == 0)
	{//int�^
		nUseByte = 4;
	}
	else if (strcmp(pSprit, "float") == 0)
	{//float�^
		nUseByte = 4;
	}
	else if (strcmp(pSprit, "bool") == 0)
	{//bool�^
		nUseByte = 1;
	}

	//�ϐ���
	pName = strtok(nullptr, " =\n");

	//�ϐ���`
	Definition(pName, nUseByte);
}

//=================================
//�ϐ���`
//=================================
void CUserDefinition::Definition(const char * pName, const int nUseByte)
{
	if (m_nVariableNum < VARIABLE_NUM)
	{
		//��`�p�ϐ����I�m��
		m_definedVariable[m_nVariableNum] = new Variable;

		//�ϐ���
		int nLength = strlen(pName) + 1;
		m_definedVariable[m_nVariableNum]->pName = new char[nLength];
		strcpy(m_definedVariable[m_nVariableNum]->pName, pName);

		//�o�C�g��
		m_definedVariable[m_nVariableNum]->nUseByte = nUseByte;

		//�����₷
		m_nVariableNum++;
	}
}
