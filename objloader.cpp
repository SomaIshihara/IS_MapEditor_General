//======================================================
//
//オブジェクト読み込み処理[objloader.cpp]
//Author:石原颯馬
//
//======================================================
#include "objloader.h"
#include "texture.h"
#include "xmodel.h"
#include "objectX.h"
#include "userdef.h"
#include <fstream>
#include <iostream>
#include <string>

//静的メンバ変数
const int CObjLoader::STR_LENGTH = 256;

//========================
//データ読み込み
//========================
CObjLoader::LOADRESULT CObjLoader::LoadData(const char * pPath)
{
	FILE* pFile;
	BINCODE code;
	bool bRead = false;
	char** ppFilePath = nullptr;
	int nReadedModel = 0;
	CVariableManager* pVariableManager = CManager::GetVariableManager();

	pFile = fopen(pPath, "rb");

	if (pFile != nullptr)
	{//開けた
		//定義変数初期化
		pVariableManager->ReleaseAll();

		while (1)
		{
			fread(&code, sizeof(BINCODE), 1, pFile);

			//文字列チェック
			if (code == BIN_CODE_SCRIPT)
			{//読み取り開始
				bRead = true;
			}
			else if (code == BIN_CODE_END_SCRIPT)
			{//読み取り終了
				bRead = false;
				break;
			}
			else if (bRead == true)
			{//読み取り
				if (code == BIN_CODE_TEXTURE_FILENAME)
				{
					char aPath[STR_LENGTH];
					fread(&aPath[0], sizeof(char), STR_LENGTH, pFile);
					CManager::GetTextureEditorPop()->Load(&aPath[0]);
				}
				else if (code == BIN_CODE_MODEL_NUM)
				{
					int nNumAll;
					fread(&nNumAll, sizeof(int), 1, pFile);
					ppFilePath = new char*[nNumAll];
				}
				else if (code == BIN_CODE_MODEL_FILENAME)
				{
					char aPath[STR_LENGTH];
					fread(&aPath[0], sizeof(char), STR_LENGTH, pFile);
					CXModel::Load(&aPath[0]);

					//モデルパス読み取り（引き出し用に使う）
					ppFilePath[nReadedModel] = new char[strlen(&aPath[0]) + 1];
					strcpy(ppFilePath[nReadedModel], &aPath[0]);
					nReadedModel++;
				}
				else if (code == BIN_CODE_MODELSET)
				{
					D3DXVECTOR3 pos, rot;
					int nModelNum = -1;
					CXModel* pModel = nullptr;
					fread(&pos, sizeof(D3DXVECTOR3), 1, pFile);
					fread(&rot, sizeof(D3DXVECTOR3), 1, pFile);
					fread(&nModelNum, sizeof(int), 1, pFile);
					pModel = CXModel::Load(ppFilePath[nModelNum]);

					CObjectX* pObject = CObjectX::Create(pos, rot, pModel);

					//追加変数読み込み
					for (int cnt = 0; cnt < pVariableManager->GetDefinedNum(); cnt++)
					{
						char aVal[STR_LENGTH] = "";
						fread(&aVal[0], sizeof(char), STR_LENGTH, pFile);
						char* pName = strtok(&aVal[0], " =\n");
						char* pValue = strtok(nullptr, " =\n");

						//型読み込み
						switch (pObject->GetVariable()[cnt]->GetType())
						{
						case CVariable::Integer:
						{
							int nData = atoi(pValue);
							pObject->GetVariable()[cnt]->SetData(&nData);
						}
						break;
						case CVariable::Float:
						{
							float fData = 0.0f;
							pObject->GetVariable()[cnt]->SetData(&fData);
						}
						break;
						case CVariable::Boolean:
						{
							char bData = (strcmp(pValue, "true") == 0) ? 0xff : 0x00;
							pObject->GetVariable()[cnt]->SetData(&bData);
						}
							break;
						default:
							assert(false);
							break;
						}
					}
				}
				else if (code == BIN_CODE_USERDEF)
				{
					char aDefVariableStr[STR_LENGTH];

					fread(&aDefVariableStr[0], sizeof(char), STR_LENGTH, pFile);

					pVariableManager->Analysis(&aDefVariableStr[0]);
				}
			}
		}

		fclose(pFile);
		return RES_OK;
	}
	else
	{//開けなかった（ファイルないんじゃね？）
		return RES_ERR_FILE_NOTFOUND;
	}
}

//========================
//データ書き込み
//========================
CObjLoader::LOADRESULT CObjLoader::SaveData(const char * pPath)
{
	FILE* pFile;

	pFile = fopen(pPath, "wb");
	
	if (pFile != nullptr)
	{//普通に開けた
		//開始コード書き込み
		BINCODE code = BIN_CODE_SCRIPT;
		fwrite(&code, sizeof(BINCODE), 1, pFile);

		//定義変数取得
		CVariableManager* pVariableManager = CManager::GetVariableManager();
		CVariable::Variable** ppDefined = pVariableManager->GetDefinedVariable();
		for (int cnt = 0; cnt < pVariableManager->GetDefinedNum(); cnt++)
		{
			//ユーザー定義変数書き込み開始
			code = BIN_CODE_USERDEF;
			fwrite(&code, sizeof(BINCODE), 1, pFile);

			char aDefVariableStr[STR_LENGTH] = "";

			//型書き込み
			switch (ppDefined[cnt]->type)
			{
			case CVariable::Integer:
			{
				strcat(&aDefVariableStr[0], "int ");

				//変数名書き込み
				strcat(&aDefVariableStr[0], &ppDefined[cnt]->pName[0]);

				//初期値書き込み
				strcat(&aDefVariableStr[0], " = ");
				int nData = 0;
				nData = *(int*)ppDefined[cnt]->pData;
				strcat(&aDefVariableStr[0], std::to_string(nData).c_str());
			}
				break;
			case CVariable::Float:
			{
				strcat(&aDefVariableStr[0], "float ");

				//変数名書き込み
				strcat(&aDefVariableStr[0], &ppDefined[cnt]->pName[0]);

				//初期値書き込み
				strcat(&aDefVariableStr[0], " = ");
				float fData = 0;
				fData = *(float*)ppDefined[cnt]->pData;
				strcat(&aDefVariableStr[0], std::to_string(fData).c_str());
			}
				break;
			case CVariable::Boolean:
				strcat(&aDefVariableStr[0], "bool ");

				//変数名書き込み
				strcat(&aDefVariableStr[0], &ppDefined[cnt]->pName[0]);

				//初期値書き込み
				strcat(&aDefVariableStr[0], " = ");
				strcat(&aDefVariableStr[0], ((bool)ppDefined[cnt]->pData == true) ? "true" : "false");
				break;
			default:
				assert(false);
				break;
			}

			//ファイルに書き込み
			fwrite(&aDefVariableStr[0], sizeof(char), STR_LENGTH, pFile);
		}

		//モデル個数書き込み
		code = BIN_CODE_MODEL_NUM;
		int nNumAll = CXModel::GetNumAll();
		fwrite(&code, sizeof(BINCODE), 1, pFile);
		fwrite(&nNumAll,sizeof(int),1,pFile);

		//モデルファイルパス書き込み
		code = BIN_CODE_MODEL_FILENAME;
		CXModel* pModel = CXModel::GetTop();	//リスト書き込み体制
		while (pModel != nullptr)
		{
			CXModel* pObjectNext = pModel->GetNext();

			fwrite(&code, sizeof(BINCODE), 1, pFile);						//コード
			fwrite(pModel->GetPath(), sizeof(char), STR_LENGTH, pFile);	//データ

			pModel = pObjectNext;
		}

		//モデル情報書き込み
		code = BIN_CODE_MODELSET;
		CObjectX* pObject = CObjectX::GetTop();
		while (pObject != nullptr)
		{
			CObjectX* pObjectNext = pObject->GetNext();

			fwrite(&code, sizeof(BINCODE), 1, pFile);				//コード
			fwrite(&pObject->GetPos(), sizeof(D3DXVECTOR3), 1, pFile);
			fwrite(&pObject->GetRot(), sizeof(D3DXVECTOR3), 1, pFile);

			//モデル種類番号化
			int nModelNum = 0;
			CXModel* pModel = CXModel::GetTop();
			while (pModel != nullptr && pModel != pObject->GetModel())
			{
				pModel = pModel->GetNext();
				nModelNum++;
			}
			fwrite(&nModelNum, sizeof(int), 1, pFile);

			//追加変数書き込み
			for (int cnt = 0; cnt < pVariableManager->GetDefinedNum(); cnt++)
			{
				char aVal[STR_LENGTH] = "";

				strcat(&aVal[0], pObject->GetVariable()[cnt]->GetName());	//変数名
				strcat(&aVal[0], " = ");	//=

				//値書き込み
				switch (pObject->GetVariable()[cnt]->GetType())
				{
				case CVariable::Integer:
				{
					//初期値書き込み
					int nData = 0;
					nData = *(int*)pObject->GetVariable()[cnt]->GetData();
					strcat(&aVal[0], std::to_string(nData).c_str());
				}
				break;
				case CVariable::Float:
				{
					//初期値書き込み
					float fData = 0;
					fData = *(float*)pObject->GetVariable()[cnt]->GetData();
					strcat(&aVal[0], std::to_string(fData).c_str());
				}
				break;
				case CVariable::Boolean:
					//初期値書き込み
					strcat(&aVal[0], (*(unsigned char*)pObject->GetVariable()[cnt]->GetData() == 0xff) ? "true" : "false");
					break;
				default:
					assert(false);
					break;
				}
				fwrite(&aVal[0], sizeof(char), STR_LENGTH, pFile);
			}

			pObject = pObjectNext;
		}

		//終了コード書き込み
		code = BIN_CODE_END_SCRIPT;
		fwrite(&code, sizeof(BINCODE), 1, pFile);

		fclose(pFile);
		return RES_OK;
	}
	else
	{//なぜか開けなかった（なんで？）
		return RES_ERR_FILE_NOTFOUND;
	}
}

//========================
//データ読み込み(TXT)
//========================
CObjLoader::LOADRESULT CObjLoader::LoadTXTData(const char * pPath)
{
	return RES_OK;
}

//========================
//データ書き込み(TXT)
//========================
CObjLoader::LOADRESULT CObjLoader::SaveTXTData(const char * pPath)
{
	std::ofstream ofs(pPath);	//ファイル読み込み

	//書き込み開始
	{
		CTexture* pTexture = CManager::GetTextureEditorPop();
		CXModel* pModel = CXModel::GetTop();
		int nNumTexture = pTexture->GetNumAll();
		int nNumModel = CXModel::GetNumAll();
		ofs << "#==============================================================================\n";
		ofs << "#\n";
		ofs << "# マップ配置スクリプトファイル [" << pPath << "]\n";
		ofs << "# IS_MapEditor_Generalより生成\n";
		ofs << "#\n";
		ofs << "#==============================================================================\n";
		ofs << "SCRIPT\n\n";

		ofs << "#==============================================================================\n";
		ofs << "# テクスチャ数\n";
		ofs << "#==============================================================================\n";
		ofs << "NUM_TEXTURE = " << nNumTexture << "\n";
		ofs << "\n";

		ofs << "#==============================================================================\n";
		ofs << "# テクスチャファイル名\n";
		ofs << "#==============================================================================\n";
		for (int cnt = 0; cnt < nNumTexture; cnt++)
		{
			ofs << "TEXTURE_FILENAME = " << pTexture->GetPath(cnt) << "\n";
		}
		ofs << "\n";

		ofs << "#==============================================================================\n";
		ofs << "# モデル数\n";
		ofs << "#==============================================================================\n";
		ofs << "NUM_MODEL = " << nNumModel << "\n";
		ofs << "\n";

		ofs << "#==============================================================================\n";
		ofs << "# モデルファイル名\n";
		ofs << "#==============================================================================\n";
		for (int cnt = 0; cnt < nNumModel; cnt++)
		{
			if (pModel != nullptr)
			{
				CXModel* pObjectNext = pModel->GetNext();

				//パス書き込み
				ofs << "MODEL_FILENAME = " << pModel->GetPath() << "\n";

				pModel = pObjectNext;
			}
			else
			{
				assert(false);
			}
		}
		ofs << "\n";

		ofs << "#==============================================================================\n";
		ofs << "# カメラ情報\n";
		ofs << "#==============================================================================\n";
		ofs << "#CAMERASET(エディタ未実装のため手動で入力してください)\n";
		ofs << "#	POS = 0 0 0\n";
		ofs << "#	ROT = 0 0 0\n";
		ofs << "#END_CAMERASET\n";
		ofs << "\n";

		ofs << "#==============================================================================\n";
		ofs << "# ライト情報\n";
		ofs << "#==============================================================================\n";
		ofs << "#LIGHTSET(エディタ未実装のため手動で入力してください)\n";
		ofs << "#	DIRECTION = 0 0 0\n";
		ofs << "#	DIFFUSE = 0 0 0\n";
		ofs << "#END_LIGHTSET\n";
		ofs << "\n";

		ofs << "#==============================================================================\n";
		ofs << "# 空情報\n";
		ofs << "#==============================================================================\n";
		ofs << "#SKYSET(エディタ未実装のため手動で入力してください)\n";
		ofs << "#	TEXTYPE = 0\n";
		ofs << "#	MOVE = 0\n";
		ofs << "#END_SKYSET\n";
		ofs << "\n";

		ofs << "#==============================================================================\n";
		ofs << "# 山情報\n";
		ofs << "#==============================================================================\n";
		ofs << "#MOUNTAINSET(エディタ未実装のため手動で入力してください)\n";
		ofs << "#	TEXTYPE = 0\n";
		ofs << "#END_MOUNTAINSET\n";
		ofs << "\n";

		ofs << "#==============================================================================\n";
		ofs << "# 床情報\n";
		ofs << "#==============================================================================\n";
		ofs << "#FIELDSET(エディタ未実装のため手動で入力してください)\n";
		ofs << "#	TEXTYPE = 0\n";
		ofs << "#	POS = 0 0 0\n";
		ofs << "#	ROT = 0 0 0\n";
		ofs << "#	BLOCK = 0 0\n";
		ofs << "#	SIZE = 0 0\n";
		ofs << "#	MOVE = 0 0\n";
		ofs << "#END_FIELDSET\n";
		ofs << "\n";

		ofs << "#==============================================================================\n";
		ofs << "# モデル配置\n";
		ofs << "#==============================================================================\n";
		CObjectX* pObject = CObjectX::GetTop();
		while (pObject != nullptr)
		{
			CObjectX* pObjectNext = pObject->GetNext();

			//開始書き込み
			ofs << "MODELSET\n";

			//モデル番号
			int nModelNum = 0;
			CXModel* pModel = CXModel::GetTop();
			while (pModel != nullptr && pModel != pObject->GetModel())
			{
				pModel = pModel->GetNext();
				nModelNum++;
			}
			ofs << "	TYPE = " << nModelNum << "\n";

			//位置向き
			D3DXVECTOR3 pos = pObject->GetPos();
			D3DXVECTOR3 rot = pObject->GetRot();
			ofs << "	POS = " << pos.x << " " << pos.y << " " << pos.z << "\n";
			ofs << "	ROT = " << rot.x << " " << rot.y << " " << rot.z << "\n";
			ofs << "#	影設定はエディタ未実装のため手動で入力してください\n";
			ofs << "	SHADOW = 1\n";

			//終了書き込み
			ofs << "END_MODELSET\n";
			ofs << "\n";

			pObject = pObjectNext;
		}

		//終了文字
		ofs << "END_SCRIPT\n";
	}

	return RES_OK;
}
