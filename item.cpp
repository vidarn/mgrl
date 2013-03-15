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
}

Item::~Item()
{
    for(int i=0;i<m_inventory.size();i++){
        delete m_inventory[i];
    }
}

void
Item::act()
{
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
        addTag(ITEM_MELEE);addTag(ITEM_WEAPON);addTag(ITEM_PICK_UP);}
    if(name == "ranged" && val.b){
        addTag(ITEM_RANGED);addTag(ITEM_WEAPON);addTag(ITEM_PICK_UP);}
    if(name == "helmet" && val.b){
        addTag(ITEM_HELMET);addTag(ITEM_ARMOR);addTag(ITEM_PICK_UP);}
    if(name == "body_armor" && val.b){
        addTag(ITEM_BODY_ARMOR);addTag(ITEM_ARMOR);addTag(ITEM_PICK_UP);}
    if(name == "gloves" && val.b){
        addTag(ITEM_GLOVES);addTag(ITEM_ARMOR);addTag(ITEM_PICK_UP);}
    if(name == "boots" && val.b){
        addTag(ITEM_BOOTS);addTag(ITEM_ARMOR);addTag(ITEM_PICK_UP);}
    if(name == "shield" && val.b){
        addTag(ITEM_SHIELD);addTag(ITEM_ARMOR);addTag(ITEM_PICK_UP);}
    if(name == "scroll" && val.b){
        addTag(ITEM_SCROLL);addTag(ITEM_READ);addTag(ITEM_PICK_UP);}
    if(name == "potion" && val.b){
        addTag(ITEM_POTION);addTag(ITEM_QUAFF);addTag(ITEM_PICK_UP);}
    if(name == "minor_heal_potion" && val.b){
        addTag(POTION_MINOR_HEAL);}
    if(name == "heal_potion" && val.b){
        addTag(POTION_HEAL);}
    if(name == "major_heal_potion" && val.b){
        addTag(POTION_MAJOR_HEAL);}
    if(name == "container"){
        addTag(ITEM_OPEN);addTag(ITEM_CONTAINER);
        for ( char ** iterator = (char **)TCOD_list_begin(val.list); iterator != (char **)TCOD_list_end(val.list); iterator ++ ) {
            char *currentValue=*iterator;
            m_inventoryStrings.push_back(currentValue);
        }
    }
    if(name == "damage"){
        m_damageNumDice = val.dice.nb_rolls;
        m_damageDie = val.dice.nb_faces;
    }
    if(name == "ac"){
        m_acBonus = val.i;
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
    for(int i=0;i<m_inventoryStrings.size();i++){
        Actor *actor = m_level->m_actorFactory.getActor(m_inventoryStrings[i],level);
        if(actor != 0){
            m_inventory.push_back(actor);
        }
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
                for(int i=0;i<m_inventory.size();i++){
                    itemList.push_back(ListDefinition(LIST_ENTRY, k++ , m_inventory[i]->m_name));
                }
                ListOverlay itemOverlay(30, 30, "Take what?", true, itemList);
                itemOverlay.main(m_level);
                for(int i=0;i<itemOverlay.m_definition.size();i++){
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
}

void
Item::quaff(Actor *drinker)
{
    bool showMessage = true;
    std::string msg = "The ";
    msg += m_name;
    if(hasTag(ITEM_QUAFF)){
        if(hasTag(POTION_MINOR_HEAL)){
            drinker->healDamage(20,this);
            msg += " replenishes your health";
        }
        if(hasTag(POTION_HEAL)){
            drinker->healDamage(70,this);
        }
        if(hasTag(POTION_MAJOR_HEAL)){
            drinker->healDamage(150,this);
        }
    }
    if(showMessage)
        m_level->m_messages->showMessage(msg,MESSAGE_NOTIFICATION);
}

