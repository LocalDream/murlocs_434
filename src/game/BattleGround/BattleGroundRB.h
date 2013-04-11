/*
 * Copyright (C) 2009-2013 300murlocs <http://300murlocs.com/>
 * Copyright (C) 2005-2012 MaNGOS <http://getmangos.com/> 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __BATTLEGROUNDABG_H
#define __BATTLEGROUNDABG_H

class BattleGround;

class BattleGroundABGScore : public BattleGroundScore
{
    public:
        BattleGroundABGScore() {};
        virtual ~BattleGroundABGScore() {};
};

class BattleGroundRB : public BattleGround
{
    friend class BattleGroundMgr;

    public:
        BattleGroundRB();
        ~BattleGroundRB();
        void Update(uint32 diff) override;

        /* inherited from BattlegroundClass */
        virtual void AddPlayer(Player *plr) override;
        virtual void StartingEventCloseDoors() override;
        virtual void StartingEventOpenDoors() override;

        void RemovePlayer(Player *plr, ObjectGuid guid) override;
        void HandleAreaTrigger(Player *Source, uint32 Trigger) override;
        //bool SetupBattleGround();

        /* Scorekeeping */
        void UpdatePlayerScore(Player *Source, uint32 type, uint32 value) override;

    private:
};
#endif
