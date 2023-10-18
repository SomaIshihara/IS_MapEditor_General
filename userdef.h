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
	//型構造体
	enum TYPE
	{
		Integer = 0,
		Float,
		Boolean,
	};

	//変数構造体
	struct Variable
	{
		TYPE type;		//型名
		int nUseByte;	//変数使用バイト数
		char* pName;	//変数名（動的確保）
		char* pData;	//データ（動的確保）
	};

	//コンストラクタ・デストラクタ
	CVariable() { m_variable = { Integer,0,nullptr,nullptr }; }
	~CVariable(){}

	void Declaration(const TYPE type, const char* pName, void* pData = nullptr);		//宣言
	void* GetData(void) { return m_variable.pData; }		//データ取得
	char* GetName(void) { return m_variable.pName; }		//名前取得
	TYPE GetType(void) { return m_variable.type; }			//型取得
	void SetData(void* pData);	//設定
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
	void Definition(const CVariable::TYPE type, const char* pName, void* pData = nullptr);	//定義
	void ReleaseAll(void);
	CVariable* Declaration(const char* pName, void* pData = nullptr);		//宣言
	CVariable::Variable** GetDefinedVariable(void) { return &m_definedVariable[0]; }
	int GetDefinedNum(void) { return m_nVariableNum; }
	void Analysis(char* pCode);								//1行読み込んで分析

private:
	CVariable::Variable* m_definedVariable[VARIABLE_NUM];	//定義済みの変数
	int m_nVariableNum;										//定義済みの変数の個数
};

#endif // !_USERDEF_H_
