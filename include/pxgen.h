#ifndef USD_CPP_CMAKE_INCLUDE_PXGEN_H_
#define USD_CPP_CMAKE_INCLUDE_PXGEN_H_

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <pxr/base/vt/value.h>
#include <pxr/usdImaging/usdImagingGL/engine.h>
#include <pxr/usdImaging/usdImagingGL/renderParams.h>

#include <iostream>
#include <memory>
#include <vector>
#include <pxr/base/gf/camera.h>

namespace pxgen {

/*
 * Generate a VtArray from Eigen::VectorXT array
 * */
template<typename T>
pxr::VtValue VectorX(Eigen::Matrix<T, Eigen::Dynamic, 1> eigen_arr) {
  pxr::VtArray<T> vt_arr(eigen_arr.rows());
  for (int i = 0; i < eigen_arr.rows(); i++)
	vt_arr[i] = eigen_arr(i);
  return pxr::VtValue(vt_arr);
}

/*
 * Generate a VtArray with size and value
 * */
template<typename T>
pxr::VtValue VectorX(int size, T value) {
  return pxr::VtValue(pxr::VtArray<T>(size, value));
}

/*
 * Generate a PxrType array from Eigen::MatrixXT
 * */
template<typename PxrType, typename T>
pxr::VtValue MatrixX(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> eigen_mat) {
  pxr::VtArray<PxrType> pxr_arr(eigen_mat.rows());
  assert(PxrType::dimension == eigen_mat.cols());
  for (int i = 0; i < eigen_mat.rows(); i++)
	for (int j = 0; j < eigen_mat.cols(); j++)
	  pxr_arr[i][j] = eigen_mat(i, j);
  return pxr::VtValue(pxr_arr);
}

class CameraOrbit {
 public:
  CameraOrbit(const Eigen::Vector3d& pos,
			  const Eigen::Vector3d& target,
			  double vel_horizontal,
			  double vel_vertical,
			  double vel_motion): _pos(pos), _target(target) {
	_velocity << vel_horizontal, vel_vertical, vel_motion;
	_t.SetLookAt(pxr::GfVec3d(_pos.data()), pxr::GfVec3d(_target.data()), pxr::GfVec3d::YAxis());
  }

  void TakeInput(double hori, double vert, double forward) {
	Eigen::Vector3d direc = _pos - _target;
	double length = direc.norm();
	direc.normalize();
	Eigen::Vector3d vert_axis;
	vert_axis << -direc(2), 0.0, direc(0);
	vert_axis.normalize();

	Eigen::Matrix3d rot = (Eigen::AngleAxis(hori * _velocity[0], Eigen::Vector3d::UnitY())
		* Eigen::AngleAxis(vert * _velocity[1], vert_axis)).toRotationMatrix();
	length = length - forward * _velocity(2);
	_pos = _target + length * rot * direc;
	_t.SetLookAt(pxr::GfVec3d(_pos.data()), pxr::GfVec3d(_target.data()), pxr::GfVec3d::YAxis());
	std::cout << _pos << std::endl;
  }

  pxr::GfMatrix4d get_transform() {
	return _t.GetInverse();
  }

   private:

  Eigen::Vector3d _pos;
  Eigen::Vector3d _target;
  Eigen::Vector3d _velocity;
  pxr::GfMatrix4d _t;

};

class UsdOrbitRenderer {
 public:
  UsdOrbitRenderer(){
	_light_vec.resize(0);
	_material.SetAmbient(pxr::GfVec4f(0.0, 0.0, 0.0, 1.0));
	_params.enableLighting = true;
	_control = nullptr;

  }
  ~UsdOrbitRenderer() = default;

  void set_orbit_control(const Eigen::Vector3d& pos,
						 const Eigen::Vector3d& target,
						 double hori_velocity = 1.0,
						 double vert_velocity = 1.0,
						 double forward_velocity = 1.0) {
	_control = std::make_unique<CameraOrbit>(pos,
											 target,
											 hori_velocity * 0.006,
											 vert_velocity * 0.006,
											 forward_velocity * 0.5 * (target - pos).norm());
	_material.SetAmbient(pxr::GfVec4f(0.1, 0.1, 0.1, 1.0));

  }

  void TakeInput(double hori_motion, double vert_motion, double forward_motion){
	_control->TakeInput(hori_motion, vert_motion, forward_motion);
  }


  void set_camera_distance(float dis) {
	_camera.SetFocusDistance(dis);
  }

  void AddLight(Eigen::Vector3f pos, Eigen::Vector3f diffuse, Eigen::Vector3f specular) {
	pxr::GlfSimpleLight light(pxr::GfVec4f(pos(0), pos(1), pos(2), 1.0f));
	// light.SetAmbient(pxr::GfVec4f(1.0, 1.0, 1.0, 1.0));
	light.SetDiffuse(pxr::GfVec4f(diffuse(0), diffuse(1), diffuse(2), 1.0f));
	light.SetSpecular(pxr::GfVec4f(specular(0), specular(1), specular(2), 1.0f));
	light.SetHasShadow(true);
	_light_vec.push_back(light);
  }

  void Update(){
	_camera.SetTransform(_control->get_transform());
	_engine.SetCameraState(_camera.GetFrustum().ComputeViewMatrix(),
						  _camera.GetFrustum().ComputeProjectionMatrix());
  }

  void Render(const pxr::UsdPrim& prim, int w, int h){
	_engine.SetRenderViewport(pxr::GfVec4d(0, 0, w, h));
	_engine.SetLightingState(_light_vec, _material, pxr::GfVec4f(1.0f, 1.0f, 1.0f, 1.0f));
	_engine.Render(prim, _params);

  }

  pxr::GfCamera _camera;

 private:
  std::vector<pxr::GlfSimpleLight> _light_vec;
  pxr::GlfSimpleMaterial _material;
  std::unique_ptr<CameraOrbit> _control;
  pxr::UsdImagingGLEngine _engine;
  pxr::UsdImagingGLRenderParams _params;

};

}

#endif //USD_CPP_CMAKE_INCLUDE_PXGEN_H_
