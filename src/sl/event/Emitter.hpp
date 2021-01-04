#pragma once

#include <memory>

#include <xvent/EventEmitter.h>
#include "sl/core/Logger.h"

namespace sl::event {

class Emitter {
public:
    static void init(xvent::EventEmitter&& eventEmitter) {
        m_xventEmitter = std::make_unique<xvent::EventEmitter>(std::move(eventEmitter));
    }

    template <typename Ev, typename... Args>
    static void emit(Args&&... args) {
		m_xventEmitter->emit<Ev>(std::forward<Args>(args)...);
    }

    template <typename Ev, typename... Args>
    static void emitTo(std::string destination, Args&&... args) {
        m_xventEmitter->emitTo<Ev>(std::move(destination), std::forward<Args>(args)...);
    }

    template <typename Ev, typename... Args>
    static void emitTo(std::vector<std::string> destinations, Args&&... args) {
        m_xventEmitter->emitTo<Ev>(std::move(destinations), std::forward<Args>(args)...);
    }


    inline static std::unique_ptr<xvent::EventEmitter> m_xventEmitter = nullptr;
};
}
