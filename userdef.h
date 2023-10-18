//======================================================
//
//ユーザー定義の処理のヘッダ[userdef.h]
//Author:石原颯馬
//
//======================================================
#ifndef _USERDEF_H_
#define _USERDEF_H_

#define VARIABLE_NUM	(32)	//変数定義最大数

//変数クラス（単体利用可）
class CVariable
{
public:
	//変数構造体
	struct Variable
	{
		int nUseByte;	//変数使用バイト数
		char* pName;	//変数名（動的確保）
		char* pData;	//データ（動的確保）
	};

	//コンストラクタ・デストラクタ
	CVariable() { m_variable = { 0,nullptr,nullptr }; }
	~CVariable(){}

	void Declaration(const int nUseByte, const char* pName, void* pData = nullptr);		//宣言
	void* GetData(void) { return m_variable.pData; }		//データ取得
	char* GetName(void) { return m_variable.pName; }		//名前取得
	void SetData(void* pData) { memcpy(&m_variable.pData, pData, m_variable.nUseByte); }	//設定
	void Release(void);
private:
	Variable m_variable;	//変数
};

//変数マネクラス
class CVariableManager
{
public:
	//コンストラクタ・デストラクタ
	CVariableManager();
	~CVariableManager() {}

	//各種処理
	void ReadUserDefData(const char* pPath);
	void Definition(const char* pName, const int nUseByte, void* pData = nullptr);	//定義
	void ReleaseAll(void);
	CVariable* Declaration(const char* pName, void* pData = nullptr);		//宣言

private:
	void Analysis(char* pCode);								//1行読み込んで分析
	CVariable::Variable* m_definedVariable[VARIABLE_NUM];	//定義済みの変数
	int m_nVariableNum;										//定義済みの変数の個数
};

#endif // !_USERDEF_H_
