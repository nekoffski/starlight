#include "starlight/core/Error.hh"

#include <gtest/gtest.h>

#include <type_traits>

namespace {

static_assert(std::is_same_v<sl::Error, kc::Error>);
static_assert(std::is_same_v<sl::Result<int>, kc::Result<int>>);

TEST(StarlightError, PropagatesLibraryErrorsWithoutChangingTheirCategory) {
    const auto load = []() -> sl::Result<void> {
        const kc::Result<int> value = kc::Error::unexpected(
            kc::ErrorCode::fileNotFound, "Missing {}", "shader"
        );
        return sl::Error::unexpected(value.error());
    };

    const auto r = load();
    ASSERT_FALSE(r);
    EXPECT_EQ(r.error().code(), kc::ErrorCode::fileNotFound);
    EXPECT_EQ(&r.error().code().category(), &kc::errorCategory());
    EXPECT_EQ(r.error().message(), "Missing shader");
}

TEST(StarlightError, DistinguishesEngineAndLibraryCodesWithTheSameNumber) {
    const sl::Error engine{sl::ErrorCode::tooManyFrameRequests, "Frame full"};
    const sl::Error library{kc::ErrorCode::fileSystemError, "Disk full"};

    EXPECT_EQ(engine.code().value(), library.code().value());
    EXPECT_NE(engine.code(), library.code());
    EXPECT_EQ(engine.code(), sl::ErrorCode::tooManyFrameRequests);
    EXPECT_EQ(&engine.code().category(), &sl::errorCategory());
    EXPECT_STREQ(engine.code().category().name(), "starlight");
    EXPECT_EQ(engine.code().message(), "Too many frame requests");
    EXPECT_EQ(
        fmt::format("{}", engine), "code: starlight:6, details: 'Frame full'"
    );

    sl::Error cleared = engine;
    cleared.reset();
    EXPECT_FALSE(cleared.valid());
    EXPECT_EQ(cleared.code(), kc::ErrorCode::noError);
}

}  // namespace
