#pragma once

#include <memory>

namespace camera::core { class IFrame; }

namespace camera::consumer
{

    class ITextureUploader
    {
    public:
        virtual ~ITextureUploader() = default;

        // Initialize uploader. rhiHandle may be used by implementations.
        virtual void init(void* rhiHandle) = 0;

        // Upload frame. The batch pointer is uploader-specific and may be null.
        virtual bool uploadFrame(std::shared_ptr<camera::core::IFrame> frame, void* batch) = 0;

        // Release GPU resources
        virtual void release() = 0;
    };

} // namespace camera::renderer
