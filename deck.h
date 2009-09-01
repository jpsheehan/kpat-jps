/*
 * Copyright (C) 1995 Paul Olav Tvete <paul@troll.no>
 * Copyright (C) 2000-2009 Stephan Kulow <coolo@kde.org>
 *
 * License of original code:
 * -------------------------------------------------------------------------
 *   Permission to use, copy, modify, and distribute this software and its
 *   documentation for any purpose and without fee is hereby granted,
 *   provided that the above copyright notice appear in all copies and that
 *   both that copyright notice and this permission notice appear in
 *   supporting documentation.
 *
 *   This file is provided AS IS with no warranties of any kind.  The author
 *   shall have no liability with respect to the infringement of copyrights,
 *   trade secrets or any patents by this file or any part thereof.  In no
 *   event will the author be liable for any lost revenue or profits or
 *   other special, indirect and consequential damages.
 * -------------------------------------------------------------------------
 *
 * License of modifications/additions made after 2009-01-01:
 * -------------------------------------------------------------------------
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of 
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * -------------------------------------------------------------------------
 */

#ifndef DECK_H
#define DECK_H

#include "pile.h"

/***************************************

  Deck (Pile with id 0) -- create and shuffle 52 cards

**************************************/
class Deck: public Pile
{

private:
    explicit Deck( DealerScene* parent = 0, int m = 1, int s = 4 );
    virtual ~Deck();

public:
    static void createDeck( DealerScene *parent = 0, uint m = 1, uint s = 4 );
    static void destroyDeck();
    static Deck *deck() { return my_deck; }

    void collectAndShuffle();

    Card* nextCard();

    uint decksNum() const { return mult; }
    uint suitsNum() const { return suits; }
    void updatePixmaps();

private: // functions

    void makeDeck();
    void collectCards();
    void shuffle();

private:

    uint mult;
    uint suits;
    QList<Card*> _deck;

    static Deck *my_deck;
};

#endif
