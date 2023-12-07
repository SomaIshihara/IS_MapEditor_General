//======================================================
//
//GUIオブジェクト[gui.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "object.h"
#include "objectX.h"
#include "texture.h"
#include "gui.h"
#include "debugproc.h"
#include "input.h"
#include "player.h"
#include "xmodel.h"
#include "meshField.h"
#include "objloader.h"
#include "interface.h"
#include "manipulation.h"

//************************************************
//オブジェ追加
//************************************************
//=================================
//コンストラクタ
//=================================
CGUIAddObj::CGUIAddObj() : CObject(PRIORITY_05)
{
	m_SelectObj = nullptr;
	m_pos = CManager::VEC3_ZERO;
	m_rot = CManager::VEC3_ZERO;
	m_bDragged = false;
}

//=================================
//デストラクタ
//=================================
CGUIAddObj::~CGUIAddObj()
{
}

//========================
//初期化処理
//========================
HRESULT CGUIAddObj::Init(void)
{
	return S_OK;
}

//========================
//終了処理
//========================
void CGUIAddObj::Uninit(void)
{
	//自分自身破棄
	Release();
}

//========================
//更新処理
//========================
void CGUIAddObj::Update(void)
{
	//ImGUI
	ImGuiIO& io = ImGui::GetIO();

	//メッシュフィールド
	static int width = 1;
	static int height = 1;
	static float length = 100.0f;

	// Demonstrate the various window flags. Typically you would just use the default!
	static bool no_titlebar = false;
	static bool no_scrollbar = false;
	static bool no_menu = false;
	static bool no_move = false;
	static bool no_resize = false;
	static bool no_collapse = false;
	static bool no_close = false;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = false;
	static bool unsaved_document = false;

	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;

	ImGui::SetNextWindowPos(ImVec2(900, 50));
	ImGui::SetNextWindowSize(ImVec2(335, 260), ImGuiCond_Once);	// サイズ
	ImGui::Begin(u8"Main Window", nullptr, window_flags);			// 名前

	ImGui::Text("Select Model");

	if (ImGui::BeginListBox("ModelList", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		CXModel* pModel = CXModel::GetTop();

		while (pModel != nullptr)
		{
			CXModel* pNextModel = pModel->GetNext();

			const bool is_selected = (m_SelectObj == pModel);
			if (ImGui::Selectable(pModel->GetPath(), is_selected))
				m_SelectObj = pModel;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();

			pModel = pNextModel;
		}

		ImGui::EndListBox();
	}

	//配置
	if (m_SelectObj != nullptr)
	{
		ImGui::Text("-->Drag and drop placement<--");
		ImGui::ImageButton(CManager::GetTextureSystem()->GetAddress(0), ImVec2(48.0f, 48.0f));
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{//ドラッグ開始
			ImGui::SetDragDropPayload("Place", nullptr, 0);
			ImGui::Text("Drop Place");
			ImGui::EndDragDropSource();
			m_bDragged = true;
		}
		else if (m_bDragged == true)
		{//ドラッグ終了
			m_bDragged = false;

			if (io.WantCaptureMouse == false)
			{
				CInputMouse* mouse = CManager::GetInputMouse();	//マウス取得

				//オブジェクト選択（0.0～1.0）
				D3DXVECTOR3 posNear = mouse->ConvertClickPosToWorld(0.0f);
				D3DXVECTOR3 posFar = mouse->ConvertClickPosToWorld(1.0f);

				D3DXPLANE plane;
				D3DXPlaneFromPointNormal(&plane, &CManager::VEC3_ZERO, &(D3DXVECTOR3(0.0f, 1.0f, 0.0f)));
				D3DXVECTOR3 posCloss;
				D3DXPlaneIntersectLine(&posCloss, &plane, &posNear, &posFar);
				posCloss.x = ceilf(posCloss.x);
				posCloss.y = ceilf(posCloss.y);
				posCloss.z = ceilf(posCloss.z);
 				CObjectX::Create(posCloss, CManager::VEC3_ZERO, m_SelectObj);
			}
		}
	}

	//モデル削除
	if (ImGui::Button("Unload Model") == true && m_SelectObj != nullptr)
	{
		ImGui::OpenPopup("Unload Model?");
	}
	if (ImGui::BeginPopupModal("Unload Model?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Read Model Unload?");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0))) 
		{ 
			CManager::GetPlayer()->UnsetSelObj();
			m_SelectObj->Unload();
			m_SelectObj = nullptr;
			ImGui::CloseCurrentPopup(); 
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) 
		{ 
			ImGui::CloseCurrentPopup(); 
		}
		ImGui::EndPopup();
	}

	ImGui::End();
}

//========================
//描画処理
//========================
void CGUIAddObj::Draw(void)
{
}

//========================
//生成処理
//========================
CGUIAddObj* CGUIAddObj::Create(void)
{
	CGUIAddObj* pObj = nullptr;

	if (pObj == nullptr)
	{
		//オブジェクトの生成
		pObj = new CGUIAddObj;

		//初期化
		pObj->Init();

		return pObj;
	}
	else
	{
		return nullptr;
	}
}

//************************************************
//オブジェ変更
//************************************************
//=================================
//コンストラクタ
//=================================
CGUIChangeObj::CGUIChangeObj() : CObject(PRIORITY_05)
{
	m_pos = CManager::VEC3_ZERO;
	m_rot = CManager::VEC3_ZERO;
	m_bDragged = false;
	m_bSizeChange = false;
	m_bBlockChange = false;
	m_aSize[0] = 0.0f;
	m_aSize[1] = 0.0f;
	m_aBlock[0] = 1;
	m_aBlock[1] = 1;
}

//=================================
//デストラクタ
//=================================
CGUIChangeObj::~CGUIChangeObj()
{
}

//========================
//初期化処理
//========================
HRESULT CGUIChangeObj::Init(void)
{
	return S_OK;
}

//========================
//終了処理
//========================
void CGUIChangeObj::Uninit(void)
{
	//自分自身破棄
	Release();
}

//========================
//更新処理
//========================
void CGUIChangeObj::Update(void)
{
	//ImGUI
	ImGuiIO& io = ImGui::GetIO();

	//メッシュフィールド
	static int width = 1;
	static int height = 1;
	static float length = 100.0f;

	// Demonstrate the various window flags. Typically you would just use the default!
	static bool no_titlebar = false;
	static bool no_scrollbar = false;
	static bool no_menu = false;
	static bool no_move = false;
	static bool no_resize = false;
	static bool no_collapse = false;
	static bool no_close = false;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = false;
	static bool unsaved_document = false;

	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;

	ImGui::SetNextWindowPos(ImVec2(900, 350));
	ImGui::SetNextWindowSize(ImVec2(335, 350), ImGuiCond_Once);	// サイズ
	ImGui::Begin(u8"Change Model", nullptr, window_flags);			// 名前

	CManipulationObj* pObject = CManager::GetPlayer()->GetSelObj();

	//オブジェクト選択してる？
	if (pObject != nullptr)
	{//してるよ
		//インターフェース取得
		IManipulation* face = pObject->GetInterface();

		D3DXVECTOR3 dxPos = face->GetPos();
		D3DXVECTOR3 dxRot = face->GetRot();

		//UI表示
		ImGui::Text("[Selected]");
		ImGui::DragFloat3("Pos", dxPos, 1.0f);
		ImGui::DragFloat3("Rot", dxRot, 0.01f, -D3DX_PI, D3DX_PI);

		//オブジェクトに入れる
		face->SetPos(dxPos);
		face->SetRot(dxRot);

		//跡形もなく消し去るボタン
		CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();
		if (ImGui::Button("Delete") || pKeyboard->GetTrigger(DIK_DELETE))
		{//ぽちっとな
			CManager::GetPlayer()->UnsetSelObj();
			face->Delete();
		}

		//各オブジェごとの処理
		if (face->GetType() == IManipulation::TYPE_OBJX)
		{
			DispUI_OBJX(face);

			//モデル複製
			if (ImGui::Button("Copy") == true)
			{
				IManipulation* pCopyInterface = CObjectX::Create(face->GetPos(), face->GetRot(), face->GetModel());	//同じものを生成
				face = pCopyInterface;	//生成したものを選択
			}
		}
		else if (face->GetType() == IManipulation::TYPE_MESHFIELD)
		{
			DispUI_MF(face);

			//モデル複製
			if (ImGui::Button("Copy") == true)
			{
				IManipulation* pCopyInterface = CMeshField::Create(face->GetPos(), face->GetRot(), face->GetWidth(),
					face->GetDepth(), face->GetBlockWidth(), face->GetBlockDepth());	//同じものを生成
				face = pCopyInterface;	//生成したものを選択
			}
		}
	}
	else
	{//してないよ
		ImGui::Text("[No Selected]");
	}

	ImGui::End();
}

//========================
//描画処理
//========================
void CGUIChangeObj::Draw(void)
{
}

//========================
//生成処理
//========================
CGUIChangeObj* CGUIChangeObj::Create(void)
{
	CGUIChangeObj* pObj = nullptr;

	if (pObj == nullptr)
	{
		//オブジェクトの生成
		pObj = new CGUIChangeObj;

		//初期化
		pObj->Init();

		return pObj;
	}
	else
	{
		return nullptr;
	}
}

//========================
//OBJXの時のUI表示
//========================
void CGUIChangeObj::DispUI_OBJX(IManipulation* face)
{
	//ユーザー定義
	ImGui::Text("User Definition");

	CVariableManager* pVariableManager = CManager::GetVariableManager();
	for (int cnt = 0; cnt < pVariableManager->GetDefinedNum(); cnt++)
	{
		switch (face->GetVariable()[cnt]->GetType())
		{
		case CVariable::Integer:
		{
			int nData = *(int*)face->GetVariable()[cnt]->GetData();
			ImGui::InputInt(face->GetVariable()[cnt]->GetName(), &nData);
			face->GetVariable()[cnt]->SetData(&nData);
		}
		break;
		case CVariable::Float:
		{
			float fData = *(float*)face->GetVariable()[cnt]->GetData();
			ImGui::InputFloat(face->GetVariable()[cnt]->GetName(), &fData);
			face->GetVariable()[cnt]->SetData(&fData);
		}
		break;
		case CVariable::Boolean:
		{
			bool bData = (*(unsigned char*)face->GetVariable()[cnt]->GetData() == 0xff) ? true : false;
			ImGui::Checkbox(face->GetVariable()[cnt]->GetName(), &bData);
			face->GetVariable()[cnt]->SetData(&bData);
		}
		break;
		default:
			assert(false);
			break;
		}
	}
}

//========================
//メッシュフィールドの時のUI表示
//========================
void CGUIChangeObj::DispUI_MF(IManipulation* face)
{
	//メッシュフィールド専用パラメータ
	ImGui::Text("MeshField Paramater");

	//必要な変数
	//UI表示前
	float aSize[2];
	int aBlock[2];
	if (m_bSizeChange == true)
	{
		aSize[0] = m_aSize[0];
		aSize[1] = m_aSize[1];
	}
	else
	{
		aSize[0] = face->GetWidth();
		aSize[1] = face->GetDepth();
	}
	if (m_bBlockChange == true)
	{
		aBlock[0] = m_aBlock[0];
		aBlock[1] = m_aBlock[1];
	}
	else
	{
		aBlock[0] = face->GetBlockWidth();
		aBlock[1] = face->GetBlockDepth();
	}

	//UI表示後
	float aSizeChange[2];
	aSizeChange[0] = aSize[0];
	aSizeChange[1] = aSize[1];

	int aBlockChange[2];
	aBlockChange[0] = aBlock[0];
	aBlockChange[1] = aBlock[1];

	//調整
	ImGui::DragFloat2("BlockSize", &aSizeChange[0], 1.0f, 1.0f);
	ImGui::DragInt2("BlockNum", &aBlockChange[0],0.1f,1);
	
	//変更があれば設定
	if (aSize[0] != aSizeChange[0] || aSize[1] != aSizeChange[1])
	{
		m_bSizeChange = true;
		m_aSize[0] = aSizeChange[0];
		if (m_aSize[0] <= 0.0f)
		{
			m_aSize[0] = 1.0f;
		}
		m_aSize[1] = aSizeChange[1];
		if (m_aSize[1] <= 0.0f)
		{
			m_aSize[1] = 1.0f;
		}
	}
	else if(m_bSizeChange == true && CManager::GetInputMouse()->GetPress(CInputMouse::CLICK_LEFT) == false)
	{
		m_bSizeChange = false;
		face->SetSize(m_aSize[0], 0.0f, m_aSize[1]);
	}
	if (aBlock[0] != aBlockChange[0] || aBlock[1] != aBlockChange[1])
	{
		m_bBlockChange = true;
		m_aBlock[0] = aBlockChange[0];
		if (m_aBlock[0] <= 0)
		{
			m_aBlock[0] = 1;
		}
		m_aBlock[1] = aBlockChange[1];
		if (m_aBlock[1] <= 0)
		{
			m_aBlock[1] = 1;
		}
	}
	else if (m_bBlockChange == true && CManager::GetInputMouse()->GetPress(CInputMouse::CLICK_LEFT) == false)
	{
		m_bBlockChange = false;
		face->SetBlockNum(m_aBlock[0], m_aBlock[1]);
	}
}
