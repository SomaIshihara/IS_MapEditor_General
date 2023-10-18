//======================================================
//
//�}�l�[�W���̃w�b�_[manager.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "main.h"

//�O���錾
class CInputKeyboard;
class CInputMouse;
class CRenderer;
class CPlayer;
class CDebugProc;
class CCamera;
class CLight;
class CTexture;
class CMeshField;
class CGUIAddObj;
class CGUIChangeObj;
class CVariableManager;

class CManager
{
public:
	//�ÓIconst
	static const int CManager::INT_ZERO;
	static const float CManager::FLT_ZERO;
	static const D3DXVECTOR2 CManager::VEC2_ZERO;
	static const D3DXVECTOR3 CManager::VEC3_ZERO;

	//�R���X�g���N�^�E�f�X�g���N�^
	CManager();
	~CManager();

	//��{����
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	//FPS�v��
	void CheckFPS(DWORD dwCurrentTime, DWORD dwExecLastTime);

	//�擾
	static CInputKeyboard* GetInputKeyboard(void) { return m_pInputKeyboard; }
	static CInputMouse* GetInputMouse(void) { return m_pInputMouse; }
	static CRenderer* GetRenderer(void) { return m_pRenderer; }
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CDebugProc* GetDebProc(void) { return m_pDebProc; }
	static CCamera* GetCamera(void) { return m_pCamera; }
	static CLight* GetLight(void) { return m_pLight; }
	static CTexture* GetTexture(void) { return m_pTexture; }
	static CMeshField* GetMeshField(void) { return m_pMeshField; }
	static CVariableManager* GetVariableManager(void) { return m_pVariableManager; }

private:
	//�ϐ�
	static CInputKeyboard* m_pInputKeyboard;	//�L�[�{�[�h
	static CInputMouse* m_pInputMouse;			//�}�E�X
	static CRenderer* m_pRenderer;				//�����_���[
	static CDebugProc* m_pDebProc;				//�f�o�b�O
	static CPlayer* m_pPlayer;					//�v���C���[
	static CCamera* m_pCamera;					//�J����
	static CLight* m_pLight;					//���C�g
	static CTexture* m_pTexture;				//�e�N�X�`��
	static CMeshField* m_pMeshField;			//���b�V���t�B�[���h
	static CGUIAddObj* m_pGUIAddObj;			//�I�u�W�F�N�g�ǉ�GUI
	static CGUIChangeObj* m_pGUIChangeObj;		//�I�u�W�F�N�g�ύXGUI
	static CVariableManager* m_pVariableManager;//�ϐ��}�l

	static int m_nFPS;							//FPS
	static DWORD m_dwFrameCount;				//�񂵂��t���[����
};

#endif