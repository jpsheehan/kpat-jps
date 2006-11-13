/*
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * This file is provided AS IS with no warranties of any kind.  The author
 * shall have no liability with respect to the infringement of copyrights,
 * trade secrets or any patents by this file or any part thereof.  In no
 * event will the author be liable for any lost revenue or profits or
 * other special, indirect and consequential damages.
*/
#ifndef YUKON_H
#define YUKON_H

#include "dealer.h"

class Yukon : public DealerScene {
    friend class YukonSolver;

    Q_OBJECT

public:
    Yukon( );

public slots:
    void deal();
    virtual void restart();

private:
    Pile* store[7];
    Pile* target[4];
};

#endif
