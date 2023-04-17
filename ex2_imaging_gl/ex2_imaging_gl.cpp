#include <pxr/usd/usd/common.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/stageCache.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "pxgen.h"

int main(int argc, char **argv) {
  if (!glfwInit()) {
    std::cout << "Error: Call GLFW init first.\n";
    exit(-1);
  }
  GLFWwindow *window =
      glfwCreateWindow(1024, 768, "Triangle", nullptr, nullptr);
  if (!window) return -1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  std::string asset_path = ASSETS_PATH;
  std::string output_path = OUTPUT_PATH;
  // pxr::UsdStageRefPtr stage = pxr::UsdStage::Open(asset_path +
  // "/Forest_House.usdz");
  pxr::UsdStageRefPtr stage =
      pxr::UsdStage::Open(output_path + "/ex1_triangle.usda");
  if (!stage) {
    std::cout << "Error! Cannot find usda file\n";
    return -1;
  }

  pxgen::UsdOrbitRenderer renderer;
  renderer.set_orbit_control(Eigen::Vector3d(0.0, 0.0, 15.0),
                             Eigen::Vector3d(0.5, 0.5, 0.0));
  renderer.set_camera_distance(100.0);
  renderer.AddLight(Eigen::Vector3f(1.0f, 1.0f, 1.0f),
                    Eigen::Vector3f(1.0f, 1.0f, 1.0f),
                    Eigen::Vector3f(0.0f, 0.0f, 1.0f));

  double prev = glfwGetTime();
  double prevx, prevy;
  double deltax, deltay;
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    double delta = glfwGetTime() - prev;
    prev = glfwGetTime();
    glfwGetCursorPos(window, &deltax, &deltay);
    deltax -= prevx;
    deltay -= prevy;
    glfwGetCursorPos(window, &prevx, &prevy);

    double rot_hori = 0.0;
    double rot_vert = 0.0;
    double move_forward = 0.0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) move_forward = 1.0;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) move_forward = -1.0;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
      rot_hori = deltax;
      rot_vert = deltay;
    }

    renderer.TakeInput(rot_hori, rot_vert, move_forward * delta);
    renderer.Update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.12, 0.04, 1.0);
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    renderer.Render(stage->GetPseudoRoot(), w, h);

    glfwSwapBuffers(window);
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}