#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#define ARR_SIZE 16

int main() {
    // 1. Initialize GLFW
    if (!glfwInit()) return EXIT_FAILURE;

    // Use a modern OpenGL version
    const char* glsl_version = "#version 130";
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Algorithm Visualizer", nullptr, nullptr);
    if (!window) return EXIT_FAILURE;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable VSync

    // 2. Setup ImGui Context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Keyboard controls

    // Setup Style
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;

    // 3. Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    std::vector<float> values(ARR_SIZE);
    for (int i = 0; i < ARR_SIZE; ++i) values[i] = (float)i + 1;

    float temp = (float)ARR_SIZE;

    // 4. Main Loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // We set the next window to cover the ENTIRE GLFW window area
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
                                        ImGuiWindowFlags_NoMove |
                                        ImGuiWindowFlags_NoResize |
                                        ImGuiWindowFlags_NoSavedSettings;

        ImGui::Begin("MainVisualizer", nullptr, window_flags);

        ImGui::Text("Algorithm Visualizer | Elements: %zu", values.size());
        ImGui::Separator();

        ImGui::Spacing();

        // Draw the visualizer bars
        // ImGui::PlotHistogram allows us to visualize the vector instantly
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.2f, 0.6f, 1.0f, 1.0f));
        ImGui::PlotHistogram("##Values", values.data(), (int)values.size(), 0, nullptr, 0.0f, (float)temp, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 100));
        ImGui::PopStyleColor();

        ImGui::Spacing();
        ImGui::Separator();

        if (ImGui::Button("Shuffle Array")) {
            std::shuffle(values.begin(), values.end(), std::default_random_engine());
        }

        if (ImGui::Button("Double Array")) {
            int lenght = values.size();

            for (int i = lenght; i < lenght * 2; ++i) {
                values.push_back((float)(values.size() + 1));
            }

            temp = (float)values.size();

            std::shuffle(values.begin(), values.end(), std::default_random_engine());
        }


        ImGui::End(); // End MainVisualizer

        // 5. Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.06f, 0.06f, 0.06f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // 6. Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}