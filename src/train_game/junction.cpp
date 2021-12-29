/// @file train_game/junction.cpp

#include <train_game/junction.h>

namespace train_game
{

struct junction::Impl
{
public:
    Impl(){}
    virtual ~Impl(){}
};

junction::junction()
    :m_p_impl(new Impl)
{
}

junction::~junction()
{
    delete m_p_impl;
    m_p_impl = 0;
}

}
