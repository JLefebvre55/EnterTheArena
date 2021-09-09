ENTER THE ARENA

USER MANUAL


Table of Contents:
------------------
1. Preface

2. Installing & Running

3. Compiling

4. Running

5. Gameplay

6. Known Bugs


----------------------------
1. Preface
----------------------------

This game was written in Ansi C, originally for a 80486 clone PC, and compiled using Borland Turbo C V2.01. Sound is achieved via PC Speaker.

Enter The Arena was written in two weeks, but don't tell my teacher that.	
	
I still don't know what a pointer is. Edit 2021: In now know what a pointer is, and they're super cool.

Copyright 2018-2021, Jayden Lefebvre.

----------------------------
2. Environment Setup
----------------------------

1. Download and install DOSBox from https://www.dosbox.com/.

2. Run DOSBox.

3. To mount a directory to drive C:\, run "mount c: DIRECTORY", with DIRECTORY being replaced by the full path to the directory on the host machine.

4. Navigate to this mounted drive by running "c:".

----------------------------
3. Compiling
----------------------------

NOTE: A DOSBox-ready version of the ETA executable is included with this repository. You can skip to section 4.

1. Download a set of Borland Turbo C 1.0 5.25" floppy images from https://winworldpc.com/product/borland-turbo-c/1x

2. Extract all the images, and then extract the contents of each image into separate folders.

3. Run DOSBox.

4. Refer to the README inside IDE.img for instructions on how to install and use Borland Turbo C in DOSBox, mounting the image directories as needed (as opposed to inserting floppy discs).

5. Open ETA.C and compile.

----------------------------
4. Running
----------------------------

1. Mount and navigate to the ETA.exe executable.

2. Run "eta" to start the game.

----------------------------
5. Gameplay
----------------------------

The game starts. The intro screen is shown, and a cool jingle is played.

The main menu is shown. The user can select between 3 save slots. If the user chooses an empty slot, they are offered the choice to start a new game in that slot. If the user chooses an occupied slot, they can either rename the save, delete the save, or start that save game. The main menu is active until the user starts a game.

The menu screen offers five options. The user can enter the arena, go to the shop, display their stats, save, or exit. Options 3 through 5 are self-explanatory.

Entering the arena generates an enemy from a random list of possible enemies and places it within 2 levels above or below the player's current level. The user can then choose to use their sword (basic attack, 1 in 15 chance to miss or critical strike), cast magic (takes 1 mana, 1 in 10 chance to fail), use a health or mana potion (restores either stat to full), or run away (dropping some gold upon fleeing). The player makes their choice, and then the computer-controlled enemy attacks. Gameplay continues in this fashion until either the player or the enemy are dead.

Entering the shop offers four options. The user can buy a health or mana potion to be used in the arena (10 gold each), or a scratch card (5 gold). The scratch card has three randomly generated icons, each having a 1 in 3 chance of being either Dragon, Sword, or Gold. Getting two golds awards the player 10 gold. Getting all three golds awards the user 20 gold. Both of these also refund the scratch ticket price. Any other combination will result in a loss.

The game has no end, no finish. It is a battle-oriented open-ended text-based dungeon arena game. Pretty neat.

----------------------------
6. Known Bugs
----------------------------

1. Inputting a non-integer character when prompted for an integer causes the game to loop infinitely. This is due to a bug in ANSI C when using the function "scanf()" and passing a formatting string requesting an integer.

2. It is unknown how the game will function if the player inputs a name longer than 20 characters. That is the hardcoded max limitation.
