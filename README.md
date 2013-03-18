MGRL
====

>"Welcome to the deep and deadly dungeons of Svogthos.
You are an aspiring planeswalker in search of the Orb of Knowledge, known to lie deep under ground."

MGRL is a roguelike game written for the 7day Roguelike Challenge 2013. The objective of the game is to get to dungeon level 15 and obtaining the Orb of knowledge.

*The most up-to-date versions of the game and this document can be found at https://github.com/vidarn/mgrl*

##Downloads

Binaries for Windows and 64-bit Linux are available from [here](http://dl.dropbox.com/u/1618340/MGRL/MGRL.zip) (same zip file for both platforms).
This is the 7drl-version of the game with some post-compo bug fixes. No new features have been implemented however.

There is a post-compo version of the game called MGRL-mini. It's faster and more action packed. It is also the recommended version for people with smaller screen sizes.  
MGRL-mini can be downloaded from [here](http://dl.dropbox.com/u/1618340/MGRL/MGRL-mini.zip) (Windows and 64-bit Linux).

##Instructions

In-game instructions are rather sparse, so please read these to learn the basics of the game.

###Colors of Magic

There are three colors of magic, *Red*, *Blue* and *White*. Each color has three spells (one of them being a *Mana Stream*).

###Spells

On the right side of the screen (in the *Status* box) you will see three spell entries, each containing a name (e.g. Lightning Bolt) and a Mana cost (e.g. 1 red Mana).  
By pressing the corresponding number key (1,2 or 3) You will invoke (cast) the spell.  
When a spell has been used a new random spell appears in its place. Note that this new spell is random and therefore might be the same spell that you just had in that slot.  
There are three types of spells.  

* *Mana stream*.
    * Invoking this spell will activate it (it becomes a darker shade of color to indicate that it's active).
    * While active, the *Mana Stream* will replenish your Mana at regular intervals.
    * Apart from picking up a *Shard of Knowledge*, *Mana Stream* is your **only way of regaining Mana**.
    * To deactivate the *Mana stream*, simply press the number key again. This will place a random new spell in the slot.
* *Instant* Spells.
    * Invoking an *instant* spell will have an effect (such as Lightning Bolt dealing damage to an enemy) and then immediately place a random new spell in the slot.
* *Continuous* Spells.
    * Just like with *Mana Streams*, invoking this spell will activate it (it becomes a darker shade of color to indicate that it's active).
    * While active, the *Continous* spell will decrease your maximum Mana by the cost of the spell. You will regain the Mana when the spell is deactivated.
    * To deactivate a *Continous* spell, simply press the number key again. This will place a random new spell in the slot.
    * A *Continous* spell will conjure an item when activated (such as Sacred Nectar placing a potent healing potion in your inventory). This item will disappear when the spell is deactivated.

###Controls

* 1,2,3 - invoke spells
* h,j,k,l - move/attack straight
* y,u,b,n - move/attack diagonally
* i - inventory
* q - drink
* g - pick up
* Q - quiver
* f - fire (ranged weapon)
* o - open (container, doors open by just bumping in to them)
* w - wield
* d - drop
* \> - descend stairs
* \< - ascend stairs

##Beginner tips

Playing a multi-color character can be challenging. For your first characters, try focusing on one color.  
Blue is probably the easiest color to start with. Both spells are cheap and highly useful. Blast Trap deals huge amounts of damage and Risky Displacement is a great panic button.  
New players are adviced to try a Vedalken character, as they start with several levels of blue mana.  
If you find that the game is difficult, remember that you start with a bow, and don't be afraid to use your spells and any potions you have.

##Build instructions

MGRL depends on [libtcod](http://doryen.eptalys.net/libtcod/) and boost::random. 
Note that boost::random is a header-only part of boost, and you don't need to compile boost to use it.  
I have successfully built the game for linux using g++ and for windows using mingw. With minor modifications of search paths, the Makefiles should work for you too.  
Don't hesitate to contact me if you have any questions.

##Contact
If you have any questions, complaints or comments, feel free to contact me at vidar.nelson@gmail.com or tweet me at @vidarn.
