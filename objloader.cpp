//======================================================
//
//�I�u�W�F�N�g�ǂݍ��ݏ���[objloader.cpp]
//Author:�Ό��D�n
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

//�ÓI�����o�ϐ�
const int CObjLoader::STR_LENGTH = 256;

//========================
//�f�[�^�ǂݍ���
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
	{//�J����
		//��`�ϐ�������
		pVariableManager->ReleaseAll();

		while (1)
		{
			fread(&code, sizeof(BINCODE), 1, pFile);

			//������`�F�b�N
			if (code == BIN_CODE_SCRIPT)
			{//�ǂݎ��J�n
				bRead = true;
			}
			else if (code == BIN_CODE_END_SCRIPT)
			{//�ǂݎ��I��
				bRead = false;
				break;
			}
			else if (bRead == true)
			{//�ǂݎ��
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

					//���f���p�X�ǂݎ��i�����o���p�Ɏg���j
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

					//�ǉ��ϐ��ǂݍ���
					for (int cnt = 0; cnt < pVariableManager->GetDefinedNum(); cnt++)
					{
						char aVal[STR_LENGTH] = "";
						fread(&aVal[0], sizeof(char), STR_LENGTH, pFile);
						char* pName = strtok(&aVal[0], " =\n");
						char* pValue = strtok(nullptr, " =\n");

						//�^�ǂݍ���
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
	{//�J���Ȃ������i�t�@�C���Ȃ��񂶂�ˁH�j
		return RES_ERR_FILE_NOTFOUND;
	}
}

//========================
//�f�[�^��������
//========================
CObjLoader::LOADRESULT CObjLoader::SaveData(const char * pPath)
{
	FILE* pFile;

	pFile = fopen(pPath, "wb");
	
	if (pFile != nullptr)
	{//���ʂɊJ����
		//�J�n�R�[�h��������
		BINCODE code = BIN_CODE_SCRIPT;
		fwrite(&code, sizeof(BINCODE), 1, pFile);

		//��`�ϐ��擾
		CVariableManager* pVariableManager = CManager::GetVariableManager();
		CVariable::Variable** ppDefined = pVariableManager->GetDefinedVariable();
		for (int cnt = 0; cnt < pVariableManager->GetDefinedNum(); cnt++)
		{
			//���[�U�[��`�ϐ��������݊J�n
			code = BIN_CODE_USERDEF;
			fwrite(&code, sizeof(BINCODE), 1, pFile);

			char aDefVariableStr[STR_LENGTH] = "";

			//�^��������
			switch (ppDefined[cnt]->type)
			{
			case CVariable::Integer:
			{
				strcat(&aDefVariableStr[0], "int ");

				//�ϐ�����������
				strcat(&aDefVariableStr[0], &ppDefined[cnt]->pName[0]);

				//�����l��������
				strcat(&aDefVariableStr[0], " = ");
				int nData = 0;
				nData = *(int*)ppDefined[cnt]->pData;
				strcat(&aDefVariableStr[0], std::to_string(nData).c_str());
			}
				break;
			case CVariable::Float:
			{
				strcat(&aDefVariableStr[0], "float ");

				//�ϐ�����������
				strcat(&aDefVariableStr[0], &ppDefined[cnt]->pName[0]);

				//�����l��������
				strcat(&aDefVariableStr[0], " = ");
				float fData = 0;
				fData = *(float*)ppDefined[cnt]->pData;
				strcat(&aDefVariableStr[0], std::to_string(fData).c_str());
			}
				break;
			case CVariable::Boolean:
				strcat(&aDefVariableStr[0], "bool ");

				//�ϐ�����������
				strcat(&aDefVariableStr[0], &ppDefined[cnt]->pName[0]);

				//�����l��������
				strcat(&aDefVariableStr[0], " = ");
				strcat(&aDefVariableStr[0], ((bool)ppDefined[cnt]->pData == true) ? "true" : "false");
				break;
			default:
				assert(false);
				break;
			}

			//�t�@�C���ɏ�������
			fwrite(&aDefVariableStr[0], sizeof(char), STR_LENGTH, pFile);
		}

		//���f������������
		code = BIN_CODE_MODEL_NUM;
		int nNumAll = CXModel::GetNumAll();
		fwrite(&code, sizeof(BINCODE), 1, pFile);
		fwrite(&nNumAll,sizeof(int),1,pFile);

		//���f���t�@�C���p�X��������
		code = BIN_CODE_MODEL_FILENAME;
		CXModel* pModel = CXModel::GetTop();	//���X�g�������ݑ̐�
		while (pModel != nullptr)
		{
			CXModel* pObjectNext = pModel->GetNext();

			fwrite(&code, sizeof(BINCODE), 1, pFile);						//�R�[�h
			fwrite(pModel->GetPath(), sizeof(char), STR_LENGTH, pFile);	//�f�[�^

			pModel = pObjectNext;
		}

		//���f����񏑂�����
		code = BIN_CODE_MODELSET;
		CObjectX* pObject = CObjectX::GetTop();
		while (pObject != nullptr)
		{
			CObjectX* pObjectNext = pObject->GetNext();

			fwrite(&code, sizeof(BINCODE), 1, pFile);				//�R�[�h
			fwrite(&pObject->GetPos(), sizeof(D3DXVECTOR3), 1, pFile);
			fwrite(&pObject->GetRot(), sizeof(D3DXVECTOR3), 1, pFile);

			//���f����ޔԍ���
			int nModelNum = 0;
			CXModel* pModel = CXModel::GetTop();
			while (pModel != nullptr && pModel != pObject->GetModel())
			{
				pModel = pModel->GetNext();
				nModelNum++;
			}
			fwrite(&nModelNum, sizeof(int), 1, pFile);

			//�ǉ��ϐ���������
			for (int cnt = 0; cnt < pVariableManager->GetDefinedNum(); cnt++)
			{
				char aVal[STR_LENGTH] = "";

				strcat(&aVal[0], pObject->GetVariable()[cnt]->GetName());	//�ϐ���
				strcat(&aVal[0], " = ");	//=

				//�l��������
				switch (pObject->GetVariable()[cnt]->GetType())
				{
				case CVariable::Integer:
				{
					//�����l��������
					int nData = 0;
					nData = *(int*)pObject->GetVariable()[cnt]->GetData();
					strcat(&aVal[0], std::to_string(nData).c_str());
				}
				break;
				case CVariable::Float:
				{
					//�����l��������
					float fData = 0;
					fData = *(float*)pObject->GetVariable()[cnt]->GetData();
					strcat(&aVal[0], std::to_string(fData).c_str());
				}
				break;
				case CVariable::Boolean:
					//�����l��������
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

		//�I���R�[�h��������
		code = BIN_CODE_END_SCRIPT;
		fwrite(&code, sizeof(BINCODE), 1, pFile);

		fclose(pFile);
		return RES_OK;
	}
	else
	{//�Ȃ����J���Ȃ������i�Ȃ�ŁH�j
		return RES_ERR_FILE_NOTFOUND;
	}
}

//========================
//�f�[�^�ǂݍ���(TXT)
//========================
CObjLoader::LOADRESULT CObjLoader::LoadTXTData(const char * pPath)
{
	return RES_OK;
}

//========================
//�f�[�^��������(TXT)
//========================
CObjLoader::LOADRESULT CObjLoader::SaveTXTData(const char * pPath)
{
	std::ofstream ofs(pPath);	//�t�@�C���ǂݍ���

	//�������݊J�n
	{
		CTexture* pTexture = CManager::GetTextureEditorPop();
		CXModel* pModel = CXModel::GetTop();
		int nNumTexture = pTexture->GetNumAll();
		int nNumModel = CXModel::GetNumAll();
		ofs << "#==============================================================================\n";
		ofs << "#\n";
		ofs << "# �}�b�v�z�u�X�N���v�g�t�@�C�� [" << pPath << "]\n";
		ofs << "# IS_MapEditor_General��萶��\n";
		ofs << "#\n";
		ofs << "#==============================================================================\n";
		ofs << "SCRIPT\n\n";

		ofs << "#==============================================================================\n";
		ofs << "# �e�N�X�`����\n";
		ofs << "#==============================================================================\n";
		ofs << "NUM_TEXTURE = " << nNumTexture << "\n";
		ofs << "\n";

		ofs << "#==============================================================================\n";
		ofs << "# �e�N�X�`���t�@�C����\n";
		ofs << "#==============================================================================\n";
		for (int cnt = 0; cnt < nNumTexture; cnt++)
		{
			ofs << "TEXTURE_FILENAME = " << pTexture->GetPath(cnt) << "\n";
		}
		ofs << "\n";

		ofs << "#==============================================================================\n";
		ofs << "# ���f����\n";
		ofs << "#==============================================================================\n";
		ofs << "NUM_MODEL = " << nNumModel << "\n";
		ofs << "\n";

		ofs << "#==============================================================================\n";
		ofs << "# ���f���t�@�C����\n";
		ofs << "#==============================================================================\n";
		for (int cnt = 0; cnt < nNumModel; cnt++)
		{
			if (pModel != nullptr)
			{
				CXModel* pObjectNext = pModel->GetNext();

				//�p�X��������
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
		ofs << "# �J�������\n";
		ofs << "#==============================================================================\n";
		ofs << "#CAMERASET(�G�f�B�^�������̂��ߎ蓮�œ��͂��Ă�������)\n";
		ofs << "#	POS = 0 0 0\n";
		ofs << "#	ROT = 0 0 0\n";
		ofs << "#END_CAMERASET\n";
		ofs << "\n";

		ofs << "#==============================================================================\n";
		ofs << "# ���C�g���\n";
		ofs << "#==============================================================================\n";
		ofs << "#LIGHTSET(�G�f�B�^�������̂��ߎ蓮�œ��͂��Ă�������)\n";
		ofs << "#	DIRECTION = 0 0 0\n";
		ofs << "#	DIFFUSE = 0 0 0\n";
		ofs << "#END_LIGHTSET\n";
		ofs << "\n";

		ofs << "#==============================================================================\n";
		ofs << "# ����\n";
		ofs << "#==============================================================================\n";
		ofs << "#SKYSET(�G�f�B�^�������̂��ߎ蓮�œ��͂��Ă�������)\n";
		ofs << "#	TEXTYPE = 0\n";
		ofs << "#	MOVE = 0\n";
		ofs << "#END_SKYSET\n";
		ofs << "\n";

		ofs << "#==============================================================================\n";
		ofs << "# �R���\n";
		ofs << "#==============================================================================\n";
		ofs << "#MOUNTAINSET(�G�f�B�^�������̂��ߎ蓮�œ��͂��Ă�������)\n";
		ofs << "#	TEXTYPE = 0\n";
		ofs << "#END_MOUNTAINSET\n";
		ofs << "\n";

		ofs << "#==============================================================================\n";
		ofs << "# �����\n";
		ofs << "#==============================================================================\n";
		ofs << "#FIELDSET(�G�f�B�^�������̂��ߎ蓮�œ��͂��Ă�������)\n";
		ofs << "#	TEXTYPE = 0\n";
		ofs << "#	POS = 0 0 0\n";
		ofs << "#	ROT = 0 0 0\n";
		ofs << "#	BLOCK = 0 0\n";
		ofs << "#	SIZE = 0 0\n";
		ofs << "#	MOVE = 0 0\n";
		ofs << "#END_FIELDSET\n";
		ofs << "\n";

		ofs << "#==============================================================================\n";
		ofs << "# ���f���z�u\n";
		ofs << "#==============================================================================\n";
		CObjectX* pObject = CObjectX::GetTop();
		while (pObject != nullptr)
		{
			CObjectX* pObjectNext = pObject->GetNext();

			//�J�n��������
			ofs << "MODELSET\n";

			//���f���ԍ�
			int nModelNum = 0;
			CXModel* pModel = CXModel::GetTop();
			while (pModel != nullptr && pModel != pObject->GetModel())
			{
				pModel = pModel->GetNext();
				nModelNum++;
			}
			ofs << "	TYPE = " << nModelNum << "\n";

			//�ʒu����
			D3DXVECTOR3 pos = pObject->GetPos();
			D3DXVECTOR3 rot = pObject->GetRot();
			ofs << "	POS = " << pos.x << " " << pos.y << " " << pos.z << "\n";
			ofs << "	ROT = " << rot.x << " " << rot.y << " " << rot.z << "\n";
			ofs << "#	�e�ݒ�̓G�f�B�^�������̂��ߎ蓮�œ��͂��Ă�������\n";
			ofs << "	SHADOW = 1\n";

			//�I����������
			ofs << "END_MODELSET\n";
			ofs << "\n";

			pObject = pObjectNext;
		}

		//�I������
		ofs << "END_SCRIPT\n";
	}

	return RES_OK;
}
