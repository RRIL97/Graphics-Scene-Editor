// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2018 Jérémie Dumas <jeremie.dumas@ens-lyon.org>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
////////////////////////////////////////////////////////////////////////////////
//#include "ImGuiMenu.h"
//#include "ImGuiHelpers.h"
#include <igl/project.h>
#include "ImGuiHelpers.h"
#include "igl/opengl/glfw/renderer.h"

#include "ImGuiMenu.h"

#include "../imgui.h"
#include "igl/opengl/glfw/imgui/imgui_impl_glfw.h"
#include "igl/opengl/glfw/imgui/imgui_impl_opengl3.h"

//#include <imgui_fonts_droid_sans.h>
//#include <GLFW/glfw3.h>
#include <time.h>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

namespace igl
{
namespace opengl
{
namespace glfw
{
namespace imgui
{

IGL_INLINE void ImGuiMenu::init(Display* disp)
{
  // Setup ImGui binding
  if (disp->window)
  {
    IMGUI_CHECKVERSION();
    if (!context_)
    {
      // Single global context by default, but can be overridden by the user
      static ImGuiContext * __global_context = ImGui::CreateContext();
      context_ = __global_context;
    }
    const char* glsl_version = "#version 150";

    ImGui_ImplGlfw_InitForOpenGL(disp->window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::GetIO().IniFilename = nullptr;
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 5.0f;
    reload_font();
  }
}

IGL_INLINE void ImGuiMenu::reload_font(int font_size)
{
  hidpi_scaling_ = hidpi_scaling();
  pixel_ratio_ = pixel_ratio();
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->Clear();
 // io.Fonts->AddFontFromMemoryCompressedTTF(droid_sans_compressed_data,
 //   droid_sans_compressed_size, font_size * hidpi_scaling_);
  io.FontGlobalScale = 1.0 / pixel_ratio_;
}

IGL_INLINE void ImGuiMenu::shutdown()
{
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  // User is responsible for destroying context if a custom context is given
  // ImGui::DestroyContext(*context_);
}

IGL_INLINE bool ImGuiMenu::pre_draw()
{
  glfwPollEvents();

  // Check whether window dpi has changed
  float scaling = hidpi_scaling();
  if (std::abs(scaling - hidpi_scaling_) > 1e-5)
  {
    reload_font();
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
  }

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  return false;
}

IGL_INLINE bool ImGuiMenu::post_draw()
{
  //draw_menu(viewer,core);
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  return false;
}

IGL_INLINE void ImGuiMenu::post_resize(int width, int height)
{
  if (context_)
  {
    ImGui::GetIO().DisplaySize.x = float(width);
    ImGui::GetIO().DisplaySize.y = float(height);
  }
}

// Mouse IO
IGL_INLINE bool ImGuiMenu::mouse_down(GLFWwindow* window, int button, int modifier)
{
  ImGui_ImplGlfw_MouseButtonCallback(window, button, GLFW_PRESS, modifier);
  return ImGui::GetIO().WantCaptureMouse;
}

IGL_INLINE bool ImGuiMenu::mouse_up(GLFWwindow* window,int button, int modifier)
{
  //return ImGui::GetIO().WantCaptureMouse;
  // !! Should not steal mouse up
  return false;
}

IGL_INLINE bool ImGuiMenu::mouse_move(GLFWwindow* window,int mouse_x, int mouse_y)
{
  return ImGui::GetIO().WantCaptureMouse;
}

IGL_INLINE bool ImGuiMenu::mouse_scroll(GLFWwindow* window,float delta_y)
{
  ImGui_ImplGlfw_ScrollCallback(window, 0.f, delta_y);
  return ImGui::GetIO().WantCaptureMouse;
}

// Keyboard IO
IGL_INLINE bool ImGuiMenu::key_pressed(GLFWwindow* window,unsigned int key, int modifiers)
{
  ImGui_ImplGlfw_CharCallback(nullptr, key);
  return ImGui::GetIO().WantCaptureKeyboard;
}

IGL_INLINE bool ImGuiMenu::key_down(GLFWwindow* window, int key, int modifiers)
{
  ImGui_ImplGlfw_KeyCallback(window, key, 0, GLFW_PRESS, modifiers);
  return ImGui::GetIO().WantCaptureKeyboard;
}

IGL_INLINE bool ImGuiMenu::key_up(GLFWwindow* window,int key, int modifiers)
{
  ImGui_ImplGlfw_KeyCallback(window, key, 0, GLFW_RELEASE, modifiers);
  return ImGui::GetIO().WantCaptureKeyboard;
}



IGL_INLINE void ImGuiMenu::draw_viewer_menu(igl::opengl::glfw::Viewer* viewer, std::vector<igl::opengl::Camera*>& camera, Eigen::Vector4i& viewWindow, std::vector<DrawInfo*> drawInfos)
{
    int current_selected_radio;
    bool* p_open = NULL;
    static bool no_titlebar = false;
    static bool no_scrollbar = false;
    static bool no_menu = true;
    static bool no_move = false;
    static bool no_resize = false;
    static bool no_collapse = false;
    static bool no_close = false;
    static bool no_nav = false;
    static bool no_background = false;
    static bool no_bring_to_front = false;
    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
    if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
    if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
    if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
    if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::Begin(
        "Viewer", p_open,
        window_flags
    );
    ImGui::SetNextWindowPos(ImVec2((float)0, (float)0), ImGuiCond_Always);
    ImGui::SetWindowSize(ImVec2((float)0, (float)0), ImGuiCond_Always);
    ImGui::End();
    no_move = true;
    no_resize = true;
    ImGui::Begin(
        "Viewer", p_open,
        window_flags
    );
    float w = ImGui::GetContentRegionAvailWidth();
    float p = ImGui::GetStyle().FramePadding.x;

    if (ImGui::CollapsingHeader("Animation Options", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::PushItemWidth(80 * menu_scaling());
        ImGui::Checkbox("blur motion", &viewer->blurMotionWhenMoving);
        if (ImGui::Button("set curve", ImVec2((w - p) / 2.0f, 0)))
        {
            viewer->setCurve();
        }
        ImGui::SameLine(0, p);
        if (ImGui::Button("remove curve", ImVec2((w - p) / 2.0f, 0)))
        {
            viewer->removeCurve();
        }
        if (ImGui::Button("set curve camera", ImVec2((w - p) / 2.0f, 0)))
        {
            viewer->setCameraCurve();
        }
        ImGui::SameLine(0, p);
        if (ImGui::Button("remove curve camera", ImVec2((w - p) / 2.0f, 0)))
        {
            viewer->removeCameraCurve();
        }
        ImGui::SliderInt("Animation Start Delay", &viewer->animationDelay, 0, 5);
        std::string startButtonMsg;
        if (pauseAnimation) {
            startButtonMsg = "Continue Animation";
        }
        else {
            startButtonMsg = "Start Animation";

        }
        if (ImGui::Button(startButtonMsg.c_str(), ImVec2((w - p), 0)))
        {
            viewer->startDrawBezierCurve = true;
            viewer->stopAnimation = false;
            viewer->playAnimationMiliTime = time(NULL);
            viewer->data_list[11]->clear();
        }
        if (!viewer->stopAnimation) {
            if (ImGui::Button("pause", ImVec2((w - p) / 2.0f, 0)))
            {
                viewer->stopAnimation = true;
                viewer->pauseAnimation = true;
                pauseAnimation = true;
            }
            ImGui::SameLine(0, p);
            if (ImGui::Button("Stop", ImVec2((w - p) / 2.0f, 0)))
            {
                viewer->stopAnimation = true;
                viewer->pauseAnimation = false;
                pauseAnimation = false;

            }
        }
        ImGui::PopItemWidth();

    }
    // Mesh
    if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
    {

        if (ImGui::Button("Load##Mesh", ImVec2((w - p) / 2.f, 0)))
        {
            int savedIndx = viewer->selected_data_index;
            // viewer->selected_data_index = viewer->parents.size();
            // viewer->AddShape(viewer->xCylinder,-1,viewer->TRIANGLES);
            viewer->open_dialog_load_mesh();
            if (viewer->data_list.size() > viewer->parents.size())
            {
                std::cout <<"viewports "<< viewer->data_list[viewer->selected_data_index]->viewports << std::endl;
                viewer->parents.push_back(-1);
                viewer->data_list[viewer->selected_data_index]->viewports = 1;
                viewer->SetShapeShader(viewer->selected_data_index, 1);
                viewer->SetShapeMaterial(viewer->selected_data_index, 4);
                viewer->data_list.back()->show_overlay = 0;
                viewer->data_list.back()->layer = 1;
                viewer->data_list.back()->show_lines = 0;
                viewer->data_list.back()->UnHide();
             //   viewer->data_list.back()->show_faces = 3;
                viewer->data()->mode = viewer->TRIANGLES;
                viewer->selected_data_index = savedIndx;
            }
        }
        ImGui::SameLine(0, p);
        if (ImGui::Button("Save##Mesh", ImVec2((w - p) / 2.f, 0)))
        {
            viewer->open_dialog_save_mesh();
        }
    }

    if (ImGui::CollapsingHeader("Theme", ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (int i = 0; i < viewer->ThemeNames.size(); i++)
            if (ImGui::RadioButton(viewer->ThemeNames[i].c_str(), &current_selected_radio, i))
            {
                if (current_selected_radio != prevSelectedTheme) {
                    prevSelectedTheme = current_selected_radio;
                    viewer->SetShapeMaterial(0, prevSelectedTheme);
                }
            }
    }
    if (ImGui::CollapsingHeader("fog Options", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Show fog", &viewer->showFog);
        ImGui::DragFloat("density", &viewer->fogDensity, 0.001f, 0.005f, 0.02f);
    }
    // Viewing options
    if (ImGui::CollapsingHeader("Viewing Options", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::Button("Center object", ImVec2(-1, 0)))
        {
            std::cout << "not implemented yet" << std::endl;
            //      core[1].align_camera_center(viewer->data().V, viewer->data().F); TODO: add function like this to camera
        }
        //if (ImGui::Button("Snap canonical view", ImVec2(-1, 0)))
        //{
        //  core[1].snap_to_canonical_quaternion();
        //}

        // Zoom
        ImGui::PushItemWidth(80 * menu_scaling());
        if (camera[0]->_ortho)
            ImGui::DragFloat("Zoom", &(camera[0]->_length), 0.05f, 0.1f, 20.0f);
        else
            ImGui::DragFloat("Fov", &(camera[0]->_fov), 0.05f, 30.0f, 90.0f);

        // Select rotation type
        static Eigen::Quaternionf trackball_angle = Eigen::Quaternionf::Identity();
        static bool orthographic = true;

        // Orthographic view
        ImGui::Checkbox("Orthographic view", &(camera[0]->_ortho));
        if (camera[0]->_ortho) {
            camera[0]->SetProjection(0, camera[0]->_relationWH);
        }
        else {
            camera[0]->SetProjection(camera[0]->_fov > 0 ? camera[0]->_fov : 45, camera[0]->_relationWH);
        }

        ImGui::PopItemWidth();
    }
    if (ImGui::CollapsingHeader("Cameras Options", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::InputText("name", cameraName, 30);
        // ImGui::SameLine(0, p);
        if (ImGui::Button("add Camera", ImVec2((w - p) / 2.0f, 0))) {
            if (std::string(cameraName).empty())
                errorMsg = "please enter camera name";

            else {
                rndr->addCameraToDesignMode(cameraName);
                errorMsg = "Successfully added camera";

            }
        }
        if (ImGui::BeginCombo("", camera[0]->name.c_str())) {
            for (int i = 0; i < camera.size(); i++) {
                if (i == 1) { //the camera for viewport1 
                    continue;
                }
                bool selected = prevSelectedCamera == camera[i]->name;
                if (ImGui::Selectable(camera[i]->name.c_str(), selected)) {
                    prevSelectedCamera = camera[i]->name;
                    prevSelectedCameraIndx = i;
                }
                if (selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        //ImGui::SameLine(0, p);
        if (ImGui::Button("set Camera", ImVec2((w - p) / 2.0f, 0)))
        {
            rndr->changeCamera(prevSelectedCameraIndx);
            prevSelectedCameraIndx = 0;
            errorMsg = "Successfully changed camera";
        }
        if (viewer->isSplitX) {
            if (ImGui::Button("set for right", ImVec2((w - p) / 2.0f, 0)))
            {
                rndr->changeSplitXCamera(prevSelectedCameraIndx);
                errorMsg = "Successfully changed camera";
            }
        }
        if (viewer->isSplitY) {
            ImGui::SameLine(0, p);
            if (ImGui::Button("set for up", ImVec2((w - p) / 2.0f, 0)))
            {
                rndr->changeSplitYCamera(prevSelectedCameraIndx);
                errorMsg = "Successfully changed camera";
            }
        }

        if (viewer->setCameraPathBezier) {
            if (ImGui::Button("choose Path", ImVec2((w - p) / 2.0f, 0)))
            {
                auto& cameraPath = viewer->camerasPaths.find(camera[0]->name)->second;

                if (cameraPath.size() == 4) {
                    for (int i = 0; i < 3; i++)
                        viewer->data_list[viewer->cameraPathStartIndx + i]->SetTranslation(Eigen::Vector3d(0,0,0));
                }
                viewer->data_list[11]->clear();
                viewer->layers[0]->isVisible = true;
                viewer->setCameraPathBezier = false;

          }
         
        if (!viewer->moveCameraBezier) {
            if (ImGui::Button("move bezier", ImVec2((w - p) / 2.0f, 0))) {
                if (viewer->setCameraPathBezier) {
                    viewer->cameraIdMoveBezier = prevSelectedCameraIndx;
                    viewer->moveCameraBezier = true;
                }
            }

        }
        else {
            if (ImGui::Button("stop bezier", ImVec2((w - p) / 2.0f, 0))) {
                viewer->moveCameraBezier = false;
            }
        }
    }
    else {
        if (ImGui::Button("set the Path", ImVec2((w - p) / 2.0f, 0)))
        {
            viewer->selected_data_index = viewer->data_list.size() - 1;
            viewer->setCameraPathBezier = true;
            std::vector<Eigen::Vector3d> cameraPath;
            cameraPath.push_back(camera[0]->GetTranslation());
            for (int i = 0; i < 3; i++)
            {
                cameraPath.push_back(viewer->data_list[viewer->cameraPathStartIndx + i]->GetTranslation());
            }
            auto it = viewer->camerasPaths.find(camera[0]->name);
            if (it != viewer->camerasPaths.end()) {
                it->second = cameraPath;
            }
            else
                viewer->camerasPaths.emplace(camera[0]->name, cameraPath);
            viewer->layers[0]->isVisible = false;

        }
    }
}

  if (ImGui::CollapsingHeader("screen Options", ImGuiTreeNodeFlags_DefaultOpen))
  {
      ImGui::Checkbox("Zoom area",
          [&]() { return rndr->tryToZoom; },
          [&](bool value) { rndr->tryToZoom = value; });
      auto iter = rndr->cameraPrevZoomLocation.find(camera[0]->name);
      //we did zoom to area 
      if (iter != rndr->cameraPrevZoomLocation.end() && !iter->second.empty()) {
          ImGui::SameLine();
          if (ImGui::Button("unzoom", ImVec2((w - p)/2, 0)))
          {
              camera[0]->SetTranslation(iter->second.back());
              iter->second.pop_back();
          }

      }
      ImGui::Checkbox("split x",
          [&]() { return viewer->isSplitX; },
          [&](bool value) { viewer->isSplitX = value;
      viewer->splitX(); });
      ImGui::Checkbox("split Y",
          [&]() { return viewer->isSplitY; },
          [&](bool value) { viewer->isSplitY = value;
          viewer->splitY(); });
  }

  // Helper for setting viewport specific mesh options
  auto make_checkbox = [&](const char *label, unsigned int &option)
  {
    return ImGui::Checkbox(label,
      [&]() { return drawInfos[1]->is_set(option); },
      [&](bool value) { return drawInfos[1]->set(option, value); }
    );
  };
      ImGui::ColorEdit4("Background", drawInfos[1]->Clear_RGBA.data(),
      ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel);

  if (ImGui::CollapsingHeader("Objects Options", ImGuiTreeNodeFlags_DefaultOpen))
   {
      ImGui::Text("Material Options:");
      if (ImGui::BeginCombo("Material", currentMaterial.c_str())) {
          for (int i = 7; i < viewer->materials.size(); i++) {
              if (viewer->materials[i]->canChoose()) {
                  bool selected = strcmp(currentMaterial.c_str(), viewer->materials[i]->getName().c_str());
                  if (ImGui::Selectable(viewer->materials[i]->getName().c_str(), selected)) {
                      currentMaterial = viewer->materials[i]->getName();
                      materialIndx = i;
                  }
                  if (selected) {
                      ImGui::SetItemDefaultFocus();
                  }
              }
          }
              ImGui::EndCombo();
          }
          if (ImGui::Button("set Material", ImVec2((w - p) / 2.0f, 0)))
          {
              viewer->updateMaterialForSelectedShapes(materialIndx);
          }
          ImGui::SameLine(0, p);
          if (ImGui::Button("add Material", ImVec2((w - p) / 2.0f, 0)))
          {
              viewer->open_dialog_load_Texture();
          }

      ImGui::Text("Blur Options:");
      if (ImGui::Button("make##Blur", ImVec2((w - p)/2.0f, 0)))
      {
          viewer->makeBlur();
      }
      ImGui::SameLine(0, p);
      if (ImGui::Button("remove##Blur", ImVec2((w - p)/2.0f, 0)))
      {
          viewer->removeBlur();
      }
      ImGui::Text("Transparent Options:");
      if (ImGui::Button("make##Transparent", ImVec2((w - p) / 2.0f, 0)))
      {
          viewer->makeTransparent();
      }
      ImGui::SameLine(0, p);
      if (ImGui::Button("remove##Transparent", ImVec2((w - p) / 2.0f, 0)))
      {
          viewer->removeTransparent();
      }


   }


  if (ImGui::CollapsingHeader("layers", ImGuiTreeNodeFlags_DefaultOpen))
  {
      ImGui::InputText("Name", layerName , 30);
      if (ImGui::Button("add layer", ImVec2((w - p) , 0)))
      {
          std::string name(layerName);
          if (!name.empty()) {
              viewer->addLayer(name);
              errorMsg = "Successfully added layer";
          }
          else {
              errorMsg = "please enter layer name";
          }
      }
      if (ImGui::Button("remove layer", ImVec2((w - p), 0)))
      {
          std::string name(layerName);
          if (!name.empty())
              if (!viewer->removeLayer(name))
                  errorMsg = "layer doesnt exists";
              else
                  errorMsg = "Successfully removed the layer";

          else {
                  errorMsg = "please enter layer name";
          }
      }
      if (ImGui::Button("set layer", ImVec2((w - p), 0)))
      {
          std::string name(layerName);
          if (!name.empty())
              if (!viewer->setLayer(name))
                  errorMsg = "layer doesnt exists";
              else
                  errorMsg = "Successfully set layer\n for picked objects";
              else {
                  errorMsg = "please enter layer name";
              }
      }
      for (auto iter : viewer->layers) {
          if (iter->layerNum != 0) {
              std::string text = "show layer";
              ImGui::Checkbox(iter->name.c_str(),
                  [&]() { return iter->isVisible; },
                  [&](bool value) { iter->isVisible = value; });
          }
      }
      ImGui::Text(errorMsg.c_str());
  }
  

  ImGui::End();
}



IGL_INLINE float ImGuiMenu::pixel_ratio()
{
  // Computes pixel ratio for hidpi devices
  int buf_size[2];
  int win_size[2];
  GLFWwindow* window = glfwGetCurrentContext();
  glfwGetFramebufferSize(window, &buf_size[0], &buf_size[1]);
  glfwGetWindowSize(window, &win_size[0], &win_size[1]);
  return (float) buf_size[0] / (float) win_size[0];
}

IGL_INLINE float ImGuiMenu::hidpi_scaling()
{
  // Computes scaling factor for hidpi devices
  float xscale, yscale;
  GLFWwindow* window = glfwGetCurrentContext();
  glfwGetWindowContentScale(window, &xscale, &yscale);
  return 0.5 * (xscale + yscale);
}

} // end namespace
} // end namespace
} // end namespace
} // end namespace
