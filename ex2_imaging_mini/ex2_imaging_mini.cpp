#include <pxr/usd/usd/common.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/stageCache.h>
#include <pxr/usdImaging/usdImagingGL/engine.h>
#include <pxr/base/gf/camera.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

int main() {
  if (!glfwInit()) {
    std::cout << "Error: Call GLFW init first.\n";
    exit(-1);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  GLFWwindow *window =
      glfwCreateWindow(1024, 768, "Triangle", nullptr, nullptr);
  if (!window) return -1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  std::string asset_path = ASSETS_PATH;
  std::string output_path = OUTPUT_PATH;
  pxr::UsdStageRefPtr stage =
      pxr::UsdStage::Open(output_path + "/ex1_triangle.usda");
  if (!stage) {
    std::cout << "Error! Cannot find usd file\nMake sure you have ran "
                 "ex1_triangle before this example.\n";
    return -1;
  }

  std::vector<pxr::GlfSimpleLight> light_vec;
  pxr::GlfSimpleMaterial material;
  pxr::UsdImagingGLEngine engine;
  pxr::UsdImagingGLRenderParams params;
  pxr::GfCamera camera;

  // Set simple lighting
  pxr::GlfSimpleLight light(pxr::GfVec4f(1.0f, 1.0f, 1.0f, 1.0f));
  light.SetDiffuse(pxr::GfVec4f(1.0f, 1.0f, 1.0f, 1.0f));
  light.SetSpecular(pxr::GfVec4f(0.0f, 0.0f, 1.0f, 1.0f));
  light.SetHasShadow(true);
  light_vec.push_back(light);
  params.enableLighting = true;

  // Set simple material
  material.SetAmbient(pxr::GfVec4f(0.1, 0.1, 0.1, 1.0));

  // Set camera transformation
  pxr::GfMatrix4d trans;
  pxr::GfVec3d pos(0.5, 0.5, 15.0);
  pxr::GfVec3d target(0.5, 0.5, 0.0);
  trans.SetLookAt(pos, target, pxr::GfVec3d::YAxis());

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    camera.SetTransform(trans.GetInverse());
    engine.SetCameraState(camera.GetFrustum().ComputeViewMatrix(),
                          camera.GetFrustum().ComputeProjectionMatrix());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.12, 0.04, 1.0);
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);

    engine.SetRenderViewport(pxr::GfVec4d(0, 0, w, h));
    engine.SetLightingState(light_vec, material,
                            pxr::GfVec4f(1.0f, 1.0f, 1.0f, 1.0f));
    engine.Render(stage->GetPseudoRoot(), params);

    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
