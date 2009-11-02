/*
 * Copyright (C) 1997 Rodolfo Borges <barrett@labma.ufrj.br>
 * Copyright (C) 1998-2009 Stephan Kulow <coolo@kde.org>
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

#include "mod3.h"

#include "carddeck.h"
#include "dealerinfo.h"
#include "hint.h"
#include "patsolve/mod3solver.h"

#include <KDebug>
#include <KLocale>


//-------------------------------------------------------------------------//

// Special pile type used for the fourth row. Automatically grabs a new card
// from the deck when emptied.
class Mod3Pile : public Pile
{
public:
    Mod3Pile( int _index, Pile * pile, const QString & objectName = QString() )
        : Pile( _index, objectName ), drawPile( pile ) {}
    virtual void relayoutCards() {
        Pile::relayoutCards();
        // Don't pull cards from the deck if the deck still contains all 104
        // cards. This prevents glitchy things from happening before the initial
        // deal has happened.
        if ( isEmpty() /*&& CardDeck::self()->cardsLeft() < 104*/ )
        {
            animatedAdd( drawPile->top(), true );
        }
    }
    Pile * drawPile;
};


Mod3::Mod3( )
    : DealerScene( )
{
    // Piles are placed very close together. Set layoutSpacing to 0 to prevent
    // interference between them.
    setLayoutSpacing(0.0);

    const qreal dist_x = 1.114;
    const qreal dist_y = 1.31;
    const qreal bottomRowY = 3 * dist_y + 0.2;
    const qreal rightColumX = 8 * dist_x + 0.8;

    // This patience uses 2 deck of cards.
    deck = new CardDeck(2);

    talon = new Pile(0, "talon");
    talon->setPilePos(rightColumX, bottomRowY);
    connect(talon, SIGNAL(clicked(Card*)), SLOT(newCards()));
    addPile(talon);

    aces = new Pile(50, "aces");
    aces->setPilePos(rightColumX, 0.5);
    aces->setTarget(true);
    aces->setCheckIndex(2);
    aces->setAddFlags(Pile::addSpread | Pile::several);
    aces->setReservedSpace( QSizeF( 1.0, 2.0 ));
    addPile(aces);

    for ( int r = 0; r < 4; r++ ) {
        for ( int c = 0; c < 8; c++ ) {
            int pileIndex = r * 10 + c  + 1;
            // The first 3 rows are the playing field, the fourth is the store.
            if ( r < 3 ) {
                stack[r][c] = new Pile ( pileIndex, QString( "stack%1_%2" ).arg( r ).arg( c ) );
                stack[r][c]->setPilePos( dist_x * c, dist_y * r );
                stack[r][c]->setCheckIndex( 0 );
                stack[r][c]->setTarget(true);
                stack[r][c]->setAddFlags( Pile::addSpread );
                // Very tight spread makes it easy to quickly tell number of
                // cards in each pile and we don't care about the cards beneath.
                stack[r][c]->setSpread( 0, 0.08 );
                stack[r][c]->setReservedSpace( QSizeF( 1.0, 1.23 ) );
            } else {
                stack[r][c] = new Mod3Pile ( pileIndex, talon, QString( "stack3_%1" ).arg( c ) );
                stack[r][c]->setPilePos( dist_x * c, bottomRowY );
                stack[r][c]->setReservedSpace( QSizeF( 1.0, 1.8 ) );
                stack[r][c]->setAddFlags( Pile::addSpread );
                stack[r][c]->setCheckIndex( 1 );
            }
            addPile(stack[r][c]);
        }
    }

    setActions(DealerScene::Hint | DealerScene::Demo  | DealerScene::Deal);
    setSolver( new Mod3Solver( this ) );
}

bool Mod3::checkAdd( int checkIndex, const Pile *c1, const CardList& cl) const
{
    if (checkIndex == 0) {
        Card *c2 = cl.first();

        if (c1->isEmpty())
            return (c2->rank() == ( ( c1->index() / 10 ) + 2 ) );

        if (c1->top()->suit() != c2->suit())
            return false;

        if (c2->rank() != (c1->top()->rank()+3))
            return false;

        if (c1->cardsLeft() == 1)
            return (c1->top()->rank() == ((c1->index() / 10) + 2));

        return true;

    } else if (checkIndex == 1) {
        return c1->isEmpty();

    } else if (checkIndex == 2) {
        return cl.first()->rank() == Card::Ace;

    } else
        return false;
}


bool Mod3::checkPrefering( int checkIndex, const Pile *c1, const CardList& c2) const
{
    return (checkIndex == 0 && c1->isEmpty()
            && c2.first()->rank() == (c1->index()+1));
}


//-------------------------------------------------------------------------//


void Mod3::restart()
{
    deck->returnAllCards();
    deck->shuffle( gameNumber() );
    deal();
    emit newCardsPossible(true);
}


//-------------------------------------------------------------------------//


void Mod3::dealRow(int row)
{
    if (talon->isEmpty())
        return;

    for (int c = 0; c < 8; c++) {
        Card *card = talon->top();
        stack[row][c]->animatedAdd (card, true);
    }
}


//-------------------------------------------------------------------------//


void Mod3::deal()
{
    setMarkedItems();
    deck->takeAllCards(talon);

    for (int r = 0; r < 4; r++)
        dealRow(r);
}

Card *Mod3::newCards()
{
    if (talon->isEmpty())
        return 0;

    setMarkedItems();
    dealRow(3);
    takeState();
    considerGameStarted();
    if (talon->isEmpty())
        emit newCardsPossible(false);

    return stack[3][0]->top();
}

void Mod3::setGameState(const QString &)
{
    emit newCardsPossible(!talon->isEmpty());
}



static class Mod3DealerInfo : public DealerInfo
{
public:
    Mod3DealerInfo()
      : DealerInfo(I18N_NOOP("Mod3"), Mod3Id)
    {}

    virtual DealerScene *createGame() const
    {
        return new Mod3();
    }
} mod3DealerInfo;


#include "mod3.moc"
