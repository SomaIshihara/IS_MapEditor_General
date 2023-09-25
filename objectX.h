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

typedef unsigned char BINCODE;
//バイナリのコード内容
//システム(0b00xxxxxx)
#define BIN_CODE_SYSTEM				(0b00 << 6)
#define BIN_CODE_SCRIPT				(BIN_CODE_SYSTEM + 0b000000)
#define BIN_CODE_END_SCRIPT			(BIN_CODE_SYSTEM + 0b000001)
//モデル系(0b01xxxxxx)
#define BIN_CODE_MODEL				(0b01 << 6)
#define BIN_CODE_TEXTURE_FILENAME	(BIN_CODE_MODEL + 0b000000)
#define BIN_CODE_MODEL_FILENAME		(BIN_CODE_MODEL + 0b000001)
#define BIN_CODE_MODELSET			(BIN_CODE_MODEL + 0b000010)
#define BIN_CODE_TEXTURE_NUM		(BIN_CODE_MODEL + 0b000011)
#define BIN_CODE_MODEL_NUM			(BIN_CODE_MODEL + 0b000100)
//モーション系(0b10xxxxxx)
#define BIN_CODE_MOTION				(0b10 << 6)

//前方宣言
class CXModel;

//オブジェクトクラス
class CObjectX : public CObject
{
public:
	enum LOADRESULT
	{
		RES_OK = 0,
		RES_ERR_FILE_NOTFOUND,
		RES_MAX
	};

	//静的const
	static const int PATH_LENGTH;	//仮置きに必要らしい

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

	//設定
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }

	//使用モデル単位で消す
	static void Delete(CXModel* pTarget);

	//読み込み
	static LOADRESULT LoadData(const char* pPath);
	static LOADRESULT SaveData(const char* pPath);

	//死亡フラグが立っているオブジェを殺す
	static void Exclusion(void);

private:
	//モデル
	D3DXMATRIX mtxWorld;	//ワールドマトリ
	CXModel* m_pModel;		//モデル

	//位置類
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き

	//リスト
	static CObjectX* m_pTop;	//先頭オブジェクト
	static CObjectX* m_pCur;	//最後尾オブジェクト
	CObjectX* m_pNext;			//次のオブジェクト
	CObjectX* m_pPrev;			//前のオブジェクト
	bool m_bExclusion;			//除外フラグ
	static int m_nNumAll;		//総数
};

#endif // !_OBJECT_H_
