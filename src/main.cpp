#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "arraysort.h"

#define ARR_SIZE 16

int main() {
    if (!glfwInit()) return EXIT_FAILURE;

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#endif

    // Use a modern OpenGL version
#ifdef __APPLE__
    const char *glsl_version = "#version 150";
#else
    const char *glsl_version = "#version 130";
#endif


    GLFWwindow *window = glfwCreateWindow(1280, 720, "Algorithm Visualizer", nullptr, nullptr);
    if (!window) return EXIT_FAILURE;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable VSync

    // 2. Setup ImGui Context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Keyboard controls

    // Setup Style
    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);

    // 3. Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    std::vector<float> values(ARR_SIZE);
    for (int i = 0; i < ARR_SIZE; ++i) values[i] = (float) i + 1;

    int arraySize = ARR_SIZE;
    float delayValue = 1.0f;

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

        static float footer_height = 120.0f;

        ImGui::BeginChild("ViewPort", ImVec2(0, -footer_height), true);

            // Draw the visualizer bars
            // ImGui::PlotHistogram allows us to visualize the vector instantly
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.2f, 0.6f, 1.0f, 1.0f));
            ImGui::PlotHistogram("##Values", values.data(), (int) values.size(), 0, nullptr, 0.0f, (float) arraySize,
                             ImGui::GetContentRegionAvail());
            ImGui::PopStyleColor();

        ImGui::EndChild();

        ImGui::Spacing();
        ImGui::Text("Controls:");

        ImGui::BeginGroup();
            ImGui::SetNextItemWidth(200.0f);
            ImGui::DragFloat("Animation Speed", &delayValue, 0.005f, 0.0f, 1.0f, "%.3f s");

            if (ImGui::Button("Double Array") && !ArraySort::isSorting) {
                arraySize *= 2;

                for (int i = values.size(); i < arraySize; ++i) {
                    values.push_back((float) i + 1);
                }
            }
            ImGui::SameLine();

            if (ImGui::Button("Shuffle Data") && !ArraySort::isSorting) {
                std::shuffle(std::begin(values), std::end(values), std::default_random_engine());
            }
        ImGui::EndGroup();

        ImGui::SameLine(0, 40);

        ImGui::BeginGroup();

            if (ImGui::Button("Bubble Sort") && !ArraySort::isSorting) {
                ArraySort::isSorting = true;

                std::thread(ArraySort::bubbleSort, std::ref(values), delayValue).detach();
            }
            ImGui::SameLine();

            if (ImGui::Button("Selection Sort")) {
                ArraySort::isSorting = true;

                std::thread(ArraySort::selectionSort, std::ref(values), delayValue).detach();
            }
            ImGui::SameLine();

            if (ImGui::Button("Insertion Sort")) {
                ArraySort::isSorting = true;

                std::thread(ArraySort::insertionSort, std::ref(values), delayValue).detach();
            }
            ImGui::SameLine();

            if (ImGui::Button("Merge Sort")) {
                ArraySort::isSorting = true;

                std::thread(ArraySort::mergeSort, std::ref(values), delayValue).detach();
            }
            ImGui::SameLine();

            ImGui::Button("Quick Sort");
            ImGui::SameLine();

        ImGui::EndGroup();

        ImGui::End(); // End MainVisualizer

        std::lock_guard<std::mutex> lock(ArraySort::dataMutex);

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

    return EXIT_SUCCESS;
}
