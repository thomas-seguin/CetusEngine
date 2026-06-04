#include "Core/Application.h"

#include "AppLayer.h"
int main() {
    Cetus::ApplicationSpecification spec;
    spec.Name = "Application";

    Cetus::Application app(spec);
    app.PushLayer<AppLayer>();
    app.Run();
}