#pragma once

inline namespace arba
{
namespace dpat
{

template <class Type>
class singleton
{
public:
    [[nodiscard]] static Type& instance()
    {
        static Type instance_ = Type::make_singleton_instance();
        return instance_;
    }

protected:
    [[nodiscard]] inline static Type make_singleton_instance() { return Type(); }

private:
    singleton() = default;
    friend Type;
};

} // namespace dpat
} // namespace arba
