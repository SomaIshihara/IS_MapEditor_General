//======================================================
//
//インターフェース一覧[interface.h]
//Author:石原颯馬
//
//======================================================
#ifndef _INTERFACE_H_
#define _INTERFACE_H_

//前方宣言
class CVariable;
class CXModel;

//表示オブジェクト操作インターフェース
__interface IManipulation
{
	//種類列挙
	enum TYPE
	{
		TYPE_OBJX = 0,
		TYPE_CAMERA,
		TYPE_LIGHT,
		TYPE_MAX
	};

	//共通（すべてに実装）
	D3DXVECTOR3 GetPos(void) = 0;
	void SetPos(const D3DXVECTOR3 pos) = 0;
	D3DXVECTOR3 GetRot(void) = 0;
	void SetRot(const D3DXVECTOR3 rot) = 0;
	TYPE GetType(void) = 0;
	void Delete(void) = 0;		//オブジェクト削除

	//現状Xのみ（それ以外適当に返せばいいよ）
	CVariable** GetVariable(void) = 0;
	CXModel* GetModel(void) = 0;

	//現状ライトのみ（それ以外適当に返せばいいよ）
	D3DXCOLOR GetDiffuse(void) = 0;
	void SetDiffuse(const D3DXCOLOR col) = 0;
};

#endif // !_INTERFACE_H_
