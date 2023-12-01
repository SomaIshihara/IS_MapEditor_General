//======================================================
//
//�I�u�W�F�N�g�ǂݍ��ݏ����̃w�b�_[objloader.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _OBJLOADER_H_
#define _OBJLOADER_H_

//�I�u�W�F�N�g�ǂݍ���
namespace objloader
{
	const int STR_LENGTH = 256;
	
	//�ǂݍ��݌���
	enum LOADRESULT
	{
		RES_OK = 0,
		RES_ERR_FILE_NOTFOUND,
		RES_MAX
	};

	//�o�C�i���̃R�[�h���e
	typedef unsigned char BINCODE;		//�i�[�p
	//�V�X�e��(0b00xxxxxx)
	const int BIN_CODE_SYSTEM			= (0b00 << 6);
	const int BIN_CODE_SCRIPT			= (BIN_CODE_SYSTEM + 0b000000);
	const int BIN_CODE_END_SCRIPT		= (BIN_CODE_SYSTEM + 0b000001);
	const int BIN_CODE_USERDEF			= (BIN_CODE_SYSTEM + 0b000010);
	//���f���n(0b01xxxxxx)
	const int BIN_CODE_MODEL			= (0b01 << 6);
	const int BIN_CODE_TEXTURE_FILENAME = (BIN_CODE_MODEL + 0b000000);
	const int BIN_CODE_MODEL_FILENAME	= (BIN_CODE_MODEL + 0b000001);
	const int BIN_CODE_MODELSET			= (BIN_CODE_MODEL + 0b000010);
	const int BIN_CODE_TEXTURE_NUM		= (BIN_CODE_MODEL + 0b000011);
	const int BIN_CODE_MODEL_NUM		= (BIN_CODE_MODEL + 0b000100);
	const int BIN_CODE_FIELDSET			= (BIN_CODE_MODEL + 0b000101);
	//���[�V�����n(0b10xxxxxx)
	const int BIN_CODE_MOTION			= (0b10 << 6);

	//�ǂݍ���
	LOADRESULT LoadData(const char* pPath);
	LOADRESULT SaveData(const char* pPath);
	LOADRESULT LoadTXTData(const char* pPath);
	LOADRESULT SaveTXTData(const char* pPath);
	void WriteCode(FILE* pFile, BINCODE code);
}

#endif // !_OBJECT_H_
