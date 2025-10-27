#include "TransformDebugMouse.h"
#include "../ResourceStructures/MapTextures.h"
#include "../DebugUIStructures/DebugUI.h"
#include "../CameraStructures/CameraBehavior.h"
#include "../CollisionDetectionsStructures/CollisionDetection.h"
#include "../BaseClassStructures/Segment/Segment.h"
#include "../BenriFunctionStructures/BenriFunctions.h"
#include <functional>
#include "../CubeForModelStructures/SimpleBone.h"

void TransformDebugMouse::ReleaseRegistration(VertexData* anotherVertex_)
{
	for (auto itr_datas = touchableTransDatas.begin(); itr_datas != touchableTransDatas.end(); )
	{
		if (!(*itr_datas).second.anotherVertex)
		{
			++itr_datas;
		}

		else
		{
			if ((*itr_datas).second.anotherVertex == anotherVertex_)
			{
				touchableTransDatas.erase(itr_datas++);
			}

			else
			{
				++itr_datas;
			}
		}
	}
}

TransformDebugMouse::TransformDebugMouse()
{
	draggingTarget.first = nullptr;
	draggingTarget.second = 0.0f;
	mouseSprite.InitializeSprite({0,0,0}, 32, 32, (int)TagTexture::kCursorImage);

	DebugSceneUI::GetInstance()->AddToDebugUI("TransformDebugMouse", std::bind(&TransformDebugMouse::Debug, this));
}

void TransformDebugMouse::IsCollision(Matrix4 vpMat_, Vector3 cameraPos_)
{
	float const max_radius = 20.0f;
	float const min_radius = 0.5f;
	float const max_diffZ = 50.0f;

	Vector2 f_mousePos = { (float)NoviceLike::mousePos.x,(float)NoviceLike::mousePos.y };

	for (auto itr_datas = touchableTransDatas.begin(); itr_datas != touchableTransDatas.end(); ++itr_datas)
	{
		bool do_break = false;

		Vector2 screenPos = ConvertToScreen((*itr_datas).second.GetRegisterdVectorWorld(), vpMat_);
		Vector2 diff = Vector2{ screenPos.u - (float)NoviceLike::mousePos.x,screenPos.v - (float)NoviceLike::mousePos.y };
		float mag = sqrtf(diff.u * diff.u + diff.v * diff.v);
		float tetraRad = (cameraPos_ - (*itr_datas).second.GetRegisterdVectorWorld()).GetMagnitutde();
		float tmp = (1.0f - (std::clamp(tetraRad, min_radius, max_radius) / max_diffZ)) * max_radius;

		if (mag <= mouse_radius + tmp)
		{
			if (NoviceLike::mouseState.rgbButtons[0] && !NoviceLike::preMouseState.rgbButtons[0])
			{
				if (haveInitiative)
				{
					(*itr_datas).second.getFocusd = true;
					haveInitiative = false;
					break;
				}

				else
				{
					for (auto itr_datas = touchableTransDatas.begin(); itr_datas != touchableTransDatas.end(); ++itr_datas)
					{
						if ((*itr_datas).second.getFocusd)
						{
							(*itr_datas).second.getFocusd = false;
							haveInitiative = true;

							for (auto itr_datas2 = touchableTransDatas.begin(); itr_datas2 != touchableTransDatas.end(); ++itr_datas2)
							{
								Vector2 screenPos2 = ConvertToScreen((*itr_datas2).second.GetRegisterdVectorWorld(), vpMat_);
								Vector2 diff2 = Vector2{ screenPos2.u - (float)NoviceLike::mousePos.x,screenPos2.v - (float)NoviceLike::mousePos.y };
								float mag2 = sqrtf(diff2.u * diff2.u + diff2.v * diff2.v);
								float tetraRad2 = (cameraPos_ - (*itr_datas2).second.GetRegisterdVectorWorld()).GetMagnitutde();
								float tmp2 = (1.0f - (std::clamp(tetraRad2, min_radius, max_radius) / max_diffZ)) * max_radius;

								if (mag2 <= mouse_radius + tmp2)
								{
									if (itr_datas->first == itr_datas2->first)
									{
										break;
									}

									if (NoviceLike::mouseState.rgbButtons[0] && !NoviceLike::preMouseState.rgbButtons[0])
									{
										(*itr_datas2).second.getFocusd = true;
										haveInitiative = false;
										break;
									}
								}
							}

							do_break = true;

							break;
						}
					}
				}

				if (do_break) break;
			}


			(*itr_datas).second.tetra->drawMode = kSolid;
		}

		else 
		{
			(*itr_datas).second.tetra->drawMode = kWireFrame;
		}

		if ((*itr_datas).second.getFocusd) 
		{
			(*itr_datas).second.tetra->drawMode = kWireFrame;
		}
	}

	float const max_coe = 6.0f;
	float const max_scale = 4.0f;
	float const dragNumAdjust = 1.0f / 35.0f;
	showResponse = false;

	//GetFocusedの軸にマウスでアクセス
	for (auto itr_datas = touchableTransDatas.begin(); itr_datas != touchableTransDatas.end(); ++itr_datas)
	{
		if ((*itr_datas).second.getFocusd)
		{
			if (!vertexEdit&& (*itr_datas).second.anotherVertex)
			{
				continue;
			}

			Vector3* dst = nullptr;
			if (!(*itr_datas).second.anotherVertex)
			{
				dst = &(*itr_datas).second.trans->pos;
			}

			else
			{
				dst = &(*itr_datas).second.anotherVertex->position;
			}

			float coe = (*itr_datas).second.tetra->trans.scale.x / max_scale * max_coe;
			Min(coe, 1.0f);

			float len = 1.0f * coe;
			float len2 = len * 2.0f;
			Vector3 w_pos = (*itr_datas).second.GetRegisterdVectorWorld();

			Segment3D xAxis({ w_pos.x - len * 0.5f,w_pos.y,w_pos.z }, { w_pos.x + len,w_pos.y,w_pos.z });
			Segment3D yAxis({ w_pos.x ,w_pos.y - len * 0.5f,w_pos.z }, { w_pos.x ,w_pos.y + len,w_pos.z });
			Segment3D zAxis({ w_pos.x ,w_pos.y ,w_pos.z - len * 0.5f }, { w_pos.x ,w_pos.y ,w_pos.z + len });

			Segment2D sc_xAxis(ConvertToScreen(xAxis.s, vpMat_), ConvertToScreen(xAxis.e, vpMat_));
			Segment2D sc_yAxis(ConvertToScreen(yAxis.s, vpMat_), ConvertToScreen(yAxis.e, vpMat_));
			Segment2D sc_zAxis(ConvertToScreen(zAxis.s, vpMat_), ConvertToScreen(zAxis.e, vpMat_));

			if (CollisionDetections::Col2D::SegmentAndCircle(mouse_radius * 1.5f, f_mousePos, sc_xAxis.s, sc_xAxis.e))
			{
				if (NoviceLike::mouseState.rgbButtons[0])
				{
					if (!dragStart)
					{
						dragStart = true;
						draggingTarget.first = &dst->x;
						Vector2 cameraWatchingDir = (Vector2{ -cameraPos_.x ,cameraPos_.z }).GetNormalized();
						float cross2D = -cameraWatchingDir.v;
						if (cross2D >= 0.0f)	draggingTarget.second = 1.0f;
						else draggingTarget.second = -1.0f;

						draggingTarget.second *= cameraPos_.y;
					}
				}

				showResponse = true;
			}

			else if (CollisionDetections::Col2D::SegmentAndCircle(mouse_radius * 1.5f, f_mousePos, sc_yAxis.s, sc_yAxis.e))
			{
				if (NoviceLike::mouseState.rgbButtons[0])
				{
					if (!dragStart)
					{
						dragStart = true;
						draggingTarget.first = &dst->y;
						draggingTarget.second = 1;
					}
				}

				showResponse = true;
			}

			else if (CollisionDetections::Col2D::SegmentAndCircle(mouse_radius * 1.5f, f_mousePos, sc_zAxis.s, sc_zAxis.e))
			{
				if (NoviceLike::mouseState.rgbButtons[0])
				{
					if (!dragStart)
					{
						dragStart = true;
						draggingTarget.first = &dst->z;
						Vector2 cameraWatchingDir = (Vector2{ cameraPos_.x ,-cameraPos_.z }).GetNormalized();
						float cross2D = cameraWatchingDir.u;
						if (cross2D >= 0.0f)	draggingTarget.second = 1.0f;
						else draggingTarget.second = -1.0f;

					}
				}

				showResponse = true;
			}
		}
	}

	if (draggingTarget.first)
	{
		*draggingTarget.first += ((f_mousePos.u - preMousePos.u) * GetSign(draggingTarget.second) +
			(preMousePos.v - f_mousePos.v) * GetSign(draggingTarget.second)) * dragNumAdjust;

	}

	if (!NoviceLike::preMouseState.rgbButtons[0])
	{
		dragStart = false;
		draggingTarget.first = nullptr;
		draggingTarget.second = 0.0f;
	}


}


void TransformDebugMouse::Draw(Matrix4 vpMat_)
{
	float const max_coe = 6.0f;
	float const max_scale = 4.0f;

	if (mouseStatus == MouseStatus::kEdit)
	{
		for (auto itr_datas = touchableTransDatas.begin(); itr_datas != touchableTransDatas.end(); ++itr_datas)
		{
			if (!vertexEdit && (*itr_datas).second.anotherVertex)
			{
				continue;
			}

			(*itr_datas).second.tetra->Draw();

			if ((*itr_datas).second.getFocusd)
			{
				float coe = (*itr_datas).second.tetra->trans.scale.x / max_scale * max_coe;
				Min(coe, 1.0f);

				if (!(*itr_datas).second.anotherVertex)
				{
					Transform4* this_ = (*itr_datas).second.trans;
					NoviceLike::DrawAxis(this_->GetWorldPos(), &this_->targetDir, coe, vpMat_);
				}

				else
				{
					 NoviceLike::DrawAxis((*itr_datas).second.GetRegisterdVectorWorld(), nullptr, coe, vpMat_);
				}
			}
		}
	}

	mouseSprite.Draw();
}


void TransformDebugMouse::Register(std::string name_, Transform4* trans_, float size_, VertexData* another_)
{
	if (!isUpdating)
	{
		touchableTransDatas[name_].tetra = std::make_unique<DisjointedTetrahedra>();
		touchableTransDatas[name_].tetra->Initialize({ 0,0,0 }, TagTexture::kN_nani);
		touchableTransDatas[name_].anotherVertex = another_;
		touchableTransDatas[name_].trans = trans_;
		touchableTransDatas[name_].tetra->trans.parent_trans = trans_;

		if (!another_)
		{
			touchableTransDatas[name_].tetra->trans.scale = { size_ * 1.96f,size_ * 1.96f,size_ * 1.96f };
		}

		else
		{
			touchableTransDatas[name_].tetra->trans.scale = { 0.05f,0.05f,0.05f };
		}
	}
}



void TransformDebugMouse::Update(Matrix4 vpMat_, Vector3 cameraPos_)
{
	static float counter = 0.0f;
	counter += 6.28f / 240.0f;

	////マウスの座標を取得
	preMousePos = { (float)NoviceLike::mousePos.x ,(float)NoviceLike::mousePos.y };
	NoviceLike::SetMousePos(&NoviceLike::mousePos);
	ScreenToClient(pDxCommon->hwnd, &NoviceLike::mousePos);

	mouseSprite.trans.pos = { (float)NoviceLike::mousePos.x ,(float)NoviceLike::mousePos.y,0.0f };
	mouseSprite.Update();

	//RayCast(vpMat_);

	if (mouseStatus == MouseStatus::kEdit)
	{
		for (auto itr_datas = touchableTransDatas.begin(); itr_datas != touchableTransDatas.end(); ++itr_datas)
		{
			if (!vertexEdit && (*itr_datas).second.anotherVertex)
			{
				continue;
			}

			if ((*itr_datas).second.anotherVertex)
			{
				(*itr_datas).second.tetra->trans.pos = (*itr_datas).second.anotherVertex->position;
			}

			(*itr_datas).second.tetra->Update(vpMat_);
			(*itr_datas).second.tetra->SynchronizeTexAndColor();

			if ((*itr_datas).second.getFocusd)
			{
				(*itr_datas).second.tetra->color = { 60.0f * sinf(counter) ,0.0f ,60.0f * sinf(counter) ,255 };
			}

			else
			{
				(*itr_datas).second.tetra->color = { 150.0f ,150.0f,150.0f,255 };
			}
		}

		IsCollision(vpMat_, cameraPos_);
	}

	SetMouseColor();

}


void TransformDebugMouse::Debug()
{
	static int staticMousePos[2] = { NoviceLike::mousePos.x ,(int)NoviceLike::mousePos.y };
	staticMousePos[0] = (int)NoviceLike::mousePos.x;
	staticMousePos[1] = (int)NoviceLike::mousePos.y;

	ImGui::Checkbox("EditMode", &mouseStatus); 
	ImGui::Checkbox("ShowBone", &SimpleBone::showBone);
	ImGui::Checkbox("vertexEdit", &vertexEdit);

	ImGui::DragInt( "DrawMode", reinterpret_cast<int*>(&drawMode),1,0,1);

	ImGui::DragInt2("SCREEEEN", reinterpret_cast<int*>(&staticMousePos));
	for (auto itr_datas = touchableTransDatas.begin(); itr_datas != touchableTransDatas.end(); ++itr_datas)
	{
		if ((*itr_datas).second.getFocusd)
		{
			if (!vertexEdit && (*itr_datas).second.anotherVertex)
			{
				continue;
			}

			ImGui::Text((*itr_datas).first.c_str());
			if (!(*itr_datas).second.anotherVertex)
			{
				Transform4* this_ = (*itr_datas).second.trans;
				ImGui::DragFloat3("Local_POS", reinterpret_cast<float*>(&this_->pos), 0.025f);

				ImGui::DragFloat3("TargetDir", reinterpret_cast<float*>(&this_->targetDir), 0.0025f);
				if (ImGui::TreeNode("DefaultAxis"))
				{
					ImGui::DragFloat("X_axis", reinterpret_cast<float*>(&this_->defaultQuatenion.x_axis.deltaTheta), 0.25f);
					ImGui::DragFloat("Y_axis", reinterpret_cast<float*>(&this_->defaultQuatenion.y_axis.deltaTheta), 0.25f);
					ImGui::DragFloat("Z_axis", reinterpret_cast<float*>(&this_->defaultQuatenion.z_axis.deltaTheta), 0.25f);
					ImGui::TreePop();
				}
			}

			else
			{
				ImGui::DragFloat3("vertex_POS", reinterpret_cast<float*>(&(*itr_datas).second.anotherVertex->position), 0.025f);
				ImGui::DragFloat2("vertex_TEX", reinterpret_cast<float*>(&(*itr_datas).second.anotherVertex->texcoord), 0.025f);

			}
		}
	}

}


Vector3  TransData::GetRegisterdVectorWorld()
{
	return tetra->trans.GetWorldPos();
}

void TransformDebugMouse::SetMouseColor()
{
	mouseSprite.color.x = 255.0f * (showResponse);
	mouseSprite.color.y = 255.0f * (showResponse);
	mouseSprite.color.z = 255.0f * (showResponse);

	if (draggingTarget.first)
	{
		mouseSprite.color.x = 255.0f;
		mouseSprite.color.y = 0.0f;
		mouseSprite.color.z = 0.0f;
	}
}
