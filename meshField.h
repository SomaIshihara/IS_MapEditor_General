//======================================================
//
//メッシュフィールド処理のヘッダ[meshField.h
//Author:石原颯馬
//
//======================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "object.h"
#include "manager.h"
#include "texture.h"
#include "collision.h"
#include "interface.h"

//前方宣言
class CManipulationObj;

//メッシュフィールドクラス
class CMeshField : public CObject, public IManipulation
{
public:
	//コンストラクタ・デストラクタ
	CMeshField();
	~CMeshField();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CMeshField* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fDepth,
		const int nBlockWidth,const int nBlockDepth);

	//テクスチャ設定
	void BindTexture(int nIdx) { m_nIdxTexture = nIdx; }

	//設定
	void SetTexType(const CTexture::TYPE type) { m_texType = type; }

	//取得
	int GetIdxTexture(void) { return m_nIdxTexture; }
	int GetModelIdx(void) { return -1; }
	CManipulationObj* GetManipulationObj(void) { return m_pManipObj; }
	static CMeshField* GetTop(void) { return m_pTop; }
	CMeshField* GetNext(void) { return m_pNext; }

	//インターフェース分
	//共通
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	IManipulation::TYPE GetType(void) { return m_type; }
	CCollision* GetCollision(void) { return &m_collision; }
	void Delete(void) { Uninit(); }
	//Xのみ（実装必要なし）
	CVariable** GetVariable(void) { return nullptr; }
	CXModel* GetModel(void) { return nullptr; }
	//ライトのみ（実装必要なし）
	D3DXCOLOR GetDiffuse(void) { return D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); }
	void SetDiffuse(const D3DXCOLOR col) {}
	//メッシュフィールドとビルボードとXのみ
	void SetSize(const float fWidth, const float fHeight, const float fDepth) { m_fWidth = fWidth; m_fDepth = fDepth; SetVtxBuff(); }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return 0.0f; }
	float GetDepth(void) { return m_fDepth; }
	//メッシュフィールドのみ
	void SetBlockNum(const int nWidth, const int nDepth) { m_nBlockWidth = nWidth; m_nBlockDepth = nDepth; SetVtxNum(); }
	int GetBlockWidth(void) { return m_nBlockWidth; }
	int GetBlockDepth(void) { return m_nBlockDepth; }

	//高さ求める
	float GetHeight(D3DXVECTOR3 posNew);

	//除外
	void Exclusion(void);
private:
	//関数
	void CreateBuff(void);
	void ReleaseBuff(void);
	void SetVtxBuff(void);
	void SetVtxNum(void);
	void SetIdxBuff(void);
	WORD* SetIdxSwaingField(int nCntWidth, WORD *pIdx);

	//変数
	int m_nIdxTexture;					//テクスチャ番号
	LPDIRECT3DVERTEXBUFFER9 m_pVtxbuff;	//頂点バッファ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//インデックスバッファ
	D3DXMATRIX m_mtxWorld;				//ワールドマトリ
	CTexture::TYPE m_texType;			//テクスチャタイプ
	IManipulation::TYPE m_type;			//種類
	CCollision m_collision;				//当たり判定

	//操作インターフェース
	CManipulationObj* m_pManipObj;

	//位置類
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き
	float m_fWidth;		//幅
	float m_fDepth;		//奥行
	int m_nBlockWidth;	//ブロック幅
	int m_nBlockDepth;	//ブロック奥行


	//リスト
	static CMeshField* m_pTop;	//先頭オブジェクト
	static CMeshField* m_pCur;	//最後尾オブジェクト
	CMeshField* m_pNext;			//次のオブジェクト
	CMeshField* m_pPrev;			//前のオブジェクト
	static int m_nNumAll;		//総数
};

#endif // !_MESHFIELD_H_
