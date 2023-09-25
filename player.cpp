//======================================================
//
//プレイヤー処理[player.cpp]
//Author:石原颯馬
//
//======================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "object.h"
#include "input.h"
#include "camera.h"
#include "objectX.h"
#include "xmodel.h"

//静的メンバ変数
const float CPlayer::MOVE_SPEED = 10.0f;		//カメラ移動速度
const float CPlayer::ROTATE_SPEED = 0.0012f;	//マウス移動での回転速度

//=================================
//コンストラクタ
//=================================
CPlayer::CPlayer()
{
	m_cursorPos = CManager::VEC3_ZERO;
}

//=================================
//デストラクタ
//=================================
CPlayer::~CPlayer()
{}

//=================================
//初期化
//=================================
HRESULT CPlayer::Init(void)
{
	return S_OK;
}

//=================================
//終了
//=================================
void CPlayer::Uninit(void)
{
	
}

//=================================
//更新
//=================================
void CPlayer::Update(void)
{
	CInputMouse* pMouse = CManager::GetInputMouse();	//マウス取得
	Move();

	if (pMouse->GetPress(CInputMouse::CLICK_RIGHT) == true)
	{//回転
		Rotate();
	}
	else
	{//マウスカーソル位置保存
		m_cursorPos = pMouse->GetPos();
	}

	ImGuiIO& io = ImGui::GetIO();
	if (pMouse->GetTrigger(CInputMouse::CLICK_LEFT) == true && io.WantCaptureMouse == false)
	{//位置特定
		Select();
	}
}

//=================================
//移動
//=================================
void CPlayer::Move(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	//キーボード取得
	CCamera* pCamera = CManager::GetCamera();					//カメラ取得
	D3DXVECTOR3 move = CManager::VEC3_ZERO;
	D3DXVECTOR3 rot = pCamera->GetRot();
	//移動
	if (pKeyboard->GetPress(DIK_A) == true)
	{
		move.x += -cosf(rot.y) * MOVE_SPEED;
		move.z += -sinf(rot.y) * MOVE_SPEED;
	}
	else if (pKeyboard->GetPress(DIK_D) == true)
	{
		move.x += cosf(rot.y) * MOVE_SPEED;
		move.z += sinf(rot.y) * MOVE_SPEED;
	}

	if (pKeyboard->GetPress(DIK_W) == true)
	{
		move.x += -sinf(rot.y) * MOVE_SPEED;
		move.z += cosf(rot.y) * MOVE_SPEED;
	}
	else if (pKeyboard->GetPress(DIK_S) == true)
	{
		move.x += sinf(rot.y) * MOVE_SPEED;
		move.z += -cosf(rot.y) * MOVE_SPEED;
	}

	//移動
	pCamera->SetCameraPos(move);
}

//=================================
//回転
//=================================
void CPlayer::Rotate(void)
{
	CCamera* pCamera = CManager::GetCamera();
	CInputMouse* pMouse = CManager::GetInputMouse();
	D3DXVECTOR3 rot = CManager::VEC3_ZERO;

	D3DXVECTOR3 move = CManager::VEC3_ZERO;
	move.x = pMouse->GetMove().x;
	move.y = pMouse->GetMove().y;

	rot.y -= move.x * ROTATE_SPEED;
	rot.x -= move.y * ROTATE_SPEED;

	pCamera->SetCameraRot(rot);

	//カーソルを元の位置に戻す
	POINT setCursorpos;
	setCursorpos.x = (LONG)m_cursorPos.x;
	setCursorpos.y = (LONG)m_cursorPos.y;
	ClientToScreen(FindWindowA(CLASS_NAME, nullptr), &setCursorpos);
	SetCursorPos(setCursorpos.x, setCursorpos.y);
}

//=================================
//選択
//=================================
void CPlayer::Select(void)
{
	CInputMouse* mouse = CManager::GetInputMouse();	//マウス取得

	//オブジェクト選択（0.0～1.0）
	D3DXVECTOR3 posNear = mouse->ConvertClickPosToWorld(0.0f);
	D3DXVECTOR3 posFar = mouse->ConvertClickPosToWorld(1.0f);

	//近い何か
	CObjectX* pObjectNear = nullptr;
	float fLengthNear = CManager::FLT_ZERO;

	CObjectX* pObject = CObjectX::GetTop();	//リストの最初を持ってくる

	while (pObject != nullptr)
	{//ぬるぬるになるまでやる
		CObjectX* pObjectNext = pObject->GetNext();	//次のオブジェ保存
		
		//本処理
		if (pObject->GetModel()->GetCollision().CollisionCheck(posNear, posFar, pObject->GetPos(), pObject->GetRot()))
		{
			float fLength = D3DXVec3Length(&(pObject->GetPos() - posNear));

			if (pObjectNear == nullptr || fLengthNear > fLength)
			{//近い
				pObjectNear = pObject;
				fLengthNear = fLength;
			}
		}

		pObject = pObjectNext;	//次を入れる
	}

	if (pObjectNear != nullptr)
	{//何かしら選択できた
		m_pObject = pObjectNear;	//クリックしたオブジェを入れておく
	}
	else
	{//何も選択してない
		m_pObject = nullptr;
	}
}
