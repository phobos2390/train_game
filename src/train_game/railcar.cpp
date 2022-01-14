/// @file train_game/railcar.cpp

#include <train_game/railcar.h>

namespace train_game
{

struct railcar::Impl
{
public:
    Impl(){}
    virtual ~Impl(){}
};

railcar::railcar()
    :m_p_impl(new Impl)
{
}

railcar::~railcar()
{
    delete m_p_impl;
    m_p_impl = 0;
}

}
