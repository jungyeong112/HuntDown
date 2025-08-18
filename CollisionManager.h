#pragma once
#include "pch.h"
#include "Define.h"
#include "Cobj.h"

class CCollisionManager
{
private:
	CCollisionManager() {}
	~CCollisionManager() {}
public:
	CCollisionManager(const CCollisionManager&) = delete;
	CCollisionManager& operator=(const CCollisionManager&) = delete;

public:
	static CCollisionManager& Instance() {
		static CCollisionManager instance;
		return instance;
	}

	void Initialize() {
		Clear();
	}

	void SetObjList(list<CObj*>* _pArrlist) {
		m_pObjList = _pArrlist;
	}

	void ActiveCollision(OBJID _id1, OBJID _id2) {
		arrCollisionMask[_id1][_id2] = true;
		arrCollisionMask[_id2][_id1] = true; // 걍 LateUpdate 상에서 두번안되도록 i<=j만 체크합니다,
	}

	void LateUpdate() {
		if (!m_pObjList) return;
		for (int i = 0; i < OBJ_END; i++)
			for (int j = i; j < OBJ_END; j++) {
				if (!arrCollisionMask[i][j]) continue;
				/*if ((OBJID)j == LINE)
				{
					CheckLineCollision((OBJID)i, (OBJID)j);
				}*/
				CheckCollision((OBJID)i, (OBJID)j);
			}
	}

	void CheckCollision(OBJID _id1, OBJID _id2)
	{
		auto list1 = m_pObjList[_id1];
		auto list2 = m_pObjList[_id2];
		for (auto& Src : list1)
			for (auto& Dst : list2) {
				if (Dst == Src) continue;
				//if (!Dst->IsActive() || !Src->IsActive()) continue;
				if (IsIntersect(Dst->Get_Rect(), Src->Get_Rect())) 
				{
					Src->OnCollision(GetCollision(Src, Dst, _id2));
					Dst->OnCollision(GetCollision(Dst, Src, _id1));
				}
			}
	}

	FCollision GetCollision(CObj* _pSrcObj, CObj* _pDstObj, OBJID _Dstid)
	{
		FCollision _Collision;

		auto _DstInfo = _pDstObj->Get_Info();
		auto _SrcInfo = _pSrcObj->Get_Info();
		_Collision.m_OBJID = _Dstid;
		_Collision.m_pObject = _pDstObj;
		_Collision.m_fX = (_DstInfo.fCX +_SrcInfo.fCX) * 0.5 - abs (_DstInfo.fX - _SrcInfo.fX);
		_Collision.m_fY = (_DstInfo.fCY  + _SrcInfo.fCY) * 0.5 - abs (_DstInfo.fY - _SrcInfo.fY);
		_Collision.m_fX = abs(_Collision.m_fX);
		_Collision.m_fY = abs(_Collision.m_fY);
		if (_Collision.m_fX < _Collision.m_fY)
			_Collision.m_Collisiontype = (_DstInfo.fX < _SrcInfo.fX) ? CF_Left : CF_Right;
		else
			_Collision.m_Collisiontype = (_DstInfo.fY < _SrcInfo.fY) ? CF_Top : CF_Bottom;

		return _Collision;
	}
	void Clear() {
		for (int i = 0; i < OBJ_END; i++)
			for (int j = 0; j < OBJ_END; j++)
				arrCollisionMask[i][j] = false;
	}

private:
	list<CObj*>* m_pObjList;
	bool arrCollisionMask[OBJ_END][OBJ_END];
};

