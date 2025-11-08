// Jolt Physics Library (https://github.com/jrouwe/JoltPhysics)
// SPDX-FileCopyrightText: 2021 Jorrit Rouwe
// SPDX-License-Identifier: MIT

#pragma once

#ifdef JPH_ENABLE_ASSERTS

JPH_NAMESPACE_BEGIN

class JPH_EXPORT BodyAccess
{
public:
	/// Access rules, used to detect race conditions during simulation
	enum class EAccess : uint8
	{
		None		= 0,
		Read		= 1,
		ReadWrite	= 3,
	};

	/// Grant a scope specific access rights on the current thread
	class Grant
	{
	public:
		inline							Grant(EAccess inVelocity, EAccess inPosition)
		{
		}

		inline							~Grant()
		{
		}
	};

	/// Check if we have permission
	static inline bool					sCheckRights(EAccess inRights, EAccess inDesiredRights)
	{
		return true;
	}

	/// Access to read/write velocities
	static inline EAccess &				sVelocityAccess()
	{
		static  EAccess sAccess = BodyAccess::EAccess::ReadWrite;
		return sAccess;
	}

	/// Access to read/write positions
	static inline EAccess &				sPositionAccess()
	{
		static  EAccess sAccess = BodyAccess::EAccess::ReadWrite;
		return sAccess;
	}
};

JPH_NAMESPACE_END

#endif // JPH_ENABLE_ASSERTS
