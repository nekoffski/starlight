#pragma once

#include "Config.hh"
#include <kstd/Singleton.hh>

namespace sl {

class Globals : public kstd::Singleton<Globals> {
public:
    explicit Globals(const Config& config);

    const Config& getConfig() const;

private:
    Config m_config;
};

}  // namespace sl