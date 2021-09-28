#include "pch.h"
#include "Skeleton.h"

#include <rapidjson/document.h>

void Skeleton::Load(const std::wstring& path)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		MK_ASSERT(nullptr, "Skel file not found.");
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		MK_ASSERT(nullptr, "It's not valid json file.");
	}

	const rapidjson::Value& boneCount = doc["bonecount"];
	if (!boneCount.IsUint())
	{
		MK_ASSERT(nullptr, "Skel file doesn't have a bone count.");
	}

	UINT count = boneCount.GetUint();

	if (count > MAX_SKELETON_BONES)
	{
		MK_ASSERT(nullptr, "Skel exceeds maximum bone count.");
	}

	m_Bones.reserve(count);

	const rapidjson::Value& bones = doc["bones"];
	if (!bones.IsArray())
	{
		MK_ASSERT(nullptr, "Skel file doesn't have any bone array.");
	}

	if (bones.Size() != count)
	{
		MK_ASSERT(nullptr, "Skeleton has a mismatch between the bone count and number of bones");
	}

	Bone temp;

	for (rapidjson::SizeType i = 0; i < count; i++)
	{
		if (!bones[i].IsObject())
		{
			MK_ASSERT(nullptr, "Bone is invalid.");
		}

		const rapidjson::Value& name = bones[i]["name"];
		temp.Name = name.GetString();

		const rapidjson::Value& parent = bones[i]["parent"];
		temp.Parent = parent.GetInt();

		const rapidjson::Value& bindPose = bones[i]["bindpose"];

		const rapidjson::Value& rot = bindPose["rot"];
		const rapidjson::Value& trans = bindPose["trans"];

		temp.LocalBindPose.Rotation.x = rot[0].GetFloat();
		temp.LocalBindPose.Rotation.y = rot[1].GetFloat();
		temp.LocalBindPose.Rotation.z = rot[2].GetFloat();
		temp.LocalBindPose.Rotation.w = rot[3].GetFloat();

		temp.LocalBindPose.Translation.x = trans[0].GetFloat();
		temp.LocalBindPose.Translation.y = trans[1].GetFloat();
		temp.LocalBindPose.Translation.z = trans[2].GetFloat();

		m_Bones.emplace_back(std::move(temp));
	}

	ComputeGlobalInvBindPose();
}

void Skeleton::ComputeGlobalInvBindPose()
{
	m_GlobalInvBindPoses.resize(GetNumBones());

	m_GlobalInvBindPoses[0] = m_Bones[0].LocalBindPose.ToMatrix();

	for (UINT i = 1; i < m_GlobalInvBindPoses.size(); i++)
	{
		Matrix mat = m_Bones[i].LocalBindPose.ToMatrix();
		mat *= m_GlobalInvBindPoses[m_Bones[i].Parent];

		m_GlobalInvBindPoses[i] = mat;
	}

	for (UINT i = 0; i < m_GlobalInvBindPoses.size(); i++)
	{
		m_GlobalInvBindPoses[i] = m_GlobalInvBindPoses[i].Invert();
	}
}
