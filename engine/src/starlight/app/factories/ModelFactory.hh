#pragma once

#include "starlight/core/memory/Memory.hh"
#include "starlight/core/Factory.hh"
#include "starlight/app/model/Model.hh"
#include "starlight/app/model/CustomModel.hh"

namespace sl {

class ModelFactory : public Factory<ModelFactory, Model> {
public:
    explicit ModelFactory();

    SharedPtr<CustomModel> create(OptStr name = {});
    // SharedPtr<Model> load(const std::string& name);

    SharedPtr<Model> getDefault();

private:
    SharedPtr<CustomModel> m_defaultModel;
};

}  // namespace sl
