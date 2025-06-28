#include <arba/dpat/singleton/singleton.hpp>

#include <gtest/gtest.h>

class global_tool : public dpat::singleton<global_tool>
{
    using singleton_ = dpat::singleton<global_tool>;
    friend singleton_;

    global_tool() {}
};

class global_text : public dpat::singleton<global_text>
{
    using singleton_ = dpat::singleton<global_text>;
    friend singleton_;

    explicit global_text(std::string_view data) : data_(data) {}
    [[nodiscard]] inline static global_text make_singleton_instance() { return global_text("global"); }

public:
    const std::string& text() const { return data_; }

private:
    std::string data_;
};

TEST(singleton_tests, test_global_tool)
{
    ASSERT_EQ(&global_tool::instance(), &global_tool::instance());
}

TEST(singleton_tests, test_global_text)
{
    ASSERT_EQ(&global_text::instance(), &global_text::instance());
    ASSERT_EQ(global_text::instance().text(), std::string_view("global"));
}
