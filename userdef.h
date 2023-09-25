//======================================================
//
//ユーザー定義の処理のヘッダ[userdef.h]
//Author:石原颯馬
//
//======================================================
#ifndef _USERDEF_H_
#define _USERDEF_H_

#define VARIABLE_NUM	(32)	//変数定義最大数

class CUserDefinition
{
public:
	//変数構造体
	struct Variable
	{
		char* pName;	//変数名（動的確保）
		int nUseByte;	//変数使用バイト数
		char* pData;	//データ（動的確保）
	};

	//コンストラクタ・デストラクタ
	CUserDefinition();
	~CUserDefinition();

	//各種処理
	void Get(const char* pName, void* pData);	//取得（通常）
	void Get(const int nDefNum, void* pData);	//取得（全取得など用）
	void Set(const char* pName, void* pData);	//設定（通常）
	void Set(const int nDefNum, void* pData);	//設定（全設定など用）
	void Declaration(const char* pName, void* pData = nullptr);		//宣言
	static void Analysis(char* pCode);								//1行読み込んで分析
	static void Definition(const char* pName, const int nUseByte);	//定義
	int Variable_Num(void) { return m_nVariableNum; }				//定義済み変数の数

private:
	static Variable* m_definedVariable[VARIABLE_NUM];	//定義済みの変数
	Variable* m_variable[VARIABLE_NUM];					//変数
	static int m_nVariableNum;							//定義済み変数の数
	static int m_nUsingNum;								//このクラスを使用している個数
};

#endif // !_USERDEF_H_
