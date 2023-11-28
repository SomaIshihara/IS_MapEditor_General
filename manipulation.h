//======================================================
//
//エディタ操作用オブジェクト処理のヘッダ[manipulation.h]
//Author:石原颯馬
//
//======================================================
#ifndef _MANIPULATION_H_
#define _MANIPULATION_H_
#include "main.h"
#include "manager.h"
#include "object.h"
#include "userdef.h"

//前方宣言
class CXModel;
class CVariable;
__interface IManipulation;

//オブジェクトクラス
class CManipulationObj : public CObject
{
public:
	//コンストラクタ・デストラクタ
	CManipulationObj(int nPriority = PRIORITY_DEFAULT);
	virtual ~CManipulationObj();

	//基本処理
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//生成
	static CManipulationObj* Create(IManipulation* face);

	//取得
	static CManipulationObj* GetTop(void) { return m_pTop; }
	CManipulationObj* GetNext(void) { return m_pNext; }
	IManipulation* GetInterface(void) { return m_interface; }

	//オブジェを殺す
	void Exclusion(void);

private:
	//モデル
	D3DXMATRIX mtxWorld;	//ワールドマトリ

	//リスト
	static CManipulationObj* m_pTop;	//先頭オブジェクト
	static CManipulationObj* m_pCur;	//最後尾オブジェクト
	CManipulationObj* m_pNext;			//次のオブジェクト
	CManipulationObj* m_pPrev;			//前のオブジェクト
	static int m_nNumAll;		//総数

	//操作インターフェース
	IManipulation* m_interface;
};

#endif // !_OBJECT_H_
