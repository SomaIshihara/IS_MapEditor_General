//======================================================
//
//GUI�I�u�W�F�N�g[gui.cpp]
//Author:�Ό��D�n
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
//�I�u�W�F�ǉ�
//************************************************
//=================================
//�R���X�g���N�^
//=================================
CGUIAddObj::CGUIAddObj() : CObject(PRIORITY_05)
{
	m_SelectObj = nullptr;
	m_pos = CManager::VEC3_ZERO;
	m_rot = CManager::VEC3_ZERO;
	m_bDragged = false;
}

//=================================
//�f�X�g���N�^
//=================================
CGUIAddObj::~CGUIAddObj()
{
}

//========================
//����������
//========================
HRESULT CGUIAddObj::Init(void)
{
	return S_OK;
}

//========================
//�I������
//========================
void CGUIAddObj::Uninit(void)
{
	//�������g�j��
	Release();
}

//========================
//�X�V����
//========================
void CGUIAddObj::Update(void)
{
	//ImGUI
	ImGuiIO& io = ImGui::GetIO();

	//���b�V���t�B�[���h
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
	ImGui::SetNextWindowSize(ImVec2(335, 260), ImGuiCond_Once);	// �T�C�Y
	ImGui::Begin(u8"Main Window", nullptr, window_flags);			// ���O

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

	//�z�u
	if (m_SelectObj != nullptr)
	{
		ImGui::Text("-->Drag and drop placement<--");
		ImGui::ImageButton(CManager::GetTextureSystem()->GetAddress(0), ImVec2(48.0f, 48.0f));
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{//�h���b�O�J�n
			ImGui::SetDragDropPayload("Place", nullptr, 0);
			ImGui::Text("Drop Place");
			ImGui::EndDragDropSource();
			m_bDragged = true;
		}
		else if (m_bDragged == true)
		{//�h���b�O�I��
			m_bDragged = false;

			if (io.WantCaptureMouse == false)
			{
				CInputMouse* mouse = CManager::GetInputMouse();	//�}�E�X�擾

				//�I�u�W�F�N�g�I���i0.0�`1.0�j
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

	//���f���폜
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
//�`�揈��
//========================
void CGUIAddObj::Draw(void)
{
}

//========================
//��������
//========================
CGUIAddObj* CGUIAddObj::Create(void)
{
	CGUIAddObj* pObj = nullptr;

	if (pObj == nullptr)
	{
		//�I�u�W�F�N�g�̐���
		pObj = new CGUIAddObj;

		//������
		pObj->Init();

		return pObj;
	}
	else
	{
		return nullptr;
	}
}

//************************************************
//�I�u�W�F�ύX
//************************************************
//=================================
//�R���X�g���N�^
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
//�f�X�g���N�^
//=================================
CGUIChangeObj::~CGUIChangeObj()
{
}

//========================
//����������
//========================
HRESULT CGUIChangeObj::Init(void)
{
	return S_OK;
}

//========================
//�I������
//========================
void CGUIChangeObj::Uninit(void)
{
	//�������g�j��
	Release();
}

//========================
//�X�V����
//========================
void CGUIChangeObj::Update(void)
{
	//ImGUI
	ImGuiIO& io = ImGui::GetIO();

	//���b�V���t�B�[���h
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
	ImGui::SetNextWindowSize(ImVec2(335, 350), ImGuiCond_Once);	// �T�C�Y
	ImGui::Begin(u8"Change Model", nullptr, window_flags);			// ���O

	CManipulationObj* pObject = CManager::GetPlayer()->GetSelObj();

	//�I�u�W�F�N�g�I�����Ă�H
	if (pObject != nullptr)
	{//���Ă��
		//�C���^�[�t�F�[�X�擾
		IManipulation* face = pObject->GetInterface();

		D3DXVECTOR3 dxPos = face->GetPos();
		D3DXVECTOR3 dxRot = face->GetRot();

		//UI�\��
		ImGui::Text("[Selected]");
		ImGui::DragFloat3("Pos", dxPos, 1.0f);
		ImGui::DragFloat3("Rot", dxRot, 0.01f, -D3DX_PI, D3DX_PI);

		//�I�u�W�F�N�g�ɓ����
		face->SetPos(dxPos);
		face->SetRot(dxRot);

		//�Ռ`���Ȃ���������{�^��
		CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();
		if (ImGui::Button("Delete") || pKeyboard->GetTrigger(DIK_DELETE))
		{//�ۂ����Ƃ�
			CManager::GetPlayer()->UnsetSelObj();
			face->Delete();
		}

		//�e�I�u�W�F���Ƃ̏���
		if (face->GetType() == IManipulation::TYPE_OBJX)
		{
			DispUI_OBJX(face);

			//���f������
			if (ImGui::Button("Copy") == true)
			{
				IManipulation* pCopyInterface = CObjectX::Create(face->GetPos(), face->GetRot(), face->GetModel());	//�������̂𐶐�
				face = pCopyInterface;	//�����������̂�I��
			}
		}
		else if (face->GetType() == IManipulation::TYPE_MESHFIELD)
		{
			DispUI_MF(face);

			//���f������
			if (ImGui::Button("Copy") == true)
			{
				IManipulation* pCopyInterface = CMeshField::Create(face->GetPos(), face->GetRot(), face->GetWidth(),
					face->GetDepth(), face->GetBlockWidth(), face->GetBlockDepth());	//�������̂𐶐�
				face = pCopyInterface;	//�����������̂�I��
			}
		}
	}
	else
	{//���ĂȂ���
		ImGui::Text("[No Selected]");
	}

	ImGui::End();
}

//========================
//�`�揈��
//========================
void CGUIChangeObj::Draw(void)
{
}

//========================
//��������
//========================
CGUIChangeObj* CGUIChangeObj::Create(void)
{
	CGUIChangeObj* pObj = nullptr;

	if (pObj == nullptr)
	{
		//�I�u�W�F�N�g�̐���
		pObj = new CGUIChangeObj;

		//������
		pObj->Init();

		return pObj;
	}
	else
	{
		return nullptr;
	}
}

//========================
//OBJX�̎���UI�\��
//========================
void CGUIChangeObj::DispUI_OBJX(IManipulation* face)
{
	//���[�U�[��`
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
//���b�V���t�B�[���h�̎���UI�\��
//========================
void CGUIChangeObj::DispUI_MF(IManipulation* face)
{
	//���b�V���t�B�[���h��p�p�����[�^
	ImGui::Text("MeshField Paramater");

	//�K�v�ȕϐ�
	//UI�\���O
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

	//UI�\����
	float aSizeChange[2];
	aSizeChange[0] = aSize[0];
	aSizeChange[1] = aSize[1];

	int aBlockChange[2];
	aBlockChange[0] = aBlock[0];
	aBlockChange[1] = aBlock[1];

	//����
	ImGui::DragFloat2("BlockSize", &aSizeChange[0], 1.0f, 1.0f);
	ImGui::DragInt2("BlockNum", &aBlockChange[0],0.1f,1);
	
	//�ύX������ΐݒ�
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
