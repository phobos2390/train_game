/// @file train_game/junction.h

#ifndef TRAIN_GAME_JUNCTION_H
#define TRAIN_GAME_JUNCTION_H

namespace train_game
{

class junction
{
public:
    /// Creates class value
    junction();

    /// Removes class value
    virtual ~junction();
private:
    struct Impl;
    Impl* m_p_impl;
};

}

#endif /* TRAIN_GAME_JUNCTION_H */
