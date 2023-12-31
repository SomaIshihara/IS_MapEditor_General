//==========================================
//
//カメラプログラムのヘッダ[camera.h]
//Author:石原颯馬
//
//==========================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//クラス化したものがこちらです
class CCamera
{
public:
	//静的const
	static const float LENGTH;
	static const float ROT_X_MIN;
	static const float ROT_X_MAX;

	//コンストラクタ・デストラクタ
	CCamera();
	~CCamera();
	
	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//設定処理
	void SetCamera(void);

	//位置移動
	void SetCameraPos(const D3DXVECTOR3 move);
	void SetCameraRot(const D3DXVECTOR3 rot);

	//取得
	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	D3DXVECTOR3 GetPosR(void) { return m_posR; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
private:
	//角度修正
	void FixPosV(void);
	void FixPosR(void);
	void FixRot(void);

	//メンバ変数
	D3DXMATRIX m_mtxView;		//ビューマトリ
	D3DXMATRIX m_mtxProjection;	//プロジェクションマトリ
	D3DXVECTOR3 m_posV;			//視点
	D3DXVECTOR3 m_posR;			//注視点
	D3DXVECTOR3 m_vecU;			//上ベクトル
	D3DXVECTOR3 m_rot;			//角度
	float m_fLength;			//距離
};

#endif // !_CAMERA_H_
