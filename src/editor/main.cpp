#include "sl/application/Entrypoint.h"

#include "EditorContext.hpp"

using namespace sl;
using namespace sl::scene;

class StarlightEditor : public sl::application::Application {
    SL_APPLICATION;

public:
    virtual void onStart() override {
        context = createContext<EditorContext>();
        switchContext(context);
    }

private:
    std::shared_ptr<EditorContext> context;
};

SL_ENTRYPOINT(StarlightEditor);
