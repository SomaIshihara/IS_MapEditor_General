//==========================================
//
//���C�g�v���O�����̃w�b�_[light.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

//�}�N��
#define MAX_LIGHT	(3)	//���C�g��

//�N���X���������̂�������ł�
class CLight
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CLight();
	~CLight();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//�ݒ�
	void SetDiffuse(const D3DXCOLOR col);
	void SetDirection(const D3DXVECTOR3 dir);
	void SetEnable(const bool bEnable);

	//�j��
	static void ReleaseAll(void);

	//�擾
	D3DXCOLOR GetDiffuse(void) { return m_light.Diffuse; }
	D3DXVECTOR3 GetDirection(void) { return m_light.Direction; }

	//����
	static CLight* Create(const D3DXVECTOR3 dir, const D3DXCOLOR col);

private:
	//���X�g
	static CLight* m_pTop;	//�擪�I�u�W�F�N�g
	static CLight* m_pCur;	//�Ō���I�u�W�F�N�g
	CLight* m_pNext;			//���̃I�u�W�F�N�g
	CLight* m_pPrev;			//�O�̃I�u�W�F�N�g
	int m_nID;					//ID
	static int m_nNumAll;		//����

	D3DLIGHT9 m_light;	//���C�g�\����
};

#endif // !_LIGHT_H_
