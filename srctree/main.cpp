#include <stdio.h>
#include <iostream>
#include <iomanip>

#include "imgui.h"
#include "cgra/imgui_impl_glfw_gl3.h"

#include "bvhApp.hpp"

#include <chrono>
#include <thread>
using namespace std::literals;

// Forward definition of callbacks
extern "C" {
    static void glfw_error_callback(int error, const char* msg);

    void APIENTRY debugCallback(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, GLvoid*);

    static void key_callback(GLFWwindow *, int, int, int, int);
    static void mouse_button_callback(GLFWwindow *, int, int, int);
    static void cursor_pos_callback(GLFWwindow *, double, double);
    static void scroll_callback(GLFWwindow *, double, double);
    static void char_callback(GLFWwindow *, unsigned int codepoint);
}

int main(int argc, const char** argv) {
    (void)argc;
    (void)argv;

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Error: Could not initialize GLFW" << std::endl;
        return 1;
    }
    glfwSetErrorCallback(glfw_error_callback);


    // Request at least OpenGL 3.3 with the Core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Don't allow legacy functionality
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Request a debug context so we can use debug callbacks
    // Can reduce GL performance.
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // Explicitly request doublebuffering. This is normally the default,
    // but is worth being explicit about
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "CGRA Assignment", NULL, NULL);
    if (window == nullptr) {
        return 1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE; // required for full functionality in OpenGL 3.0+
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
        return 1;
    }

    // Initialize ImGui
    ImGui_ImplGlfwGL3_Init(window, false);

    // Set callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCharCallback(window, char_callback);

    // Read the OpenGL version
    GLint glMajor, glMinor;
    glGetIntegerv(GL_MAJOR_VERSION, &glMajor);
    glGetIntegerv(GL_MINOR_VERSION, &glMinor);

    // Read the GLFW version
    int glfwMajor, glfwMinor, glfwRevision;
    glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version: " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "GLFW Version: " << glfwMajor << '.' << glfwMinor << '.' << glfwRevision << std::endl;

    // Enable debug messages if available
    if (glMajor > 4 || (glMajor == 4 && glMinor >= 3)) {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        glDebugMessageCallback(debugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
    } else if (GLEW_ARB_debug_output) {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

        glDebugMessageCallbackARB(debugCallback, nullptr);
        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
    }

    // Enabe depth testing
    glEnable(GL_DEPTH_TEST);
    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    {
        // Create the application object
        BVHApp_Application app(window);
  gl_errorFlush("constructApp");
    
     // Tell GLFW to pass along a pointer to `app` in callbacks
        glfwSetWindowUserPointer(window, reinterpret_cast<void *>(&app));
  gl_errorFlush("set wu pointer");

        try {
			// Initialise `app`
      const char * skelPath = nullptr;//"";
      if (argc > 1){skelPath = argv[1];}
      app.init(skelPath);
            // Loop until the GLFW window is marked to be closed
            while (!glfwWindowShouldClose(app.m_window)) {
                clock_t the_t = clock();
                // Poll GLFW for input events
                glfwPollEvents();                // Draw the scene.
                app.updateScene(the_t);
//	std::this_thread::sleep_for(5ms);
            }
        } catch (std::exception e) {
            // Catch any exceptions that bubble up to here and print out
            // the message
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }

    // Clean up
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    // 0 is the "everything went OK" value when returned from
    // main
    return 0;
}

static const char *getStringForSource(GLenum source) {

    switch(source) {
    case GL_DEBUG_SOURCE_API:
        return("API");
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        return("Window System");
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        return("Shader Compiler");
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        return("Third Party");
    case GL_DEBUG_SOURCE_APPLICATION:
        return("BVHApp_Application");
    case GL_DEBUG_SOURCE_OTHER:
        return("Other");
    default:
        return("n/a");
    }
}

static const char *getStringForSeverity(GLenum severity) {

    switch(severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        return("HIGH!");
    case GL_DEBUG_SEVERITY_MEDIUM:
        return("Medium");
    case GL_DEBUG_SEVERITY_LOW:
        return("Low");
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        return("None");
    default:
        return("n/a");
    }
}

static const char *getStringForType(GLenum type) {
    switch(type) {
    case GL_DEBUG_TYPE_ERROR:
        return("Error");
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        return("Deprecated Behaviour");
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        return("Undefined Behaviour");
    case GL_DEBUG_TYPE_PORTABILITY:
        return("Portability Issue");
    case GL_DEBUG_TYPE_PERFORMANCE:
        return("Performance Issue");
    case GL_DEBUG_TYPE_OTHER:
        return("Other");
    default:
        return("n/a");
    }
}
extern "C" {
    static GLenum g_prevSource;
    static GLenum g_prevType;
    static GLenum g_prevSeverity;
    static GLenum g_prevId;
    static int g_headerCount = 0;

    void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar* message, GLvoid*) {
        // Don't report notification messages
        if (severity == GL_DEBUG_SEVERITY_LOW ) return; // TODO: Defer these warnings to some less annoying output
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;

        if (id == g_prevId) return;
        if (g_headerCount == 0 || g_prevSource != source || g_prevType != type || g_prevSeverity != severity) {
            std::cerr << std::endl; // extra space
            std::cerr << "Type: " <<
                getStringForType(type) << "; Source: " <<
                getStringForSource(source) <<"; Severity: " <<
                getStringForSeverity(severity) << std::endl;

            g_headerCount = 0;
        }

        // If more than 128 messages have come through without printing a new header,
        // this will cause one to be printed next time
        g_headerCount = (g_headerCount + 1) % 128;

        g_prevSource   = source;
        g_prevType     = type;
        g_prevSeverity = severity;
        g_prevId       = id;

        std::cerr << '[' << std::setw(3) << id << "] " << message << std::endl;
    }

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        BVHApp_Application *app = (BVHApp_Application *)glfwGetWindowUserPointer(window);
        if (app == nullptr) return;

        ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mods);

        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureKeyboard) return;

        app->onKey(key, scancode, action, mods);
    }

    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
        BVHApp_Application *app = (BVHApp_Application *)glfwGetWindowUserPointer(window);
        if (app == nullptr) return;

        ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);

        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse) return;

        app->onMouseButton(button, action, mods);
    }

    static void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) {
        BVHApp_Application *app = (BVHApp_Application *)glfwGetWindowUserPointer(window);
        if (app == nullptr) return;

        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse) return;

        app->onCursorPos(xpos, ypos);
    }

    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
        BVHApp_Application *app = (BVHApp_Application *)glfwGetWindowUserPointer(window);
        if (app == nullptr) return;

        ImGui_ImplGlfwGL3_ScrollCallback(window, xoffset, yoffset);

        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse) return;

        app->onScroll(xoffset, yoffset);
    }

    static void char_callback(GLFWwindow *window, unsigned int codepoint) {
        BVHApp_Application *app = (BVHApp_Application *)glfwGetWindowUserPointer(window);
        if (app == nullptr) return;

        ImGui_ImplGlfwGL3_CharCallback(window, codepoint);
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantTextInput) return;

        // Don't pass along char callback events for now
    }

    static void glfw_error_callback(int error, const char* msg) {
        std::cerr << "GLFW Error (" << error << "): " << msg << std::endl;
    }
}

void gl_errorFlush(const char  * at){
  int err = glGetError();
  if(err>0) std::cout << " GL ERROR : " << err << " : " << glewGetErrorString(err) << " at " << at <<"\n";
}


