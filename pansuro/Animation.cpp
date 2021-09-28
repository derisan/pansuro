#include "pch.h"
#include "Animation.h"
#include "Skeleton.h"

#include <rapidjson/document.h>

void Animation::Load(const std::wstring& path)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		MK_ASSERT(nullptr, "Animation file not found.");
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

	const rapidjson::Value& sequence = doc["sequence"];
	const rapidjson::Value& frames = sequence["frames"];
	const rapidjson::Value& length = sequence["length"];
	const rapidjson::Value& bonecount = sequence["bonecount"];

	m_NumFrames = frames.GetUint();
	m_Duration = length.GetFloat();
	m_NumBones = bonecount.GetUint();
	m_FrameDuration = m_Duration / (m_NumFrames - 1);

	m_Tracks.resize(m_NumBones);

	const rapidjson::Value& tracks = sequence["tracks"];

	for (rapidjson::SizeType i = 0; i < tracks.Size(); i++)
	{
		UINT boneIndex = tracks[i]["bone"].GetUint();

		const rapidjson::Value& transforms = tracks[i]["transforms"];

		BoneTransform temp;

		for (rapidjson::SizeType j = 0; j < transforms.Size(); j++)
		{
			const rapidjson::Value& rot = transforms[j]["rot"];
			const rapidjson::Value& trans = transforms[j]["trans"];

			temp.Rotation.x = rot[0].GetFloat();
			temp.Rotation.y = rot[1].GetFloat();
			temp.Rotation.z = rot[2].GetFloat();
			temp.Rotation.w = rot[3].GetFloat();

			temp.Translation.x = trans[0].GetFloat();
			temp.Translation.y = trans[1].GetFloat();
			temp.Translation.z = trans[2].GetFloat();

			m_Tracks[boneIndex].emplace_back(temp);
		}
	}
}

void Animation::GetGlobalPoseAtTime(std::vector<Matrix>& outPoses, Skeleton* inSkeleton, float inTime) const
{
	if (outPoses.size() != m_NumBones)
	{
		outPoses.resize(m_NumBones);
	}

	UINT frame = static_cast<UINT>(inTime / m_FrameDuration);
	UINT nextFrame = frame + 1;

	float pct = inTime / m_FrameDuration - frame;

	if (m_Tracks[0].size() > 0)
	{
		BoneTransform interp = BoneTransform::Interpolate(m_Tracks[0][frame],
			m_Tracks[0][nextFrame], pct);
		outPoses[0] = interp.ToMatrix();
	}
	else
	{
		outPoses[0] = Matrix::Identity;
	}

	const std::vector<Bone>& bones = inSkeleton->GetBones();
	for (UINT bone = 1; bone < m_NumBones; bone++)
	{
		Matrix mat;

		if (m_Tracks[bone].size() > 0)
		{
			BoneTransform interp = BoneTransform::Interpolate(m_Tracks[bone][frame],
				m_Tracks[bone][nextFrame], pct);

			mat = interp.ToMatrix();
		}

		mat *= outPoses[bones[bone].Parent];
		outPoses[bone] = mat;
	}
}
