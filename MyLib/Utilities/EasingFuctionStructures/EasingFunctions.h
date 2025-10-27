#pragma once
#include <functional>
#include "../BaseClassStructures/VectorAndMatrix/VectorAndMatrix.h"


//sin(θ(1−t))sin(θ)
namespace Easing
{
	template<typename T>
	static inline T Lerp(const T& st_, const T& end_, float t_)
	{
		return end_ * t_ + st_ * (1.0f - t_);
	}

	static inline Vector3 SLerp(Vector3 thisDir_, Vector3 otherDir_, float t_)
	{
		Vector3 ret;
		thisDir_ = thisDir_.GetNormalized();
		otherDir_ = otherDir_.GetNormalized();
		ret = thisDir_;
		//* 57.2958f
		float arcCos = std::acos(thisDir_.GetCos(otherDir_)) ;
		if (sinf(arcCos) >= 1.0e-5)
		{
			float len = std::sinf(arcCos * (1.0f - t_)) / sinf(arcCos);
			float len2 = std::sinf(arcCos * t_) / sinf(arcCos);
			ret = thisDir_ * len + otherDir_ * len2;
		}

		return ret;
	}

	static inline  Vector4<float> SLerp(Vector4<float> thisDir_, Vector4<float> otherDir_, float t_)
	{
		Vector4<float> ret;
		thisDir_ = thisDir_.GetNormalizedForQuaternion();
		otherDir_ = otherDir_.GetNormalizedForQuaternion();
		ret = thisDir_;
		
		float arcCos = std::acos(thisDir_.GetCosForQuaternion(otherDir_)) ;
		if (sinf(arcCos) >= 1.0e-5)
		{
			float len = std::sinf(arcCos * (1.0f - t_)) / sinf(arcCos);
			float len2 = std::sinf(arcCos * t_) / sinf(arcCos);
			ret = { thisDir_.x * len + otherDir_.x * len2,thisDir_.y * len + otherDir_.y * len2,
				thisDir_.z * len + otherDir_.z * len2,thisDir_.w * len + otherDir_.w * len2 };
		}

		return ret;
	}


	template<typename T>
	static inline T EasingInBack(const T& st_, const T& end_, float t_)
	{
		float const c1 = 1.70158f;
		float const c3 = c1 + 1.0f;

		float convertedT = c3 * t_ * t_ * t_ - c1 * t_ * t_;

		return Lerp(st_, end_, convertedT);
	}

	template<typename T>
	static inline T EaseOutBack(const T& st_, const T& end_, float t_)
	{
		float const c1 = 1.70158f;
		float const c3 = c1 + 1.0f;

		float convertedT = 1.0f + c3 * powf(t_ - 1.0f, 3.0f) + c1 * powf(t_ - 1.0f, 2.0f);

		return Lerp(st_, end_, convertedT);
	}

	template<typename T>
	static inline T EaseOutCubic(const T& st_, const T& end_, float t_)
	{
		float convertedT = 1.0f - (float)pow(1.0f - t_, 3.0f);

		return Lerp(st_, end_, convertedT);
	}

	template<typename T>
	static inline T EaseInExpo(const T& st_, const T& end_, float t_)
	{
		float convertedT = (float)powf(2.0f, 10.0f * t_ - 10.0f);

		return Lerp(st_, end_, convertedT);
	}

	template<typename T>
	static inline T EaseOutBounce(const T& st_, const T& end_, float t_)
	{
		float const n1 = 7.5625f;
		float const d1 = 2.75f;

		float convertedT;

		if (t_ < 1.0f / d1)
		{
			convertedT = n1 * t_ * t_;
		}

		else if (t_ < 2.0f / d1)
		{
			convertedT = n1 * (t_ -= 1.5f / d1 )* t_ +0.75f;
		}

		else if (t_ < 2.5f / d1)
		{
			convertedT = n1 * (t_ -= 2.25f / d1) * t_ + 0.9375f;
		}

		else
		{
			convertedT = n1 * (t_ -= 2.625f / d1) * t_ + 0.984375f;
		}

		return Lerp(st_, end_, convertedT);
	}

	template<typename T>
	static inline T EaseOutElastic(const T& st_, const T& end_, float t_)
	{
		float convertedT;
		static float const c4 = (2 * 3.14159265359f) / 3.0f;
		convertedT	= powf(2.0f, -10.0f * t_)* sinf((t_ * 10.0f - 0.75f) * c4) + 1.0f;

		return Lerp(st_, end_, convertedT);
	}

	template<typename T>
	static inline T EaseOutExpo(const T& st_, const T& end_, float t_)
	{
		float convertedT;
		convertedT = 1.0f - powf(2.0f, -10.0f * t_);

		return Lerp(st_, end_, convertedT);

	}

	template<typename T>
	static inline T EaseOutQuint(const T& st_, const T& end_, float t_)
	{
		float convertedT;
		convertedT = 1.0f - powf(1.0f - t_, 5.0f);

		return Lerp(st_, end_, convertedT);

	}


}

