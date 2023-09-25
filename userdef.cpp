//======================================================
//
//ユーザー定義の処理[userdef.cpp]
//Author:石原颯馬
//
//======================================================
#include "precompile.h"
#include "userdef.h"

//=================================
//コンストラクタ
//=================================
CUserDefinition::CUserDefinition()
{
	m_nUsingNum++;
}

//=================================
//デストラクタ
//=================================
CUserDefinition::~CUserDefinition()
{
	m_nUsingNum--;
}

//=================================
//取得（通常）
//=================================
void CUserDefinition::Get(const char* pName, void* pData)
{
	for (int cnt = 0; cnt < m_nVariableNum; cnt++)
	{
		if (m_variable[cnt] != nullptr && strcmp(pName, m_variable[cnt]->pName) == 0)
		{//変数を見つけた
			memcpy(pData, m_variable[cnt]->pData, m_variable[cnt]->nUseByte);
			return;
		}
	}
}

//=================================
//取得（全取得など用）
//=================================
void CUserDefinition::Get(const int nDefNum, void * pData)
{
	if (nDefNum >= 0 && nDefNum < VARIABLE_NUM && m_variable[nDefNum] != nullptr)
	{
		memcpy(pData, m_variable[nDefNum]->pData, m_variable[nDefNum]->nUseByte);
	}
}

//=================================
//設定（通常）
//=================================
void CUserDefinition::Set(const char * pName, void * pData)
{
	for (int cnt = 0; cnt < m_nVariableNum; cnt++)
	{
		if (m_variable[cnt] != nullptr && strcmp(pName, m_variable[cnt]->pName) == 0)
		{//変数を見つけた
			memcpy(m_variable[cnt]->pData, pData, m_variable[cnt]->nUseByte);
			return;
		}
	}
}

//=================================
//設定（全設定など用）
//=================================
void CUserDefinition::Set(const int nDefNum, void * pData)
{
	if (nDefNum >= 0 && nDefNum < VARIABLE_NUM && m_variable[nDefNum] != nullptr)
	{
		memcpy(m_variable[nDefNum]->pData, pData, m_variable[nDefNum]->nUseByte);
	}
}

//=================================
//変数宣言
//=================================
void CUserDefinition::Declaration(const char* pName, void* pData)
{
	for (int cnt = 0; cnt < m_nVariableNum; cnt++)
	{
		if (m_variable[cnt] != nullptr && strcmp(pName, m_definedVariable[cnt]->pName) == 0)
		{//変数を見つけた
			//格納用変数動的確保
			m_variable[cnt] = new Variable;

			//初期化があればする
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

	//何型か確認
	pSprit = strtok(pCode, " =\n");
	if (strcmp(pSprit, "int") == 0)
	{//int型
		nUseByte = 4;
	}
	else if (strcmp(pSprit, "float") == 0)
	{//float型
		nUseByte = 4;
	}
	else if (strcmp(pSprit, "bool") == 0)
	{//bool型
		nUseByte = 1;
	}

	//変数名
	pName = strtok(nullptr, " =\n");

	//変数定義
	Definition(pName, nUseByte);
}

//=================================
//変数定義
//=================================
void CUserDefinition::Definition(const char * pName, const int nUseByte)
{
	if (m_nVariableNum < VARIABLE_NUM)
	{
		//定義用変数動的確保
		m_definedVariable[m_nVariableNum] = new Variable;

		//変数名
		int nLength = strlen(pName) + 1;
		m_definedVariable[m_nVariableNum]->pName = new char[nLength];
		strcpy(m_definedVariable[m_nVariableNum]->pName, pName);

		//バイト数
		m_definedVariable[m_nVariableNum]->nUseByte = nUseByte;

		//個数増やす
		m_nVariableNum++;
	}
}
