#pragma once

#include <memory>

#include "sl/core/types/NotNullPtr.hpp"

namespace sl::gfx::buffer {

class ElementBuffer {
public:
	struct Factory {
		virtual std::shared_ptr<ElementBuffer> create(core::types::NotNullPtr<void>, int, unsigned int) = 0;
	};

	inline static std::unique_ptr<Factory> factory = nullptr;

    virtual ~ElementBuffer() = default;

    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual unsigned int getIndicesCount() = 0;
};
}
