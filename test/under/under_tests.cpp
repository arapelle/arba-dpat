#include <arba/dpat/under/under.hpp>

#include <gtest/gtest.h>

namespace ut
{

class application;
class graphics_engine;
class scene_node;

class scene_node : public dpat::under<graphics_engine>
{
public:
    explicit scene_node(graphics_engine& gr_eng) : dpat::under<graphics_engine>(gr_eng) {}

    using dpat::under<graphics_engine>::has_superior;

    [[nodiscard]] inline const graphics_engine& engine() const { return this->superior(); }
    [[nodiscard]] inline       graphics_engine& engine()       { return this->superior(); }

    [[nodiscard]] inline const application& app() const { return this->superior<application>(); }
    [[nodiscard]] inline       application& app()       { return this->superior<application>(); }
};

class graphics_engine : public dpat::under<application>
{
public:
    explicit graphics_engine(application& app) : dpat::under<application>(app), root_scene_node(*this) {}

    using dpat::under<application>::has_superior;

    [[nodiscard]] inline const application& app() const { return this->superior(); }
    [[nodiscard]] inline       application& app()       { return this->superior(); }

    scene_node root_scene_node;
};

class application
{
public:
    explicit application() : gr_engine(*this) {}

    graphics_engine gr_engine;
};

} // namespace ut

TEST(under_tests, superior__direct_superior__ok)
{
    ut::application app;
    ASSERT_EQ(&app, &app.gr_engine.app());
    ASSERT_EQ(&app.gr_engine, &app.gr_engine.root_scene_node.engine());
}

TEST(under_tests, const_superior__direct_superior__ok)
{
    const ut::application app;
    ASSERT_EQ(&app, &app.gr_engine.app());
    ASSERT_EQ(&app.gr_engine, &app.gr_engine.root_scene_node.engine());
}

TEST(under_tests, superior__indirect_superior__ok)
{
    ut::application app;
    ASSERT_EQ(&app, &app.gr_engine.root_scene_node.app());
}

TEST(under_tests, const_superior__indirect_superior__ok)
{
    const ut::application app;
    ASSERT_EQ(&app, &app.gr_engine.root_scene_node.app());
}

TEST(under_tests, has_superior__ok)
{
    const ut::application app;
    ASSERT_TRUE(app.gr_engine.has_superior());
    ASSERT_TRUE(app.gr_engine.root_scene_node.has_superior());
}
