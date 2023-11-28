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
			CObjectX::Delete(m_SelectObj);
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

		float fPos[3], fRot[3];
		D3DXVECTOR3 dxPos = face->GetPos();
		D3DXVECTOR3 dxRot = face->GetRot();
		//float�z��ɓ����iImGUI�p�j
		fPos[0] = dxPos.x;
		fPos[1] = dxPos.y;
		fPos[2] = dxPos.z;
		fRot[0] = dxRot.x;
		fRot[1] = dxRot.y;
		fRot[2] = dxRot.z;

		//UI�\��
		ImGui::Text("[Selected]");
		ImGui::DragFloat3("Pos", fPos, 1.0f);
		ImGui::DragFloat3("Rot", fRot, 0.01f, -D3DX_PI, D3DX_PI);

		//Vec3�^�ɓ����
		dxPos.x = fPos[0];
		dxPos.y = fPos[1];
		dxPos.z = fPos[2];
		dxRot.x = fRot[0];
		dxRot.y = fRot[1];
		dxRot.z = fRot[2];

		//�I�u�W�F�N�g�ɓ����
		face->SetPos(dxPos);
		face->SetRot(dxRot);

		//�Ռ`���Ȃ���������{�^��
		if (ImGui::Button("Delete"))
		{//�ۂ����Ƃ�
			CManager::GetPlayer()->UnsetSelObj();
			face->Delete();
		}

		//���f������
		if (ImGui::Button("Copy") == true)
		{
			if (face->GetType() == IManipulation::TYPE_OBJX)
			{
				IManipulation* pCopyInterface = CObjectX::Create(face->GetPos(), face->GetRot(), face->GetModel());	//�������̂𐶐�
				face = pCopyInterface;	//�����������̂�I��
			}
		}

		if (face->GetType() == IManipulation::TYPE_OBJX)
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