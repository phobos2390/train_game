/// @file train_game/railcar.h

#ifndef TRAIN_GAME_RAILCAR_H
#define TRAIN_GAME_RAILCAR_H

namespace train_game
{

class railcar
{
public:
    /// Creates class value
    railcar();

    /// Removes class value
    virtual ~railcar();
private:
    struct Impl;
    Impl* m_p_impl;
};

}

#endif /* TRAIN_GAME_RAILCAR_H */
