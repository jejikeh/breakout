// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "base.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "arkanoid.h"

#include <stdio.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static const char* get_gl_glsl_version()
{
#ifdef __APPLE__
    // GL 3.2 + GLSL 320
    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    return glsl_version;
}

static GLFWwindow* create_window(int width, int height, const char* title)
{
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (!window)
    {
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");

        return nullptr;
    }

    return window;
}

inline void init_imgui_io(GLFWwindow* window, const char* glsl_version)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer back-ends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

struct ArkanoidSettingsExtended : ArkanoidSettings
{
    bool step_by_step = false;
    bool debug_draw = false;
    float debug_draw_pos_radius = 5.0f;
    float debug_draw_normal_length = 30.0f;
    float debug_draw_timeout = 0.5f;
};

int main(int, char**)
{
    // @Note(jejikeh): I just refactor some of code to separate functions. Just helps me figoure out what's going on.

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        return 1;
    }

    // Decide GL+GLSL versions
    auto glsl_version = get_gl_glsl_version();

    // Create window with graphics context
    auto window = create_window(1280, 720, "breakout");

    // Setup Dear ImGui context
    init_imgui_io(window, glsl_version);

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImVec4 clear_color = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);

    // Create gameplay and settings classes
    ArkanoidDebugData arkanoid_debug_data;
    ArkanoidSettingsExtended arkanoid_settings;

    Arkanoid* arkanoid = create_arkanoid();
    arkanoid->reset(arkanoid_settings);

    // Main loop
    double last_time = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();

        double cur_time = glfwGetTime();
        float elapsed_time = static_cast<float>(std::min(cur_time - last_time, 1.0));
        last_time = cur_time;

        bool do_arkanoid_update = true;

        // update settings window
        {
            ImGui::Begin("Arkanoid");

            ImGui::InputFloat2("World size", arkanoid_settings.world_size.data_);

            ImGui::Spacing();
            ImGui::SliderInt(
                "Bricks columns",
                &arkanoid_settings.bricks_columns_count,
                ArkanoidSettings::bricks_columns_min,
                ArkanoidSettings::bricks_columns_max);
            ImGui::SliderInt(
                "Bricks rows", &arkanoid_settings.bricks_rows_count, ArkanoidSettings::bricks_rows_min, ArkanoidSettings::bricks_rows_max);

            ImGui::Spacing();

            ImGui::SliderFloat(
                "Bricks padding columns",
                &arkanoid_settings.bricks_columns_padding,
                ArkanoidSettings::bricks_columns_padding_min,
                ArkanoidSettings::bricks_columns_padding_max);
            ImGui::SliderFloat(
                "Bricks padding rows",
                &arkanoid_settings.bricks_rows_padding,
                ArkanoidSettings::bricks_rows_padding_min,
                ArkanoidSettings::bricks_rows_padding_max);

            ImGui::Spacing();

            ImGui::SliderFloat(
                "Bricks height",
                &arkanoid_settings.bricks_height,
                ArkanoidSettings::bricks_height_min,
                ArkanoidSettings::bricks_height_max);

            ImGui::Spacing();
            ImGui::SliderFloat(
                "Ball radius", &arkanoid_settings.ball_radius, ArkanoidSettings::ball_radius_min, ArkanoidSettings::ball_radius_max);
            ImGui::SliderFloat(
                "Ball speed", &arkanoid_settings.ball_speed, ArkanoidSettings::ball_speed_min, ArkanoidSettings::ball_speed_max);

            ImGui::Spacing();
            ImGui::SliderFloat(
                "Carriage width", &arkanoid_settings.carriage_width, ArkanoidSettings::carriage_width_min, arkanoid_settings.world_size.x);

            if (ImGui::Button("Reset"))
                arkanoid->reset(arkanoid_settings);

            ImGui::End();
        }

        // debug window
        {
            ImGui::Begin("Arkanoid Debug");
            ImGui::Checkbox("Debug draw", &arkanoid_settings.debug_draw);

            if (arkanoid_settings.debug_draw)
            {
                ImGui::SliderFloat("Hit pos radius", &arkanoid_settings.debug_draw_pos_radius, 3.0f, 15.0f);
                ImGui::SliderFloat("Hit normal length", &arkanoid_settings.debug_draw_normal_length, 10.0f, 100.0f);
                ImGui::SliderFloat("Timeout", &arkanoid_settings.debug_draw_timeout, 0.1f, 10.0f);
            }

            ImGui::Spacing();
            ImGui::Checkbox("Steps by step", &arkanoid_settings.step_by_step);

            if (arkanoid_settings.step_by_step)
                do_arkanoid_update = false;

            if (ImGui::Button("Next step (SPACE Key)") || io.KeysDown[GLFW_KEY_SPACE])
                do_arkanoid_update = true;

            ImGui::End();
        }

        ImDrawList* bg_drawlist = ImGui::GetBackgroundDrawList();

        // update/render game
        {
            if (do_arkanoid_update)
            {
                arkanoid->update(io, arkanoid_debug_data, elapsed_time);

                // update debug draw data time
                size_t remove_by_timeout_count = 0;
                for (auto& hit : arkanoid_debug_data.hits)
                {
                    hit.time += elapsed_time;
                    if (hit.time > arkanoid_settings.debug_draw_timeout)
                        remove_by_timeout_count++;
                }

                // cleat outdated debug info
                if (remove_by_timeout_count > 0)
                {
                    std::rotate(
                        arkanoid_debug_data.hits.begin(),
                        arkanoid_debug_data.hits.begin() + remove_by_timeout_count,
                        arkanoid_debug_data.hits.end());

                    arkanoid_debug_data.hits.resize(arkanoid_debug_data.hits.size() - remove_by_timeout_count);
                }
            }

            arkanoid->draw(io, *bg_drawlist);
        }

        // debug draw
        if (arkanoid_settings.debug_draw)
        {
            const float len = arkanoid_settings.debug_draw_normal_length;
            for (const auto& hit : arkanoid_debug_data.hits)
            {
                bg_drawlist->AddCircleFilled(hit.screen_pos, arkanoid_settings.debug_draw_pos_radius, ImColor(255, 255, 0));
                bg_drawlist->AddLine(hit.screen_pos, hit.screen_pos + hit.normal * len, ImColor(255, 0, 0));
            }
        }

        // Rendering
        ImGui::Render();

        int display_w, display_h;

        glfwGetFramebufferSize(window, &display_w, &display_h);

        glViewport(0, 0, display_w, display_h);

        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}