//======================================================
//
//オブジェクト読み込み処理のヘッダ[objloader.h]
//Author:石原颯馬
//
//======================================================
#ifndef _OBJLOADER_H_
#define _OBJLOADER_H_
#include "main.h"
#include "manager.h"
#include "object.h"

typedef unsigned char BINCODE;
//バイナリのコード内容
//システム(0b00xxxxxx)
#define BIN_CODE_SYSTEM				(0b00 << 6)
#define BIN_CODE_SCRIPT				(BIN_CODE_SYSTEM + 0b000000)
#define BIN_CODE_END_SCRIPT			(BIN_CODE_SYSTEM + 0b000001)
#define BIN_CODE_USERDEF			(BIN_CODE_SYSTEM + 0b000010)
//モデル系(0b01xxxxxx)
#define BIN_CODE_MODEL				(0b01 << 6)
#define BIN_CODE_TEXTURE_FILENAME	(BIN_CODE_MODEL + 0b000000)
#define BIN_CODE_MODEL_FILENAME		(BIN_CODE_MODEL + 0b000001)
#define BIN_CODE_MODELSET			(BIN_CODE_MODEL + 0b000010)
#define BIN_CODE_TEXTURE_NUM		(BIN_CODE_MODEL + 0b000011)
#define BIN_CODE_MODEL_NUM			(BIN_CODE_MODEL + 0b000100)
//モーション系(0b10xxxxxx)
#define BIN_CODE_MOTION				(0b10 << 6)

//オブジェクト読み込みクラス
class CObjLoader
{
public:
	//静的const
	static const int STR_LENGTH;	//文字列の長さ

	enum LOADRESULT
	{
		RES_OK = 0,
		RES_ERR_FILE_NOTFOUND,
		RES_MAX
	};

	//読み込み
	static LOADRESULT LoadData(const char* pPath);
	static LOADRESULT SaveData(const char* pPath);
	static LOADRESULT LoadTXTData(const char* pPath);
	static LOADRESULT SaveTXTData(const char* pPath);
};

#endif // !_OBJECT_H_
