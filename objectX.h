//======================================================
//
//オブジェクト（X）処理のヘッダ[objectX.h]
//Author:石原颯馬
//
//======================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_
#include "main.h"
#include "manager.h"
#include "object.h"
#include "userdef.h"

//前方宣言
class CXModel;

//オブジェクトクラス
class CObjectX : public CObject
{
public:
	//コンストラクタ・デストラクタ
	CObjectX(int nPriority = PRIORITY_DEFAULT);
	virtual ~CObjectX();

	//基本処理
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//生成
	static CObjectX* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel);

	//取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return CManager::FLT_ZERO; }
	float GetHeight(void) { return CManager::FLT_ZERO; }
	float GetDepth(void) { return CManager::FLT_ZERO; }
	static CObjectX* GetTop(void) { return m_pTop; }
	CObjectX* GetNext(void) { return m_pNext; }
	CXModel* GetModel(void) { return m_pModel; }
	CVariable** GetVariable(void) { return &m_apVariable[0]; }

	//設定
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }

	//使用モデル単位で消す
	static void Delete(CXModel* pTarget);

	//死亡フラグが立っているオブジェを殺す
	static void Exclusion(void);

private:
	//モデル
	D3DXMATRIX mtxWorld;	//ワールドマトリ
	CXModel* m_pModel;		//モデル

	//位置類
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き

	//追加変数
	CVariable* m_apVariable[VARIABLE_NUM];

	//リスト
	static CObjectX* m_pTop;	//先頭オブジェクト
	static CObjectX* m_pCur;	//最後尾オブジェクト
	CObjectX* m_pNext;			//次のオブジェクト
	CObjectX* m_pPrev;			//前のオブジェクト
	bool m_bExclusion;			//除外フラグ
	static int m_nNumAll;		//総数
};

#endif // !_OBJECT_H_
