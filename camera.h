//==========================================
//
//�J�����v���O�����̃w�b�_[camera.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//�N���X���������̂�������ł�
class CCamera
{
public:
	//�ÓIconst
	static const float LENGTH;
	static const float ROT_X_MIN;
	static const float ROT_X_MAX;

	//�R���X�g���N�^�E�f�X�g���N�^
	CCamera();
	~CCamera();
	
	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//�ݒ菈��
	void SetCamera(void);

	//�ʒu�ړ�
	void SetCameraPos(const D3DXVECTOR3 move);
	void SetCameraRot(const D3DXVECTOR3 rot);

	//�擾
	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	D3DXVECTOR3 GetPosR(void) { return m_posR; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
private:
	//�p�x�C��
	void FixPosV(void);
	void FixPosR(void);
	void FixRot(void);

	//�����o�ϐ�
	D3DXMATRIX m_mtxView;		//�r���[�}�g��
	D3DXMATRIX m_mtxProjection;	//�v���W�F�N�V�����}�g��
	D3DXVECTOR3 m_posV;			//���_
	D3DXVECTOR3 m_posR;			//�����_
	D3DXVECTOR3 m_vecU;			//��x�N�g��
	D3DXVECTOR3 m_rot;			//�p�x
	float m_fLength;			//����
};

#endif // !_CAMERA_H_
