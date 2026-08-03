#include "AppLayer.h"
#include "Core/Log.h"

#include "imgui.h"

AppLayer::AppLayer() {
    Cetus::Log::GetClientLogger()->info("AppLayer created");
}

AppLayer::~AppLayer() {
    Cetus::Log::GetClientLogger()->info("AppLayer destroyed");
}

void AppLayer::OnEvent(Cetus::Event &event) {
    Cetus::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<Cetus::KeyPressedEvent>([](Cetus::KeyPressedEvent& e) {
        CT_INFO("Key pressed: {0} (repeat = {1})", e.GetKeyCode(), e.IsRepeat())
        return false; // don't mark handled, let it keep propagating for now
    });

    dispatcher.Dispatch<Cetus::MouseButtonPressedEvent>([](Cetus::MouseButtonPressedEvent& e) {
        CT_INFO("Mouse button pressed: {0}", e.GetMouseButton())
        return false;
    });
}


void AppLayer::OnUpdate(float ts) {

}

void AppLayer::OnRender() {

}

void AppLayer::OnImGuiRender() {
    ImGui::ShowDemoWindow();
}
