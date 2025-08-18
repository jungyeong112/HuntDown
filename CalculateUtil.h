#pragma once
#include <math.h>
#include "Define.h"

namespace CalculateUtil {
    const Vector2 ZERO{ 0.f, 0.f };
    const Vector2 UP{ 0.f, -1.f };
    const Vector2 DOWN{ 0.f, 1.f };
    const Vector2 LEFT{ -1.f, 0.f };
    const Vector2 RIGHT{ 1.f, 0.f };

    inline const Vector2 Mul(const Vector2& lhs, float rhs) { return Vector2(lhs.fx * rhs, lhs.fy * rhs); }
    inline const Vector2 Div(const Vector2& lhs, float rhs) { return Vector2(lhs.fx / rhs, lhs.fy / rhs); }
    inline const Vector2 Add(const Vector2& lhs, const Vector2& rhs) { return Vector2(lhs.fx + rhs.fx, lhs.fy + rhs.fy); }
    inline const Vector2 Sub(const Vector2& lhs, const Vector2& rhs) { return Vector2(lhs.fx - rhs.fx, lhs.fy - rhs.fy); }

    inline bool Equal(const Vector2& lhs, const Vector2& rhs) {
        return (lhs.fx == rhs.fx && lhs.fy == rhs.fy);
    }

    inline const float Length(const Vector2& lhs) { return sqrtf(lhs.fx * lhs.fx + lhs.fy * lhs.fy); }
    inline const float Dot(const Vector2& lhs, const Vector2& rhs) { return lhs.fx * rhs.fx + lhs.fy * rhs.fy; }

    inline const float DegreeToRadian(float _fDegree) {
        return _fDegree / 180.f * PI;
    }

    inline const float RadianToDegree(float _fRadian) {
        return _fRadian * 180.f / PI;
    }

    inline const Vector2 Rotate(const Vector2& _stTarget, float _fDegree) {
        float fRadian = DegreeToRadian(_fDegree);
        return { cosf(fRadian) * _stTarget.fx + -(sinf(fRadian) * _stTarget.fy), sinf(fRadian) * _stTarget.fx + cosf(fRadian) * _stTarget.fy };
    }

    inline const Vector2 NormalizeReturn(const Vector2& lhs) {
        return Vector2(CalculateUtil::Div(lhs, Length(lhs)));
    }

    inline void Normalize(Vector2& lhs) {
        lhs = CalculateUtil::Div(lhs, Length(lhs));
    }
    inline float Distance(Vector2& _Dst, Vector2& _Src) { return Length(Sub(_Dst, _Src)); }

    inline Vector2 Lerp(const Vector2& lhs, const Vector2& rhs, float t)
    {
        return { lhs.fx + (rhs.fx - lhs.fx) * t, lhs.fy + (rhs.fy - lhs.fy) * t };
    }
}