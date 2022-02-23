#pragma once

#include <kc/core/FileSystem.h>

#include <memory>

#include "fwd.h"
#include "sl/async/PeriodicTask.h"
#include "sl/core/GameObject.h"
#include "sl/event/Event.h"
#include "sl/event/EventManager.h"
#include "sl/math/Matrix.hpp"
#include "sl/math/Vector.hpp"

namespace sl::gfx {

class Shader : public core::GameObject {
    friend class ShaderCompiler;

   public:
    class RecompileOnUpdate : public async::PeriodicTask {
       public:
        explicit RecompileOnUpdate(std::shared_ptr<gfx::Shader> shader,
                                   const kc::core::FileSystem& fileSystem = kc::core::FileSystem{});

        bool shouldInvoke() override;
        void invoke() override;

        std::string getName() const;

       private:
        std::weak_ptr<gfx::Shader> m_shader;

        kc::core::FileSystem m_fileSystem;

        const std::string m_vertexShaderPath;
        const std::string m_fragmentShaderPath;

        std::filesystem::file_time_type m_previousVertexWrite;
        std::filesystem::file_time_type m_previousFragmentWrite;

        bool m_wasCompiledCorrectly;
    };

    struct Factory {
        virtual std::shared_ptr<Shader> create(const std::string&, const std::string&,
                                               const std::string& = "") = 0;
        virtual ~Factory() = default;
    };

    std::string name = "";

    virtual ~Shader() = default;

    virtual void enable() = 0;
    virtual void disable() = 0;

    virtual void setUniform(const std::string&, float) = 0;
    virtual void setUniform(const std::string&, int) = 0;
    virtual void setUniform(const std::string&, unsigned int) = 0;
    virtual void setUniform(const std::string&, math::Mat4) = 0;
    virtual void setUniform(const std::string&, math::Mat3) = 0;
    virtual void setUniform(const std::string&, math::Vec4) = 0;
    virtual void setUniform(const std::string&, math::Vec3) = 0;

    virtual std::string getVertexShaderPath() = 0;
    virtual std::string getFragmentShaderPath() = 0;
    virtual std::string getGeometryShaderPath() = 0;
};

}  // namespace sl::gfx
