// Jolt Physics Library (https://github.com/jrouwe/JoltPhysics)
// SPDX-FileCopyrightText: 2021 Jorrit Rouwe
// SPDX-License-Identifier: MIT

#pragma once

#include <Jolt/Core/Mutex.h>

JPH_NAMESPACE_BEGIN

#ifdef JPH_ENABLE_ASSERTS

/// This is the list of locks used by the physics engine, they need to be locked in a particular order (from top of the list to bottom of the list) in order to prevent deadlocks
enum class EPhysicsLockTypes
{
	BroadPhaseQuery			= 1 << 0,
	PerBody					= 1 << 1,
	BodiesList				= 1 << 2,
	BroadPhaseUpdate		= 1 << 3,
	ConstraintsList			= 1 << 4,
	ActiveBodiesList		= 1 << 5,
};

/// A token that indicates the context of a lock (we use 1 per physics system and we use the body manager pointer because it's convenient)
class BodyManager;
using PhysicsLockContext = const BodyManager *;

#endif // !JPH_ENABLE_ASSERTS

/// Helpers to safely lock the different mutexes that are part of the physics system while preventing deadlock
/// Class that keeps track per thread which lock are taken and if the order of locking is correct
class JPH_EXPORT PhysicsLock
{
public:

	template <class LockType>
	static inline void			sLock(LockType &inMutex JPH_IF_ENABLE_ASSERTS(, PhysicsLockContext inContext, EPhysicsLockTypes inType))
	{
		inMutex.lock();
	}

	template <class LockType>
	static inline void			sUnlock(LockType &inMutex JPH_IF_ENABLE_ASSERTS(, PhysicsLockContext inContext, EPhysicsLockTypes inType))
	{
		inMutex.unlock();
	}

	template <class LockType>
	static inline void			sLockShared(LockType &inMutex JPH_IF_ENABLE_ASSERTS(, PhysicsLockContext inContext, EPhysicsLockTypes inType))
	{
		inMutex.lock_shared();
	}

	template <class LockType>
	static inline void			sUnlockShared(LockType &inMutex JPH_IF_ENABLE_ASSERTS(, PhysicsLockContext inContext, EPhysicsLockTypes inType))
	{
		inMutex.unlock_shared();
	}
};

/// Helper class that is similar to std::unique_lock
template <class LockType>
class UniqueLock : public NonCopyable
{
public:
	explicit					UniqueLock(LockType &inLock JPH_IF_ENABLE_ASSERTS(, PhysicsLockContext inContext, EPhysicsLockTypes inType)) :
		mLock(inLock)
#ifdef JPH_ENABLE_ASSERTS
		, mContext(inContext),
		mType(inType)
#endif // JPH_ENABLE_ASSERTS
	{
		PhysicsLock::sLock(mLock JPH_IF_ENABLE_ASSERTS(, mContext, mType));
	}

								~UniqueLock()
	{
		PhysicsLock::sUnlock(mLock JPH_IF_ENABLE_ASSERTS(, mContext, mType));
	}

private:
	LockType &					mLock;
#ifdef JPH_ENABLE_ASSERTS
	PhysicsLockContext			mContext;
	EPhysicsLockTypes			mType;
#endif // JPH_ENABLE_ASSERTS
};

/// Helper class that is similar to std::shared_lock
template <class LockType>
class SharedLock : public NonCopyable
{
public:
	explicit					SharedLock(LockType &inLock JPH_IF_ENABLE_ASSERTS(, PhysicsLockContext inContext, EPhysicsLockTypes inType)) :
		mLock(inLock)
#ifdef JPH_ENABLE_ASSERTS
		, mContext(inContext)
		, mType(inType)
#endif // JPH_ENABLE_ASSERTS
	{
		PhysicsLock::sLockShared(mLock JPH_IF_ENABLE_ASSERTS(, mContext, mType));
	}

								~SharedLock()
	{
		PhysicsLock::sUnlockShared(mLock JPH_IF_ENABLE_ASSERTS(, mContext, mType));
	}

private:
	LockType &					mLock;
#ifdef JPH_ENABLE_ASSERTS
	PhysicsLockContext			mContext;
	EPhysicsLockTypes			mType;
#endif // JPH_ENABLE_ASSERTS
};

JPH_NAMESPACE_END
