//======================================================
//
//オブジェクト読み込み処理[objloader.cpp]
//Author:石原颯馬
//
//======================================================
#include "objloader.h"
#include "player.h"
#include "xmodel.h"
#include "objectX.h"
#include "userdef.h"
#include "meshField.h"
#include <fstream>
#include <iostream>
#include <string>

using objloader::LOADRESULT;

//========================
//データ読み込み
//========================
LOADRESULT objloader::LoadData(const char * pPath)
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

		//読み込みモデル全破棄（一緒に配置モデルも消える）
		CXModel::UnloadAll();

		//モデルの選択解除
		CManager::GetPlayer()->UnsetSelObj();

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

		//読み込みに使用したファイルパスを破棄
		if (ppFilePath != nullptr)
		{
			for (int cnt = 0; cnt < nReadedModel; cnt++)
			{
				if (ppFilePath[cnt] != nullptr)
				{
					delete[] ppFilePath[cnt];
					ppFilePath[cnt] = nullptr;
				}
			}
			delete[] ppFilePath;
			ppFilePath = nullptr;
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
LOADRESULT objloader::SaveData(const char * pPath)
{
	FILE* pFile;

	pFile = fopen(pPath, "wb");
	
	if (pFile != nullptr)
	{//普通に開けた
		//開始コード書き込み
		WriteCode(pFile, BIN_CODE_SCRIPT);

		//定義変数取得
		CVariableManager* pVariableManager = CManager::GetVariableManager();
		CVariable::Variable** ppDefined = pVariableManager->GetDefinedVariable();
		for (int cnt = 0; cnt < pVariableManager->GetDefinedNum(); cnt++)
		{
			//ユーザー定義変数書き込み開始
			WriteCode(pFile, BIN_CODE_USERDEF);

			std::string str = "";

			//型書き込み
			switch (ppDefined[cnt]->type)
			{
			case CVariable::Integer:
			{
				str += "int ";

				//変数名書き込み
				str += ppDefined[cnt]->pName;

				//初期値書き込み
				str += " = ";
				int nData = 0;
				nData = *(int*)ppDefined[cnt]->pData;
				str += std::to_string(nData);
			}
				break;
			case CVariable::Float:
			{
				str += "float ";

				//変数名書き込み
				str += ppDefined[cnt]->pName;

				//初期値書き込み
				str += " = ";
				float fData = 0;
				fData = *(float*)ppDefined[cnt]->pData;
				str += std::to_string(fData);
			}
				break;
			case CVariable::Boolean:
			{
				str += "bool ";

				//変数名書き込み
				str += ppDefined[cnt]->pName;

				//初期値書き込み
				str += " = ";
				str += ((bool)ppDefined[cnt]->pData == true) ? "true" : "false";
			}
				break;
			default:
				assert(false);
				break;
			}

			//ファイルに書き込み
			fwrite(str.c_str(), sizeof(char), STR_LENGTH, pFile);
		}

		//モデル個数書き込み
		WriteCode(pFile, BIN_CODE_MODEL_NUM);
		int nNumAll = CXModel::GetNumAll();
		fwrite(&nNumAll,sizeof(int),1,pFile);

		//モデルファイルパス書き込み
		CXModel* pModel = CXModel::GetTop();	//リスト書き込み体制
		while (pModel != nullptr)
		{
			CXModel* pObjectNext = pModel->GetNext();

			WriteCode(pFile, BIN_CODE_MODEL_FILENAME);
			fwrite(pModel->GetPath(), sizeof(char), STR_LENGTH, pFile);	//データ

			pModel = pObjectNext;
		}

		//モデル情報書き込み
		CObjectX* pObject = CObjectX::GetTop();
		while (pObject != nullptr)
		{
			CObjectX* pObjectNext = pObject->GetNext();

			WriteCode(pFile, BIN_CODE_MODELSET);
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
		WriteCode(pFile, BIN_CODE_END_SCRIPT);

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
LOADRESULT objloader::LoadTXTData(const char * pPath)
{
	return RES_OK;
}

//========================
//データ書き込み(TXT)
//========================
LOADRESULT objloader::SaveTXTData(const char * pPath)
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
		CMeshField* pMeshField = CMeshField::GetTop();
		while (pMeshField != nullptr)
		{
			D3DXVECTOR3 pos = pMeshField->GetPos();
			D3DXVECTOR3 rot = pMeshField->GetRot();
			ofs << "FIELDSET\n";
			ofs << "	TEXTYPE = 0 #(エディタ未実装のため手動で入力してください)\n";
			ofs << "	POS = " << pos.x << " " << pos.y << " " << pos.z << "\n";
			ofs << "	ROT = " << rot.x << " " << rot.y << " " << rot.z << "\n";
			ofs << "	BLOCK = " << pMeshField->GetBlockWidth() << " " << pMeshField->GetBlockDepth() << "\n";
			ofs << "	SIZE = " << pMeshField->GetWidth() << " " << pMeshField->GetDepth() << "\n";
			ofs << "	MOVE = 0 0 #(エディタ未実装のため手動で入力してください)\n";
			ofs << "END_FIELDSET\n";
			ofs << "\n";
			pMeshField = pMeshField->GetNext();
		}

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

//========================
//バイナリコード書き込み
//========================
void objloader::WriteCode(FILE * pFile, BINCODE code)
{
	fwrite(&code, sizeof(BINCODE), 1, pFile);
}
