#pragma once
#include "Vector3f.h"
#include "Matrix4f.h"

namespace forward
{
	class Camera
	{
	public:
		Camera();
		~Camera() {}

		void update(f32 delta);
		void updateWithKeyboardInput(i8 key);
		void updateWithMouseInput(i32 x, i32 y);

		Matrix4f updateViewMatrix();
		void setViewMatrix(const Matrix4f& mat4);

		void pitch(f32 radians);
		void yaw(f32 radians);

		void setProjectionParams(f32 fov, f32 aspect, f32 nearPlane, f32 farPlane);
		void setOrthographicParams(f32 near, f32 far, f32 width, f32 height);
		const Matrix4f& getProjectionMatrix() const;
		const Matrix4f& getViewMatrix() const;

		f32 getFOV() const { return m_fov; }
		f32 getAspectRatio() const { return m_aspect; }
		f32 getNearClip() const { return m_near; }
		f32 getFarClip() const { return m_far; }
		const Vector3f& getWorldEyePos() const { return m_pos; }
		Vector3f getWorldRight() const { return m_rot.GetRow(0); }
		Vector3f getWorldUp() const { return m_rot.GetRow(1); }
		Vector3f getWorldLookingDir() const { return m_rot.GetRow(2); }

	protected:
		Vector3f m_pos;
		Matrix3f m_rot;

		f32 m_frameTime;

		f32 m_fov;
		f32 m_near, m_far;
		f32 m_aspect;
		f32 m_width;
		f32 m_height;
		Matrix4f m_projMat;
		Matrix4f m_viewMat;
	};
}