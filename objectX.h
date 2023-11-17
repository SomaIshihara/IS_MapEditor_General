//======================================================
//
//オブジェクト（X）処理のヘッダ[objectX.h]
//Author:石原颯馬
//
//======================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

#include "manager.h"
#include "object.h"
#include "collision.h"
#include "userdef.h"
#include "interface.h"

//前方宣言
class CXModel;
class CManipulationObj;

//オブジェクトクラス
class CObjectX : public CObject, public IManipulation
{
public:
	//コンストラクタ・デストラクタ
	CObjectX(int nPriority = PRIORITY_DEFAULT);																	//デフォルト
	CObjectX(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel, int nPriority = PRIORITY_DEFAULT);	//オーバーロード（位置向きサイズ）
	virtual ~CObjectX();

	//基本処理
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//生成
	static CObjectX* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel);

	//取得
	static CObjectX* GetTop(void) { return m_pTop; }
	CObjectX* GetNext(void) { return m_pNext; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetDepth(void) { return m_fDepth; }
	CObject* GetObj(void) { return this; }
	D3DXVECTOR3 GetMove(void) { return CManager::VEC3_ZERO; }

	//設定
	void SetModel(CXModel* pModel);
	void SetColor(const bool bEnable, const D3DXCOLOR col = D3DXCOLOR(0xffffffff));

	//インターフェース分
	//共通
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	IManipulation::TYPE GetType(void) { return m_type; }
	void Delete(void) { Uninit(); }
	//Xのみ
	CVariable** GetVariable(void) { return &m_apVariable[0]; }
	CXModel* GetModel(void) { return m_pModel; }
	//ライトのみ（実装必要なし）
	D3DXCOLOR GetDiffuse(void) { return D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); }
	void SetDiffuse(const D3DXCOLOR col) {}

	//使用モデル単位で消す
	static void Delete(CXModel* pTarget);

	//リスト除外
	virtual void Exclusion(void);

private:
	//モデル
	D3DXMATRIX mtxWorld;		//ワールドマトリ
	CXModel* m_pModel;			//モデル
	D3DXCOLOR m_changeColor;	//変更した色
	bool m_bEnable;				//色変更有効無効

	//位置類
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き
	float m_fWidth;		//幅
	float m_fHeight;	//高さ
	float m_fDepth;		//奥行
	IManipulation::TYPE m_type;	//種類

	//追加変数
	CVariable* m_apVariable[VARIABLE_NUM];

	//操作インターフェース
	CManipulationObj* m_pManipObj;

	//リスト
	static CObjectX* m_pTop;	//先頭オブジェクト
	static CObjectX* m_pCur;	//最後尾オブジェクト
	CObjectX* m_pNext;			//次のオブジェクト
	CObjectX* m_pPrev;			//前のオブジェクト
	static int m_nNumAll;		//総数
};

#endif // !_OBJECT_H_
