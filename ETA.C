/*
 ============================================================================
 Name        : ETA.c
 Author      : Jayden Lefebvre
 Description : Cool game. By me.
 ============================================================================
 */

/**INCLUDES**/
#include "ETA.H"

/**MACROS**/
#define NUM_SAVES 3
#define MAX_NAME 40
#define NUM_ATTACKS 3
#define NUM_ENEMIES 11
#define STR_SAVEFILE "SAVE%d.TXT"
#define STR_LINEBREAK "-----------------------------------------------------------------------------\n"

/** DECLARATIONS **/

// PLAYER STATE
typedef struct {
	unsigned int slot; // Save slot
	char name[MAX_NAME+1];
	unsigned int level;
	unsigned int gold;

	unsigned int health;
	unsigned int maxhealth;

	unsigned int mana;
	unsigned int maxmana;

	unsigned int healthpotions;
	unsigned int manapotions;

	unsigned int maxdamage;
	unsigned int mindamage;
} Player;

// ENEMY STATE
typedef struct {
	char name[MAX_NAME+1];
	unsigned int level;

	unsigned int health;
	unsigned int maxhealth;

	char attacks[NUM_ATTACKS][MAX_NAME + 1];
	unsigned int maxdamage;
	unsigned int mindamage;
} Enemy;

// File I/O
int parseSave(Player& player); 
int deleteSave(Player& savegame);
int saveGame(Player& savegame);

// Game States
void initPlayer(Player& player);
Enemy parseEnemy(int playerlevel);

// Game Menues
Player loadMenu(void); // Main menu; returns selected player (fetched from save file slot)
int startArena(Player& player);
Player shop(Player player);

/**MAIN**/
int main(void) {
	Player player; int r;
	srand(time(NULL)); /** seed the RNG **/

	while(1){

		/** Print title **/
		clrscr();
		printf(STR_LINEBREAK);
		printf(
				"\n _____      _            _____ _             ___                       \n"
				"|  ___|    | |          |_   _| |           / _ \\                      \n"
				"| |__ _ __ | |_ ___ _ _   | | | |__   ___  / /_\\ \\_ __ ___ _ __   __ _ \n"
				"|  __| '_ \\| __/ _ \\ '__| | | | '_ \\ / _ \\ |  _  | '__/ _ \\ '_ \\ / _` |\n"
				"| |__| | | | ||  __/ |    | | | | | |  __/ | | | | | |  __/ | | | (_| |\n"
				"\\____/_| |_|\\__\\___|_|    \\_/ |_| |_|\\___| \\_| |_/_|  \\___|_| |_|\\__,_|\n\n");
		printf(STR_LINEBREAK);

		/** cool intro sounds **/
		sound(262);
		delay(900);
		sound(294);
		delay(300);
		sound(311);
		delay(600);
		sound(262);
		delay(600);
		sound(370);
		delay(1200);
		sound(392);
		delay(1200);
		nosound();
		sleep(1);

		/** Get player values **/
		player = loadMenu();
		initPlayer(player); // reset

		if(player.level == 1){	/** if new player, run tutorial **/
			clrscr();
			printf(STR_LINEBREAK);
			printf("\t\t\t\tINTRO\n");
			printf(STR_LINEBREAK);
			printf(	"You awaken in a small dungeon cell. The floor is a cold cobblestone. \nDry, yet bone-chilling. A dim light slips through a small barred window.\n\n\"Speaking of bone-chilling...\"\nyou mutter, noticing the "
					"skeleton sitting upright against the cell wall.\n\nIn his undead hands, he grasps a shining sword and a leather-bound spellbook.\n"
					"You take them, and as you exit through the open cell door, you hear a BOOM!, \nfollowed by a feral roar in the distance.\n\n"
					"You emerge into what seems to be an empty Roman-style ampitheater, and you are \ngreeted by a roaring crowd.\n");
			getch();
			
			// DO ARENA
			startArena(player);

			initPlayer(player); // reset
		}

		while (1) { /** MAIN GAME STATE LOOP **/
			int choice;

			/** Get player input **/
			clrscr();
			printf(STR_LINEBREAK);
			printf("\t\t\t\tMENU\n");
			printf(STR_LINEBREAK);
			printf("\nReady up, %s! What would you like to do?\n"
					"\t1. Enter the Arena!\n"
					"\t2. Go to Shop\n"
					"\t3. Show Stats\n"
					"\t4. Save Game\n"
					"\t5. Exit Game\n", player.name);
			do{
				printf(">");
				scanf(" %d", &choice);

			} while (choice < 1 || choice > 5);

			if(choice == 1){

				// DO ARENA
				r = startArena(player);

				initPlayer(player); // reset

				printf("You have exited the arena.\n");
				sleep(2);
			} else if(choice == 2){         /** Go to Shop **/
				player = shop(player);
			} else if (choice == 3){		/**Print stats**/
				clrscr();
				printf(STR_LINEBREAK);
				printf("\t\t\t\t%s's STATS\n", player.name);
				printf(STR_LINEBREAK);
				printf("Max Health\t\t%d\nMax Mana\t\t%d\nEnemies Obliterated\t%d\nCoins Grabbed\t\t%d\nCool Factor\t\t%d\n\n", player.maxhealth, player.maxmana, player.level, player.gold, rand()%1000);
				getch();
			}else if(choice == 4){         /** Save Game **/
				printf("Saving...\n");
				while(saveGame(player)); // try save until return 0
				sleep(1);
				printf("Game saved successfully.\n");
				sleep(1);
			} else if (choice == 5){        /** Exit Game **/
				char save; /** Save? **/

				clrscr();
				printf(STR_LINEBREAK);
				printf("\t\t\t\tQUIT\n");
				printf(STR_LINEBREAK);
				printf("\n'Shouldn't I SAVE before quitting? I will lose all unsaved progress!' [Y/N]\n");
				do{
					printf(">");
					scanf(" %c", &save);
				} while (save != 'Y' && save != 'N' && save != 'y' && save != 'n');
				if(save == 'Y' || save == 'y'){
					printf("Saving...\n");
					while(saveGame(player)); // try save until return 0
					sleep(1);
					printf("Game saved successfully.\n");
					sleep(1);
				}
				printf("Goodbye %s.\n\n", player.name);
				sleep(1);
				return 0;
			}
		}
	}
}

/**FUNCTION DECLARATIONS**/

/**
 * Shop menu
 */
Player shop(Player player){

	clrscr();
	printf("Welcome to the Shop!");

	/** some sound **/

	sound(659);
	delay(1100);
	sound(523);
	delay(1100);
	nosound();

	while(1){       /** Main shop state loop **/

		int choice;

		clrscr();
		printf(STR_LINEBREAK);
		printf("\t\tSHOP\n");
		printf(STR_LINEBREAK);
		/** Get player input **/
		printf("\nWhat would you like to do? You have %d Gold.\n"
				"\t1. Buy Health Potion - Full HP - 10 Gold\n"
				"\t2. Buy Mana Potion - Full Mana - 10 Gold\n"
				"\t3. Buy Scratch Card - Scratch to Win - 5 Gold\n"
				"\t4. Exit Shop\n", player.gold);
		do{
			printf(">");
			scanf(" %d", &choice);


		} while (choice < 1 || choice > 4);

		if(choice == 1){        /** Health potion **/
			if(player.gold >=10){
				player.healthpotions++;
				player.gold-=10;
				printf("'I now have %d health potions.'\n", player.healthpotions);
			} else {
				printf("'I don't have enough gold...'\n");
			}
		} else if(choice == 2){         /** Mana potion **/
			if(player.gold >=5){
				player.manapotions++;
				player.gold-=5;
				printf("'I now have %d mana potions.'\n", player.manapotions);
			} else {
				printf("'I don't have enough gold...'\n");
			}
		} else if(choice == 3){         /** Scratch Card **/
			if(player.gold >=5){
				int x,y,z;
				player.gold-=5;
				printf("You pick up a scratch card and begin scratching.\n");
				sleep(1);

				/** Generate 3 ints 0-2, printf corresponding item and flush **/
				x = rand()%3;
				y = rand()%3;
				z = rand()%3;
				printf(x == 0 ? "'Gold... " : x == 1 ? "Dragon... " : "Sword... ");
				fflush(stdout);
				sleep(1);
				printf(y == 0 ? "Gold... " : y == 1 ? "Dragon... " : "Sword... ");
				fflush(stdout);
				sleep(1);
				printf(z == 0 ? "Gold!'\n" : z == 1 ? "Dragon!'\n" : "Sword!'\n");
				sleep(1);

				/** Check for combos. 2 gold = 10, 3 gold = 20 **/
				if(x == 0 && y == 0 && z == 0){
					player.gold += 25;
					printf("'Three Golds! Score!!! I won 20 gold.'\n");
				} else if ((x == 0 && y == 0) || (y == 0 && z == 0) || (x == 0 && z == 0)){
					player.gold += 15;
					printf("'Two Golds! I won 10 gold.'\n");
				} else {
					printf("'Aw dangit... I Lost again.'\n");
				}


			} else {
				printf("Not enough gold!\n");
			}
		} else if (choice == 4){        /** Exit **/
			printf("Thank you; come again!\n");
			sleep(2);
			break;
		}

		sleep(2);
	}
	return player;
}

/**
 * ARENA TIME!!
 */
int startArena(Player& player){
	Enemy enemy;
	clrscr();
	printf("Welcome to the Arena, %s.\n\n", player.name);

	/** some sound **/
	sound(392);
	delay(200);
	sound(587);
	delay(1500);
	nosound();

	enemy = parseEnemy(player.level);       /** Get enemy **/
	printf("A level %d %s has entered the arena!\n", enemy.level, enemy.name);
	sleep(2);

	while(1){       /** Main arena state loop **/
		int endedturn = 0;

		do{ /** while player's turn **/

			int action;

			/** STATUS HEADER **/
			clrscr();
			printf(STR_LINEBREAK);
			printf("| LVL %d PLAYER - HP: %d/%d, MANA: %d/%d\t|\tLVL %d ENEMY - HP:%d/%d |\n", player.level, player.health, player.maxhealth, player.mana, player.maxmana, enemy.level, enemy.health, enemy.maxhealth);
			printf(STR_LINEBREAK);

			printf("\n");



			/** Get player input **/
			printf("\nWhat do you do?\n1-Use Sword, 2-Cast Magic, 3-Health Potion (%d), 4-Mana Potion (%d), 5-Run Away\n", player.healthpotions, player.manapotions);
			fflush(stdin);
			do{
				printf(">");
				scanf(" %d", &action);
			} while(action < 1 || action > 5);

			if(action == 1){        /** sword **/
				int damage, random;

				random = rand();
				if(random%15==0){       /** Critical strike! **/
					damage = ceil(player.maxdamage*1.5);
					printf("Critical strike! ");
				} else if(random%15==1){        /**Dodge**/
					damage = 0;
					printf("The %s dodged! ", enemy.name);
				}else { /**Normal**/
					damage = rand()%(player.maxdamage+1)+(player.mindamage);
					if(damage<1){
						damage=1;
					}
				}
				enemy.health-=damage;
				printf("%d damage dealt.\n", damage);
				endedturn = 1;
			} else if (action == 2){        /** cast magic. maxdamage*2 if has mana**/

				int damage;
				if(player.mana > 0){
					if(rand()%10==0){
						player.mana--;
						printf("Your spell failed! 0 damage dealt.\n");
					} else {
						damage = rand()%(player.maxdamage+2)+2;
						enemy.health-=damage;
						player.mana--;
						printf("Casted magic at the %s! %d damage dealt.\n", enemy.name, damage);
					}
					endedturn = 1;
				} else {
					printf("Not enough mana.\n");
				}
			} else if (action == 3){        /** health potion. full hp if has one or more **/
				if(player.health == player.maxhealth){
					printf("Your health is already full!\n");
				} else {
					if(player.healthpotions>0){
						player.healthpotions--;
						player.health = player.maxhealth;
						printf("Yum! Full health restored.\n");
					} else {
						printf("You have no health potions!\n");
					}
				}
			} else if (action == 4){        /** mana potion. full mana if has one or more **/
				if(player.mana == player.maxmana){
					printf("Your mana is already full!\n");
				} else {
					if(player.manapotions>0){
						player.manapotions--;
						player.mana = player.maxmana;
						printf("*Gulp* Full mana restored.\n");
					} else {
						printf("You have no mana potions!\n");
					}
				}
			} else if (action == 5){        /** flee. drop 6>=x>=2 gold **/
				int droppedgold;
				droppedgold = rand()%5+2;
				if(droppedgold>player.gold){
					droppedgold = player.gold;
				}
				printf("You fled, dropping %d gold in the process.\n", droppedgold);
				player.gold -= droppedgold;
				getch();
				return 2;
			}
			sleep(1);

		} while(!endedturn);
		if(enemy.health <=0){ /** if enemy dead, pickup 8>=x>=3 gold and level up and return to menu**/
			int gold = rand()%6+3;
			player.gold+= gold;
			player.level++;
			printf("You defeated the %s! You picked up %d gold and leveled up.\nYou are now level %d, and have %d max health and %d max mana.\n", enemy.name, gold, player.level, player.maxhealth, player.maxmana);
			getch();
			break;
		}else{
			int damage;
			sleep(1);
			/** enemy turn **/
			damage = rand()%(enemy.maxdamage-enemy.mindamage+1)+enemy.mindamage;
			player.health-= damage;

			printf("%s used %s! You took %d damage.\n", enemy.name, enemy.attacks[rand()%NUM_ATTACKS], damage);
			sleep(1);
			if(player.health <=0){  /** if player dead, drop 5>=x>=1 gold and return to menu**/
				int droppedgold = rand()%5+1;
				if(droppedgold > player.gold){
					droppedgold = player.gold;
				}
				player.gold-=droppedgold;
				printf("You have been defeated by the %s... You lose %d gold.\n", enemy.name, droppedgold);
				sleep(2);
				return 1;
			}
			sleep(1);
		}
	}
	return 0;
}

/**
 * Load menu
 */
Player loadMenu(void) {
	while (1) { /**Loops until game is started**/
		int r, i;
		Player selected; selected.slot = -1;

		clrscr();


		/** take user input **/

		printf("MAIN MENU\n\nChoose save slot:\n");

		for (i=0; i < NUM_SAVES; i++) { /** output all saves **/
			Player temp; temp.slot = i;
			r = parseSave(temp);
			if(r != 0) { // Fail
				printf("%i: %s\n", i, (r == 1 ? "ENOENT" : "EINVAL"));
				continue;
			}

			if (strcmp(temp.name, "null")==0) {
				printf("%i: Empty Save Slot\n", i);
			} else {
				printf("%i: %s's Game. Level %i.\n", i, temp.name, temp.level);
			}
		}

		do {
			printf(">");
			scanf("%d", &(selected.slot));

		} while (selected.slot < 0 || selected.slot >= NUM_SAVES);

		r = parseSave(selected);       /** get selected save **/
		if(r != 0) { // Fail
			printf((r == 1 ? "ENOENT" : "EINVAL"));
			continue;
		}

		if (strcmp(selected.name, "null") !=0) { /** if non-empty save,**/

			int choice;
			clrscr();
			/** take user input **/
			printf("What would you like to do with save slot %i?\n"
					"\t1. Play\n"
					"\t2. Erase\n"
					"\t3. Rename\n"
					"\t4. Go Back\n",
					(selected.slot));
			do {
				printf(">");
				scanf("%d", &choice);


			} while (choice < 1 || choice > 4);

			if (choice == 1) {      /** play game **/

				printf("\nLoading %s's Game from save slot %i...\n", selected.name, selected.slot);
				sleep(2);
				return selected;
			} else if (choice == 2) {       /** erase save **/

				char answer;
				/** confirm action **/
				printf("\nAre you sure you would like to erase %s's Game in slot %i? [Y/N]\n"
						"*THIS ACTION CANNOT BE UNDONE*\n",
						selected.name, selected.slot);
				do{
					printf(">");
					scanf(" %c", &answer);
				} while (answer != 'Y' && answer != 'N' && answer != 'y' && answer != 'n');

				if (answer == 'Y' || answer == 'y') {
					deleteSave(selected);
					printf("Save deleted.\n");
					sleep(1);
				}
			}
			else if (choice == 3) { /** rename save **/

				char newname[MAX_NAME+1] = { '\0' };
				char answer;

				/** take user input **/
				printf("\nWhat would you like to rename this save to? It is currently %s.\n", selected.name);
				do{
					printf(">");

					scanf(" %s", newname);
				}while(newname[0] == '\0');

				/** confirm user input **/
				printf("\nWould you like to rename this save to %s? [Y/N]\n", newname);

				do{
					printf(">");
					scanf(" %c", &answer);
				} while (answer != 'Y' && answer != 'N' && answer != 'y' && answer != 'n');

				if (answer == 'Y' || answer == 'y')
				{
					strcpy(selected.name, newname);
					while(saveGame(selected)); // try save until return 0
					printf("Save renamed.\n");
					sleep(1);
				}
			}
		} else {

			char answer;
			/** confirm choice **/
			printf("\nWould you like to start a game in slot %d? [Y/N]\n", selected.slot);
			do{
				printf(">");
				scanf(" %c", &answer);
			} while (answer != 'Y' && answer != 'N' && answer != 'y' && answer != 'n');
			if (answer == 'Y' || answer == 'y') {
				/** take user input **/
				printf("\nWhat would you like your name to be?\n");
				do{
					printf(">");
   					fgets(selected.name, MAX_NAME, stdin); 
				}while(selected.name == NULL);

				/** initialize new character and save game **/
				selected.level = 1;
				selected.gold = 0;
				selected.healthpotions = 1;
				selected.manapotions = 1;
				initPlayer(selected);
				while(saveGame(selected)); // try save until return 0
				return selected;
			}
		}
	}
}



/**
 * Clears save slot.
 */
int deleteSave(Player& savegame) {
	char filename[10];
	FILE *d;
	int r;
	sprintf(filename, "save%d.txt", savegame.slot); /** create filename string **/


	d = fopen(filename, "w");  /** open and empty the file **/

	fprintf(d, "null 0 0 0 0\n");      /** output "null player" (deleted) **/

	return fclose(d); /** close file **/
}

/**
 * Saves game state to slot.
 */
int saveGame(Player& savegame) {
	char filename[10];
	FILE *save;
	int r;
	sprintf(filename, "save%d.txt", savegame.slot); /** create filename string **/


	save = fopen(filename, "w");    /** open and empty the save file **/

	/** output player info **/
	fprintf(save, "%s %d %d %d %d\n", savegame.name, savegame.level, savegame.gold, savegame.healthpotions, savegame.manapotions);

	r = fclose(save);   /** close file **/
	return r;
}

/**
 * Gets random enemy from file
 */
Enemy parseEnemy(int playerlevel) {

	Enemy enemies[NUM_ENEMIES];         /** create array of all enemies**/
	Enemy enemy;

	FILE *enemyfile;
	int i;
	enemyfile = fopen("enemies.txt", "r+");


	for(i = 0; i < NUM_ENEMIES; i++){   /** loops through each enemy**/
		int x;
		fscanf(enemyfile, "%[^.]. ", enemies[i].name);

		for(x = 0; x < NUM_ATTACKS; x++){   /**loops through each attack**/
			fscanf(enemyfile, "%[^,], ", enemies[i].attacks[x]);
		}
		fscanf(enemyfile, "\n");
	}
	fclose(enemyfile);      /** close file **/

	/** select and initialize random enemy **/

	enemy = enemies[rand()%NUM_ENEMIES];
	enemy.level = rand()%4+(playerlevel-2);		/** enemy level 2 above or below player level **/
	if(enemy.level <1){
		enemy.level=1;
	}

	/** same idea as playerInit() **/
	enemy.maxhealth = ceil(2.5*enemy.level)+4;
	enemy.health = enemy.maxhealth;
	enemy.mindamage = floor(enemy.level/3)+1;
	enemy.maxdamage = 2*ceil(enemy.level/3)+2;

	return enemy;
}

/**
 * Loads and returns a save state from file. Returns save state.
 */
int parseSave(Player& player){
	FILE * temp;
	int r;
	char filename[10];
	sprintf(filename, STR_SAVEFILE, player.slot);


	temp = fopen(filename, "r+");

	fscanf(temp, "%s %d %d %d %d", player.name, &player.level, &player.gold, &player.healthpotions, &player.manapotions);

	r = fclose(temp);

	return r;
}

/*
 * Initialize player values
 */
void initPlayer(Player& player){
	player.maxhealth = (2*player.level)+4;			/** 2x + 4 **/
	player.health = player.maxhealth;
	player.maxmana = floor(player.level/5)+2;		/** x/5 + 2 **/
	player.mana = player.maxmana;
	player.mindamage = floor(player.level/3)+1;		/** x/3 + 1 **/
	player.maxdamage = 2*ceil(player.level/3)+2;;	/** 2x/3 + 2 **/
}
