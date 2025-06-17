#pragma once

#include <cassert>
#include <type_traits>

inline namespace arba
{
namespace dpat
{

struct hierarchy_superior_traits
{
    enum
    {
        optional = 1,
        substiuable = 2
    };
};

template <class SuperiorType, unsigned SuperiorTraits = 0>
class under
{
protected:
    under()
        requires((SuperiorTraits & hierarchy_superior_traits::optional) > 0)
    {
    }

    explicit under(SuperiorType& superior) : superior_ptr_(&superior) { assert(&superior); }

    [[nodiscard]] inline bool has_superior() const noexcept
    {
        if constexpr ((SuperiorTraits & hierarchy_superior_traits::optional) == 0)
            return true;
        else
            return superior_ptr_;
    }

    [[nodiscard]] inline const SuperiorType& superior() const noexcept
    {
        assert(has_superior());
        return *superior_ptr_;
    }
    [[nodiscard]] inline SuperiorType& superior() noexcept
    {
        assert(has_superior());
        return *superior_ptr_;
    }

    inline void set_superior(SuperiorType& superior) noexcept
        requires(SuperiorTraits > 0)
    {
        assert(&superior);
        superior_ptr_ = &superior;
    }

    template <class RequestedType>
    [[nodiscard]] inline const RequestedType& superior() const
    {
        if constexpr (std::is_same_v<SuperiorType, RequestedType>)
        {
            return this->superior();
        }
        else
        {
            return this->superior().template superior<RequestedType>();
        }
    }

    template <class RequestedType>
    [[nodiscard]] inline RequestedType& superior()
    {
        if constexpr (std::is_same_v<SuperiorType, RequestedType>)
        {
            return this->superior();
        }
        else
        {
            return this->superior().template superior<RequestedType>();
        }
    }

    template <class, unsigned>
    friend class under;

private:
    SuperiorType* superior_ptr_ = nullptr;
};

} // namespace dpat
} // namespace arba
