#include <arba/dpat/under/under.hpp>

#include <gtest/gtest.h>

#include <memory>

namespace ut
{

constexpr unsigned hierarchy_superior_traits = dpat::hierarchy_superior_traits::substiuable;

class application;
class graphics_engine;
class scene_node;

class scene_node : public dpat::under<graphics_engine, hierarchy_superior_traits>
{
public:
    explicit scene_node(graphics_engine& gr_eng) : dpat::under<graphics_engine, hierarchy_superior_traits>(gr_eng) {}

    using dpat::under<graphics_engine, hierarchy_superior_traits>::has_superior;

    [[nodiscard]] inline const graphics_engine& engine() const { return this->superior(); }
    [[nodiscard]] inline graphics_engine& engine() { return this->superior(); }

    [[nodiscard]] inline const application& app() const { return this->superior<application>(); }
    [[nodiscard]] inline application& app() { return this->superior<application>(); }
};

class graphics_engine : public dpat::under<application, hierarchy_superior_traits>
{
public:
    explicit graphics_engine(application& app)
        : dpat::under<application, hierarchy_superior_traits>(app), root_scene_node(*this)
    {
    }

    using dpat::under<application, hierarchy_superior_traits>::has_superior;
    using dpat::under<application, hierarchy_superior_traits>::set_superior;

    [[nodiscard]] inline const application& app() const { return this->superior(); }
    [[nodiscard]] inline application& app() { return this->superior(); }

    scene_node root_scene_node;
};

class application
{
public:
    explicit application() : gr_engine_uptr(std::make_unique<graphics_engine>(*this)) {}

    std::unique_ptr<graphics_engine> gr_engine_uptr;
};

} // namespace ut

TEST(under_substiuable_tests, superior__direct_superior__ok)
{
    ut::application app;
    ASSERT_EQ(&app, &app.gr_engine_uptr->app());
    ASSERT_EQ(app.gr_engine_uptr.get(), &app.gr_engine_uptr->root_scene_node.engine());
}

TEST(under_substiuable_tests, const_superior__direct_superior__ok)
{
    const ut::application app;
    ASSERT_EQ(&app, &app.gr_engine_uptr->app());
    ASSERT_EQ(app.gr_engine_uptr.get(), &app.gr_engine_uptr->root_scene_node.engine());
}

TEST(under_substiuable_tests, superior__indirect_superior__ok)
{
    ut::application app;
    ASSERT_EQ(&app, &app.gr_engine_uptr->root_scene_node.app());
}

TEST(under_substiuable_tests, const_superior__indirect_superior__ok)
{
    const ut::application app;
    ASSERT_EQ(&app, &app.gr_engine_uptr->root_scene_node.app());
}

TEST(under_substiuable_tests, has_superior__ok)
{
    const ut::application app;
    ASSERT_TRUE(app.gr_engine_uptr->has_superior());
    ASSERT_TRUE(app.gr_engine_uptr->root_scene_node.has_superior());
}

TEST(under_substiuable_tests, set_superior__ok)
{
    ut::application app;
    ut::application new_app;
    new_app.gr_engine_uptr = std::move(app.gr_engine_uptr);
    new_app.gr_engine_uptr->set_superior(new_app);
    ASSERT_EQ(&new_app, &new_app.gr_engine_uptr->app());
}
