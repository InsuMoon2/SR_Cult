#pragma once

#include "CComponent.h"
#include "CTransform.h"
#include "CState.h"
#include "CCombatStat.h"
#include "CTimerMgr.h"
#include "CCameraCom.h"

USING(Engine)

void Transform_Inspector(CTransform* transform)
{
    if (!transform)
        return;

    _vec3 pos = transform->Get_Pos();
    ImGui::Text("Position : ");
    ImGui::SameLine();
    ImGui::PushID("TransformPos");
    {
        if (ImGui::DragFloat3("##PositionDrag", &pos.x, 0.1f))
            transform->Set_Pos(pos);
    }
    ImGui::PopID();

    ImGui::Text("Rotation : ");

    static _vec3 tempRot;
    static bool  isInit = false;

    if (!isInit)
    {
        tempRot = transform->Get_Rotation();  
        isInit = true;
    }

    ImGui::PushID("TransformRot");
    {
        // X축
        float x = tempRot.x;
        if (ImGui::DragFloat("Rot X", &x, 0.005f))
        {
            tempRot.x = x;
            transform->Set_RotationAxis(ROT_X, tempRot.x);
        }

        // Y축
        float y = tempRot.y;
        if (ImGui::DragFloat("Rot Y", &y, 0.005f))
        {
            tempRot.y = y;
            transform->Set_RotationAxis(ROT_Y, tempRot.y);
        }

        // Z축
        float z = tempRot.z;
        if (ImGui::DragFloat("Rot Z", &z, 0.005f))
        {
            tempRot.z = z;
            transform->Set_RotationAxis(ROT_Z, tempRot.z);
        }
    }
    ImGui::PopID();
}

void State_Inspector(CState* state)
{
    if (state == nullptr)
        return;

    const char* curState = ToString(state->Get_State());
    const char* curDir = ToString(state->Get_Dir());

    ImGui::PushID("State");
    {
        // --- State ---
        ImGui::Text("State : ");
        ImGui::SameLine();
        ImGui::TextUnformatted(curState);

        // --- Dir ---
        ImGui::Text("Dir   : ");
        ImGui::SameLine();
        ImGui::TextUnformatted(curDir);
    }
    ImGui::PopID();
}

void CombatStat_Inspector(CCombatStat* stat)
{
    if (stat == nullptr)
        return;

    _float maxHp = stat->Get_MaxHp();
    _float hp = stat->Get_Hp();

    _float maxMp = stat->Get_MaxMp();
    _float mp = stat->Get_Mp();

    _float speed = stat->Get_Speed();
    _float attack = stat->Get_Attack();

    ImGui::PushID("CombatStat");
    {
        const float column_width = 80.0f;

        ImGui::Columns(2, "HpColumns", false);
        ImGui::SetColumnWidth(0, column_width);

        // --- Hp ---
        ImGui::Text("Max Hp : ");
        ImGui::NextColumn();
        if (ImGui::DragFloat("##MaxHp", &maxHp, 0.1f))
        {
            stat->Set_MaxHp(maxHp);
        }
        ImGui::NextColumn();

        ImGui::Text("Hp : ");
        ImGui::NextColumn();
        if(ImGui::DragFloat("##CurHp", &hp, 0.1f))
        {
            stat->Set_Hp(hp);
        }
        ImGui::NextColumn();

        ImGui::Columns(1);

        ImGui::NewLine();

        // --- Mp ---
        ImGui::Columns(2, "MpColumns", false);
        ImGui::SetColumnWidth(0, column_width);

        ImGui::Text("Max Mp : ");
        ImGui::NextColumn();
        if(ImGui::DragFloat("##MaxMp", &maxMp, 0.1f))
        {
            stat->Set_MaxMp(maxMp);
        }
        ImGui::NextColumn();

        ImGui::Text("Mp : ");
        ImGui::NextColumn();
        if(ImGui::DragFloat("##CurMp", &mp, 0.1f))
        {
            stat->Set_Mp(mp);
        }
        ImGui::NextColumn();

        ImGui::Columns(1);

        // --- Speed ---
        ImGui::Columns(2, "EtcColums", false);
        ImGui::SetColumnWidth(0, column_width);

        ImGui::Text("Speed : ");
        ImGui::NextColumn();
        if(ImGui::DragFloat("##Speed", &speed, 0.1f))
        {
            stat->Set_Speed(speed);
        }
        ImGui::NextColumn();

        // --- Attack ---
        ImGui::Text("Attack : ");
        ImGui::NextColumn();
        if(ImGui::DragFloat("##Attack", &attack, 0.1f))
        {
            stat->Set_Attack(attack);
        }
        ImGui::NextColumn();

        ImGui::Columns(1);
    }
    ImGui::PopID();


}

void Camera_Inspector(CCameraCom* camera)
{
    
}
