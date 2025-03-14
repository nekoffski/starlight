#pragma once

#include "starlight/core/memory/Memory.hh"
#include "starlight/core/Factory.hh"
#include "starlight/app/model/Model.hh"
#include "starlight/app/model/CustomModel.hh"

namespace sl {

class ModelFactory : public Factory<ModelFactory, Model> {
public:
    SharedPtr<CustomModel> create(OptStr name = {});
    // SharedPtr<Model> load(const std::string& name);
};

}  // namespace sl
