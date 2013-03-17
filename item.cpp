#include <cstdio>
#include <boost/random/uniform_int_distribution.hpp>
#include "item.h"
#include "level.h"
#include "list_overlay.h"


Item::Item()
	:Actor(),
    m_acBonus(0),m_armorCheckPenalty(0),
    m_enchantment(0),m_damageDie(3),
    m_damageNumDice(1)
{
	addTag(TAG_ITEM);
	addTag(TAG_REMEMBER);
    m_speed = 100.0f;
}

Item::~Item()
{
    for(unsigned int i=0;i<m_inventory.size();i++){
        delete m_inventory[i];
    }
}

void
Item::act(float time)
{
    m_time += m_speed*time;
    if(hasTag(TRAP)){
        if(hasTag(TRAP_BLAST)){
            bool exploded = false;
            std::vector<int> tags;
            tags.push_back(TAG_CREATURE);
            std::vector<Actor *> actors = m_level->getActors(m_x,m_y,tags);
            for(unsigned int i=0;i<actors.size();i++){
                Actor *actor = actors[i];
                if(!actor->hasTag(TAG_PLAYER)){
                    exploded = true;
                }
            }
            if(exploded){
                std::string msg = "The ";
                msg += m_name;
                msg += " explodes";
                m_level->m_messages->showMessage(msg, MESSAGE_NOTIFICATION);
                m_level->removeActor(this);
                for(unsigned int i=0;i<actors.size();i++){
                    Actor *actor = actors[i];
                    if(!actor->hasTag(TAG_PLAYER)){
                        int damage = 0;
                        for(int i=0; i<2+m_level->m_player->m_hd;i++){
                            damage += d6(RAND);
                        }
                        if(!actor->takeDamage(damage, DAMAGE_PHYSICAL, this)){
                            m_level->killActor(actor,this);
                        }
                    }
                }
                Player *player= m_level->m_player;
                for(unsigned int i=0;i<player->m_abilities.size();i++){
                    Ability *ab = player->m_abilities[i];
                    if(ab->m_id == AB_BLAST_TRAP && ab->m_active){
                        AbBlastTrap *bt = static_cast<AbBlastTrap *>(ab);
                        if(bt->m_trap == this){
                            player->invokeAbility(i);
                        }
                    }
                }
            }
        }
    }
}

void
Item::playerSpotted()
{
    m_discovered = true;
}

void
Item::handleProperty(std::string &name, TCOD_value_t &val)
{
    if(name == "hp"){
        m_hp = val.i;
    }
    if(name == "melee" && val.b){
        addTag(ITEM_MELEE);addTag(ITEM_WEAPON);addTag(ITEM_PICK_UP);addTag(TAG_TRANSPARENT);addTag(TAG_WALKABLE);
    }
    if(name == "ranged" && val.b){
        addTag(ITEM_RANGED);addTag(ITEM_WEAPON);addTag(ITEM_PICK_UP);addTag(TAG_TRANSPARENT);addTag(TAG_WALKABLE);
    }
    if(name == "ammo_bow" && val.b){
        addTag(ITEM_AMMO_BOW);addTag(ITEM_WEAPON);addTag(ITEM_PICK_UP);addTag(ITEM_STACK);addTag(ITEM_QUIVER);addTag(TAG_TRANSPARENT);addTag(TAG_WALKABLE);
    }
    if(name == "helmet" && val.b){
        addTag(ITEM_HELMET);addTag(ITEM_ARMOR);addTag(ITEM_PICK_UP);addTag(TAG_TRANSPARENT);addTag(TAG_WALKABLE);
    }
    if(name == "body_armor" && val.b){
        addTag(ITEM_BODY_ARMOR);addTag(ITEM_ARMOR);addTag(ITEM_PICK_UP);addTag(TAG_TRANSPARENT);addTag(TAG_WALKABLE);
    }
    if(name == "gloves" && val.b){
        addTag(ITEM_GLOVES);addTag(ITEM_ARMOR);addTag(ITEM_PICK_UP);addTag(TAG_TRANSPARENT);addTag(TAG_WALKABLE);
    }
    if(name == "boots" && val.b){
        addTag(ITEM_BOOTS);addTag(ITEM_ARMOR);addTag(ITEM_PICK_UP);addTag(TAG_TRANSPARENT);addTag(TAG_WALKABLE);
    }
    if(name == "shield" && val.b){
        addTag(ITEM_SHIELD);addTag(ITEM_ARMOR);addTag(ITEM_PICK_UP);addTag(TAG_TRANSPARENT);addTag(TAG_WALKABLE);
    }
    if(name == "scroll" && val.b){
        addTag(ITEM_SCROLL);addTag(ITEM_READ);addTag(ITEM_PICK_UP);addTag(TAG_TRANSPARENT);addTag(TAG_WALKABLE);
    }
    if(name == "potion" && val.b){
        addTag(ITEM_POTION);addTag(ITEM_QUAFF);addTag(TAG_TRANSPARENT);addTag(TAG_WALKABLE);addTag(ITEM_PICK_UP);addTag(ITEM_STACK);
    }
    if(name == "sacred_nectar" && val.b){
        addTag(POTION_SACRED_NECTAR);
    }
    if(name == "heal_potion" && val.b){
        addTag(POTION_HEAL);
    }
    if(name == "mana_potion" && val.b){
        addTag(POTION_MANA);
    }
    if(name == "minor" && val.b){
        addTag(TAG_MINOR);
    }
    if(name == "medium" && val.b){
        addTag(TAG_MEDIUM);
    }
    if(name == "major" && val.b){
        addTag(TAG_MAJOR);
    }
    if(name == "container"){
        addTag(ITEM_OPEN);addTag(ITEM_CONTAINER);
        /*for ( char ** iterator = (char **)TCOD_list_begin(val.list); iterator != (char **)TCOD_list_end(val.list); iterator ++ ) {
            char *currentValue=*iterator;
            m_inventoryStrings.push_back(currentValue);
        }*/
    }
    if(name == "not_potion_name" && val.b){
        addTag(POTION_NOT_NAME);}
    if(name == "damage"){
        m_damageNumDice = val.dice.nb_rolls;
        m_damageDie = val.dice.nb_faces;
    }
    if(name == "ac"){
        m_acBonus = val.i;
    }
    if(name == "door" && val.b){
        addTag(ITEM_DOOR);addTag(ITEM_BLEND_IN);
    }
    if(name == "transparent" && val.b){
        addTag(TAG_TRANSPARENT);
    }
    if(name == "walkable" && val.b){
        addTag(TAG_WALKABLE);
    }
    if(name == "blend_in" && val.b){
        addTag(ITEM_BLEND_IN);
    }
    if(name == "stairs_down" && val.b){
        addTag(ITEM_STAIRS_DOWN);
    }
    if(name == "stairs_up" && val.b){
        addTag(ITEM_STAIRS_UP);
    }
    if(name == "shard" && val.b){
        addTag(ITEM_SHARD_OF_KNOWLEDGE);
    }
    if(name == "stack" && val.b){
        addTag(ITEM_STACK);
    }
    if(name == "blast_trap" && val.b){
        addTag(TAG_TRANSPARENT);addTag(TAG_WALKABLE);addTag(TRAP);addTag(TRAP_BLAST);m_speed=0.01f;
    }
}

void
Item::handleTag(std::string &tag)
{
}

void
Item::finish(Level *level)
{
    m_level = level;
    for(unsigned int i=0;i<m_inventoryStrings.size();i++){
        Actor *actor = m_level->m_actorFactory.getActor(m_inventoryStrings[i],level);
        if(actor != 0){
            m_inventory.push_back(actor);
        }
    }
    if(hasTag(ITEM_CONTAINER)){
        double numItemProbabilities[] = {
            1.0,
            2.0,
            0.5,
            0.3,
            0.1
        };
        boost::random::discrete_distribution<> numItemDist(numItemProbabilities);
        int numItems = numItemDist(RAND);
        std::vector<std::string> tags;
        tags.push_back("pickUp");
        for(int i=0;i<numItems;i++){
            Actor *item = level->m_actorFactory.getItem(level->m_dungeonLevel, level, tags);
            if(item != 0){
                m_inventory.push_back(item);
            }
        }

    }
    if(hasTag(ITEM_POTION) && !hasTag(POTION_NOT_NAME)){
        std::string tmp = m_name;
        m_name = "Potion of ";
        m_name += tmp;
    }
}

void
Item::open(Actor *opener)
{
    if(opener->hasTag(TAG_PLAYER)){
        if(hasTag(ITEM_CONTAINER)){
            std::string msg = opener->m_name;
            msg += " open the ";
            msg += m_name;
            m_level->m_messages->showMessage(msg,MESSAGE_ACTION);
            if(m_inventory.size() > 0){
                std::vector<ListDefinition>itemList;
                char k = 'a';
                for(unsigned int i=0;i<m_inventory.size();i++){
                    itemList.push_back(ListDefinition(LIST_ENTRY, k++ , m_inventory[i]->m_name));
                }
                ListOverlay itemOverlay(30, 30, "Take what?", true, true, itemList);
                itemOverlay.main(m_level);
                for(unsigned int i=0;i<itemOverlay.m_definition.size();i++){
                    ListDefinition &def = itemOverlay.m_definition[i];
                    if(def.m_selected){
                        char val = def.m_key - 'a';
                        Actor *item = m_inventory[val-i];
                        msg = opener->m_name;
                        msg += " take a ";
                        msg += item->m_name;
                        m_level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
                        opener->pickUp(item);
                        m_inventory.erase(m_inventory.begin()+val-i);
                    }
                }
            }
            else{
                msg = "The ";
                msg += m_name;
                msg += " is empty";
                m_level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
            }
        }
    }
    if(hasTag(ITEM_DOOR)){
        if(!hasTag(TAG_OPEN)){
            addTag(TAG_OPEN);
            addTag(TAG_WALKABLE);
            addTag(TAG_TRANSPARENT);
            m_glyph++;
            std::string tmp = m_name;
            m_name = "Open ";
            m_name += tmp;
        }
    }
}

void
Item::quaff(Actor *drinker)
{
    bool showMessage = false;
    std::string msg = "The ";
    msg += m_name;
    if(hasTag(ITEM_QUAFF)){
        if(hasTag(POTION_HEAL)){
            if(hasTag(TAG_MINOR)){
                drinker->healDamage(drinker->m_maxHp/4,this);
            }
            if(hasTag(TAG_MEDIUM)){
                drinker->healDamage(drinker->m_maxHp/2,this);
            }
            if(hasTag(TAG_MAJOR)){
                drinker->healDamage(drinker->m_maxHp,this);
            }
            msg += " replenishes your health";
            showMessage = true;
        }
        if(hasTag(POTION_MANA)){
            if(drinker->hasTag(TAG_CREATURE)){
                Creature *c = (Creature *)drinker;
                c->regainMana(5,COLOR_RED);
                c->regainMana(5,COLOR_BLUE);
                c->regainMana(5,COLOR_WHITE);
                if(drinker->hasTag(TAG_PLAYER)){
                    msg += " replenishes your mana";
                    showMessage = true;
                }
            }
        }
    }
    if(showMessage)
        m_level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
}

