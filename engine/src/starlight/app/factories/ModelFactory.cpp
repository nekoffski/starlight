#include "ModelFactory.hh"

#include "MaterialFactory.hh"
#include "MeshFactory.hh"

namespace sl {

// SharedPtr<Model> ModelFactory::load(const std::string& name) {}

ModelFactory::ModelFactory() {
    m_defaultModel = create("Cube");
    m_defaultModel
      ->addSub(MeshFactory::get().getCube(), MaterialFactory::get().getDefault());
}

SharedPtr<CustomModel> ModelFactory::create(OptStr name) {
    if (name) {
        // TODO: need shared pointer dowcasting
        // if (auto resource = find(*name); resource) [[unlikely]]
        // return resource;
    }

    auto model = SharedPtr<CustomModel>::create();
    save(model);
    return model;
}

SharedPtr<Model> ModelFactory::getDefault() { return m_defaultModel; }

}  // namespace sl
