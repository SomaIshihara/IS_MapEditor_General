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
CVariableManager::CVariableManager()
{
	for (int cnt = 0; cnt < VARIABLE_NUM; cnt++)
	{
		m_definedVariable[cnt] = nullptr;
	}
	m_nVariableNum = 0;
}

//=================================
//定義変数分析
//=================================
void CVariableManager::Analysis(char* pCode)
{
	int nUseByte = 0;
	char* pType = nullptr;
	char* pName = nullptr;
	char* pDataRaw = nullptr;

	//何型か確認
	pType = strtok(pCode, " =\n");

	//変数名
	pName = strtok(nullptr, " =\n");

	//変数データ
	pDataRaw = strtok(nullptr, " =\n");

	//変数定義
	if (strcmp(pType, "int") == 0)
	{//int型
		nUseByte = 4;

		int nData = 0;
		if (pDataRaw != nullptr)
		{
			nData = atoi(pDataRaw);
		}
		Definition(pName, nUseByte, &nData);
	}
	else if (strcmp(pType, "float") == 0)
	{//float型
		nUseByte = 4;

		float fData = 0;
		if (pDataRaw != nullptr)
		{
			fData = atof(pDataRaw);
		}
		Definition(pName, nUseByte, &fData);
	}
	else if (strcmp(pType, "bool") == 0)
	{//bool型
		nUseByte = 1;

		char bData = 0x00;
		if (pDataRaw != nullptr)
		{
			bData = (strcmp(pDataRaw, "true") == 0) ? 0xff : 0x00;
		}
		Definition(pName, nUseByte, &bData);
	}
}

//=================================
//変数定義
//=================================
void CVariableManager::Definition(const char * pName, const int nUseByte, void* pData)
{
	if (m_nVariableNum < VARIABLE_NUM)
	{
		//定義用変数動的確保
		m_definedVariable[m_nVariableNum] = new CVariable::Variable;

		//変数名
		int nLength = strlen(pName) + 1;
		m_definedVariable[m_nVariableNum]->pName = new char[nLength];
		strcpy(m_definedVariable[m_nVariableNum]->pName, pName);

		//バイト数
		m_definedVariable[m_nVariableNum]->nUseByte = nUseByte;

		//初期値（あれば）
		m_definedVariable[m_nVariableNum]->pData = new char[nUseByte];
		if (pData != nullptr)
		{
			memcpy(m_definedVariable[m_nVariableNum]->pData, pData, nUseByte);
		}

		//個数増やす
		m_nVariableNum++;
	}
}

//=================================
//全破棄
//=================================
void CVariableManager::ReleaseAll(void)
{
	for (int cnt = 0; cnt < m_nVariableNum; cnt++)
	{
		if (m_definedVariable[cnt] != nullptr)
		{
			delete m_definedVariable[cnt];
			m_definedVariable[cnt] = nullptr;
		}
	}
}

//=================================
//変数宣言（変数生成）
//=================================
CVariable* CVariableManager::Declaration(const char * pName, void * pData)
{
	for (int cnt = 0; cnt < m_nVariableNum; cnt++)
	{
		if (m_definedVariable != nullptr && strcmp(&m_definedVariable[cnt]->pName[0], pName) == 0)
		{
			CVariable* pVariable = new CVariable;

			if (pData != nullptr)
			{
				pVariable->Declaration(m_definedVariable[cnt]->nUseByte, &m_definedVariable[cnt]->pName[0], pData);
			}
			else
			{
				pVariable->Declaration(m_definedVariable[cnt]->nUseByte, &m_definedVariable[cnt]->pName[0], m_definedVariable[cnt]->pData);
			}

			return pVariable;
		}
	}

	return nullptr;
}

//=================================
//変数定義ファイル読み込み
//=================================
void CVariableManager::ReadUserDefData(const char * pPath)
{
	FILE *pFile;
	char aCode[128];
	char *pSprit;
	bool bRead = false;
	int nCntInit = 0;	//初期化カウンタ

	//読み込みファイル設定
	pFile = fopen(pPath, "r");

	//敵配置情報を取得
	if (pFile != NULL)
	{
		while (1)
		{
			fgets(&aCode[0], 128, pFile);

			//コメントアウトチェック
			char *pCharPos = strchr(&aCode[0], '#');
			if (pCharPos != nullptr)
			{//strchrの返り値がぬるぽではない
				*pCharPos = '\0';
			}

			//タブ消去
			while (aCode[0] == '\t')
			{
				char aCodeBackup[128];
				strcpy(&aCodeBackup[0], &aCode[0]);
				strcpy(&aCode[0], &aCodeBackup[1]);
			}

			//文字列チェック
			if (aCode[0] == '{')
			{//読み取り開始
				bRead = true;
			}
			else if (aCode[0] == '}')
			{//読み取り終了
				bRead = false;
				break;
			}
			else if (aCode[0] == EOF)
			{//EOFかもしれない
				if (feof(pFile))
				{//いや、これはEOFだ
					bRead = false;
					break;
				}
			}
			else if (bRead == true)
			{//読み取り
				Analysis(&aCode[0]);
			}
		}
	}
	else
	{
		assert(pFile != NULL);
	}

	//ファイル閉じる
	fclose(pFile);
}

//=================================
//変数宣言
//=================================
void CVariable::Declaration(const int nUseByte, const char * pName, void * pData)
{
	//変数サイズ格納
	m_variable.nUseByte = nUseByte;

	//変数名格納
	m_variable.pName = new char[strlen(pName) + 1];
	strcpy(&m_variable.pName[0], pName);

	//変数サイズ設定
	m_variable.pData = new char[nUseByte];

	//変数の値があれば代入
	if (pData != nullptr)
	{//代入
		memcpy(m_variable.pData, pData, nUseByte);
	}
}

//=================================
//変数削除
//=================================
void CVariable::Release(void)
{
	if (m_variable.pName != nullptr)
	{
		delete m_variable.pName;
		m_variable.pName = nullptr;
	}
	if (m_variable.pData != nullptr)
	{
		delete m_variable.pData;
		m_variable.pData = nullptr;
	}

	delete this;
}
