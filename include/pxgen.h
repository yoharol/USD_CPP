#ifndef USD_CPP_CMAKE_INCLUDE_PXGEN_H_
#define USD_CPP_CMAKE_INCLUDE_PXGEN_H_

#include <Eigen/Core>
#include <pxr/base/vt/value.h>
#include <iostream>

namespace Pxgen {

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
pxr::VtValue Vectorx(int size, T value){
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

}

#endif //USD_CPP_CMAKE_INCLUDE_PXGEN_H_
