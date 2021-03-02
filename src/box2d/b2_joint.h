// MIT License

// Copyright (c) 2019 Erin Catto

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef B2_JOINT_H
#define B2_JOINT_H

#include "b2_math.h"
#include <string>
#define NOTSUPPORT {throw std::string("attempt to call a nil value");}

class b2Body;
class b2Draw;
class b2Joint;
struct b2SolverData;
class b2BlockAllocator;

struct b2JointData {
	std::string id;
};
enum b2JointType {
	e_unknownJoint,
	e_revoluteJoint,
	e_prismaticJoint,
	e_distanceJoint,
	e_pulleyJoint,
	e_mouseJoint,
	e_gearJoint,
	e_wheelJoint,
	e_weldJoint,
	e_frictionJoint,
	e_ropeJoint,
	e_motorJoint
};

struct b2Jacobian {
	b2Vec2 linear;
	float angularA;
	float angularB;
};

/// A joint edge is used to connect bodies and joints together
/// in a joint graph where each body is a node and each joint
/// is an edge. A joint edge belongs to a doubly linked list
/// maintained in each attached body. Each joint has two joint
/// nodes, one for each attached body.
struct b2JointEdge {
	b2Body* other;			///< provides quick access to the other body attached.
	b2Joint* joint;			///< the joint
	b2JointEdge* prev;		///< the previous joint edge in the body's joint list
	b2JointEdge* next;		///< the next joint edge in the body's joint list
};

/// Joint definitions are used to construct joints.
struct b2JointDef {
	b2JointDef() {
		type = e_unknownJoint;
		userData = nullptr;
		bodyA = nullptr;
		bodyB = nullptr;
		collideConnected = false;
	}

	/// The joint type is set automatically for concrete joint types.
	b2JointType type;

	/// Use this to attach application specific data to your joints.
	b2JointData* userData;

	/// The first attached body.
	b2Body* bodyA;

	/// The second attached body.
	b2Body* bodyB;

	/// Set this flag to true if the attached bodies should collide.
	bool collideConnected;
};

/// Utility to compute linear stiffness values from frequency and damping ratio
void b2LinearStiffness(float& stiffness, float& damping,
					   float frequencyHertz, float dampingRatio,
					   const b2Body* bodyA, const b2Body* bodyB);

/// Utility to compute rotational stiffness values frequency and damping ratio
void b2AngularStiffness(float& stiffness, float& damping,
						float frequencyHertz, float dampingRatio,
						const b2Body* bodyA, const b2Body* bodyB);

/// The base joint class. Joints are used to constraint two bodies together in
/// various fashions. Some joints also feature limits and motors.
class b2Joint {
public:

	/// Get the type of the concrete joint.
	b2JointType GetType() const;

	/// Get the first body attached to this joint.
	b2Body* GetBodyA();

	/// Get the second body attached to this joint.
	b2Body* GetBodyB();

	/// Get the anchor point on bodyA in world coordinates.
	virtual b2Vec2 GetAnchorA() const = 0;
	/// Get the anchor point on bodyB in world coordinates.
	virtual b2Vec2 GetAnchorB() const = 0;
	/// Get the reaction force on bodyB at the joint anchor in Newtons.
	virtual b2Vec2 GetReactionForce(float inv_dt) const = 0;
	/// Get the reaction torque on bodyB in N*m.
	virtual float GetReactionTorque(float inv_dt) const = 0;

	inline float GetReactionForceX(float inv_dt) {
		return GetReactionForce(inv_dt).x;
	}
	inline float GetReactionForceY(float inv_dt) {
		return GetReactionForce(inv_dt).y;
	}

	/// Get the next joint the world joint list.
	b2Joint* GetNext();
	const b2Joint* GetNext() const;

	/// Get the user data pointer.
	b2JointData* GetUserData() const;

	/// Set the user data pointer.
	void SetUserData(b2JointData* data);

	/// Short-cut function to determine if either body is enabled.
	int IsEnabled() const;

	/// Get collide connected.
	/// Note: modifying the collide connect flag won't work correctly because
	/// the flag is only checked when fixture AABBs begin to overlap.
	int GetCollideConnected() const;

	/// Dump this joint to the log file.
	virtual void Dump() {
		b2Dump("// Dump is not supported for this joint type.\n");
	}

	/// Shift the origin for any points stored in world coordinates.
	virtual void ShiftOrigin(const b2Vec2& newOrigin) {
		B2_NOT_USED(newOrigin);
	}
	/// Debug draw this joint
	virtual void Draw(b2Draw* draw) const;

	//////////////////////////////////
	virtual void SetMotorSpeed(float speed) NOTSUPPORT;
	virtual float GetMotorSpeed() const NOTSUPPORT;
	virtual void EnableMotor(int flag) NOTSUPPORT;
	virtual int IsMotorEnabled() const NOTSUPPORT;
	virtual void EnableLimit(int flag) NOTSUPPORT;
	virtual int IsLimitEnabled() const NOTSUPPORT;
	virtual float GetLowerLimit() const NOTSUPPORT;
	virtual float GetUpperLimit() const NOTSUPPORT;
	virtual void SetUpperLimit(float val) NOTSUPPORT;
	virtual void SetLowerLimit(float val) NOTSUPPORT;
	virtual float GetJointAngle() const NOTSUPPORT;
	virtual float GetJointSpeed() const NOTSUPPORT;
	virtual void SetMaxMotorTorque(float torque) NOTSUPPORT;
	virtual float GetMaxMotorTorque() const NOTSUPPORT;
	virtual float GetReferenceAngle() const NOTSUPPORT;
	virtual void SetStiffness(float hz) NOTSUPPORT;
	virtual float GetStiffness() const NOTSUPPORT;
	virtual void SetDamping(float damping) NOTSUPPORT;
	virtual float GetDamping() const NOTSUPPORT;
	virtual void SetRatio(float ratio) NOTSUPPORT;
	virtual float GetRatio() const NOTSUPPORT;
	virtual float GetCurrentLength() const NOTSUPPORT;
	virtual void SetMinLength(float l) NOTSUPPORT;
	virtual float GetMinLength() const NOTSUPPORT;
	virtual void SetMaxLength(float l) NOTSUPPORT;
	virtual float GetMaxLength() const NOTSUPPORT;
	virtual float GetCurrentLengthA() const NOTSUPPORT;
	virtual float GetCurrentLengthB() const NOTSUPPORT;
	virtual void SetMaxMotorForce(float force) NOTSUPPORT;
	virtual float GetMaxMotorForce() const NOTSUPPORT;
	virtual float GetMotorForce(float inv_dt) const NOTSUPPORT;
	//////////////////////////////////

	b2Body* m_bodyA;
	b2Body* m_bodyB;

	std::string &GetID() const;

	void SetID(std::string id);

	friend class b2World;
	friend class b2Body;
	friend class b2Island;
	friend class b2GearJoint;

	static b2Joint* Create(const b2JointDef* def, b2BlockAllocator* allocator);
	static void Destroy(b2Joint* joint, b2BlockAllocator* allocator);

	b2Joint(const b2JointDef* def);
	virtual ~b2Joint() {}

	virtual void InitVelocityConstraints(const b2SolverData& data) = 0;
	virtual void SolveVelocityConstraints(const b2SolverData& data) = 0;

	// This returns true if the position errors are within tolerance.
	virtual bool SolvePositionConstraints(const b2SolverData& data) = 0;

	b2JointType m_type;
	b2Joint* m_prev;
	b2Joint* m_next;
	b2JointEdge m_edgeA;
	b2JointEdge m_edgeB;

	int32 m_index;

	bool m_islandFlag;
	bool m_collideConnected;

	b2JointData* m_userData;
};

inline b2JointType b2Joint::GetType() const {
	return m_type;
}

inline b2Body* b2Joint::GetBodyA() {
	return m_bodyA;
}

inline b2Body* b2Joint::GetBodyB() {
	return m_bodyB;
}

inline b2Joint* b2Joint::GetNext() {
	return m_next;
}

inline const b2Joint* b2Joint::GetNext() const {
	return m_next;
}

inline b2JointData* b2Joint::GetUserData() const {
	return m_userData;
}

inline void b2Joint::SetUserData(b2JointData* data) {
	m_userData = data;
}

inline int b2Joint::GetCollideConnected() const {
	return m_collideConnected;
}
inline std::string &b2Joint::GetID() const {
	return GetUserData()->id;
}

inline void b2Joint::SetID(std::string id) {
	GetUserData()->id=id;
}


#endif
