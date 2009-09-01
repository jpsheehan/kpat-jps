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

#ifndef KLONDIKE_H
#define KLONDIKE_H

#include "dealer.h"
class KlondikePile;

class KSelectAction;


class Klondike : public DealerScene {
    friend class KlondikeSolver;

    Q_OBJECT

public:
    explicit Klondike();

    virtual void mapOldId(int id);
    virtual void restart();
    virtual bool startAutoDrop();
    QList<QAction*> configActions() const;

    void redeal(); // put pile back into deck
    void deal();

    void setEasy( bool easy );

public slots:
    virtual Card *newCards();

protected:
    virtual QString getGameState();
    virtual QString getGameOptions() const;
    virtual void setGameOptions(const QString &options);

private:
    bool EasyRules;
    bool redealt;

    KSelectAction *options;

    Pile* play[7];
    Pile* target[4];

    KlondikePile *pile;
    Card::Rank target_tops[4];
    bool noLongerNeeded(Card::Rank r, Card::Suit s );

private slots:
    void gameTypeChanged();
};

class KlondikePile : public Pile
{
public:
    KlondikePile( int _index, int _draw, DealerScene* parent);
    int draw() const { return m_draw; }
    void setDraws( int _draw );

protected:
    virtual void layoutCards( int duration );

private:
    int m_draw;
};

#endif
