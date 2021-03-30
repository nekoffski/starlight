#include "sl/app/Entrypoint.h"

#include "EditorContext.hpp"

using namespace sl;
using namespace sl::scene;

class StarlightEditor : public sl::app::Application {
public:
    virtual void onStart() override {
        context = createContext<EditorContext>("EditorContext");
        switchContext(context);
    }

private:
    std::shared_ptr<EditorContext> context;
};

SL_ENTRYPOINT(StarlightEditor);
