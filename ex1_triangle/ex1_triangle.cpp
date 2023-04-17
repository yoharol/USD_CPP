#include <Eigen/Core>

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdGeom/mesh.h>
#include <pxr/usd/usdGeom/metrics.h>

#include "pxgen.h"

int main() {
  std::string output_path = OUTPUT_PATH;
  auto stage = pxr::UsdStage::CreateNew(output_path + "/ex1_triangle.usda");
  pxr::UsdGeomSetStageUpAxis(stage, pxr::TfToken("Y"));
  pxr::UsdGeomXform root = pxr::UsdGeomXform::Define(stage, pxr::SdfPath("/root"));
  pxr::UsdGeomMesh mesh =
	  pxr::UsdGeomMesh::Define(stage, pxr::SdfPath("/root/mesh"));
  pxr::UsdAttribute mesh_points_attr = mesh.GetPointsAttr();

  // Set face vertex counts as [3]
  mesh.GetFaceVertexCountsAttr().Set(pxgen::VectorX(1, 3));

  // Set face vertex indices as [0, 1, 2]
  // pxr::VtArray<int> face_indices(3, 0);
  Eigen::VectorXi face_indices(3);
  face_indices << 0, 1, 2;
  mesh.GetFaceVertexIndicesAttr().Set(pxgen::VectorX(face_indices));

  // Set positions of vertices
  Eigen::MatrixXf v_p(3, 3);
  v_p << 0.0, 0.0, 0.0,
  		1.0, 0.0, 0.0,
		  0.0, 1.0, 0.0;
  mesh_points_attr.Set(pxgen::MatrixX<pxr::GfVec3f>(v_p));

  mesh.GetDoubleSidedAttr().Set(true);

  stage->Save();
}
