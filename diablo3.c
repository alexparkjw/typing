#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <locale.h>
#include <math.h>

#define DEBUG(V) printf("%d: "#V" = %f\n", __LINE__, V)

typedef struct Weapons{
    int   Type_weapon;
    int   Damage_Min;
    int   Damage_Max;
    float Attacks_per_Second;
    float Damage_per_Second;
}weapon;

typedef struct Options{
    int   Option_Index;
    float Options_Value;
}option;

typedef struct Items{
    int   Item_Parts, Op_Pr, Op_Se, Op_Sp;
    float Armor;
    option *Primery, *Secondary, *Special;
}item;

typedef struct Class{
    int   Class, Level, Total_Paragon, Paragon[16];
    int   Strength, Dexterity, Intelligence;
    int   Primery_Avility, Vitality;
    int   Life, Resource;
    int   Fury, Hatried, Discipline; 
    int   Arcane, Spirit, Mana, Wrath; 
    int   Primery_ResourceType;
    float DP, Damage, Toughness, Recovery;

    // Weapons 
    int Type_weapon;
    weapon *Offhand;    
    weapon *Onehand;    
    weapon *Twohand;    
    weapon *Rainged;
    
    // ITEMS - armor
    int Item_Parts[11];
    item *Parts[11];

    //OFFENSE
    float Damage_Increased_by_Str;
    float Damage_Increased_by_Dex;
    float Damage_Increased_by_Int;
    float Damage_Increased_by_SDI;
    float Damage_Increased_by_Skills;
    float Bonus_Damage_to_Elites;
    float Attacks_per_Second;
    float Attack_Speed_Increase;
    float Critical_Hit_Chance;
    float Critical_Hit_Damage;
    float Area_Damage;
    float Cooldown_Reduction;

    // ELEMENT DAMAGE
    float Physical_Damage_Increase;
    float Cold_Damage_Increase;
    float Fire_Damage_Increase;
    float Lightning_Damage_Increase;
    float Poison_Damage_Increase;
    float Arcrane_or_Holy_Damage_Increase;

    float Selemic_Slam_Damage_Increase;
    float Phalanx_Damage_Increase;

    // SKILL Increase;
    float Bone_Spear_Damage_Increase;
    float Siphon_Blood_Damage_Increase;
    float Corpse_Lance_Damage_Increase;

    //DEFENSE
    float Armor, Armor_by_Str, Armor_by_Dex;
    float Total_Armor_Bonus;
    float Armor_by_Items[13];
    float Damage_Reduction;
    float Pysical_Resistance;    
    float Pysical_Reduction;

    int   Cold_Resistance;    
    float Cold_Reduction;
    int   Fire_Resistance;    
    float Fire_Reduction;
    int   Lightning_Resistance;    
    float Lightning_Reduction;
    int   Poison_Resistance;    
    float Poison_Reduction;
    int   Holy_Resistance;  // Arcrane
    float Holy_Reduction;  // Arcrane

    float Crowd_Control_Reduction;
    float Missile_Damage_Reduction;
    float Melle_Damage_Reduction;
    float Elite_Damage_Reduction;
    int   Thorns;

    //LIFE
    int   Maximum_Life;
    float Total_Life_Bonus;
    float Life_per_Second; 
    float Life_per_Hit;
    float Life_per_kill;
    float Health_Globe_Healing_Bouns;
    float Bonus_to_Gold_or_Globe_Radius;

    //RESOURCE
    int   Maximum_Fury;
    float Fury_Regeneration_per_Second; 
    float Fury_Cost_Reduction;

    int   Maximum_Wrath;
    float Wrath_Regeneration_per_Second; 
    float Wrath_Cost_Reduction;

    int   Maximum_Hated;
    float Hatred_Regeneration_per_Second; 
    float Hatred_Cost_Reduction;
    int   Maximum_Discipline;
    int   Discipline_Regeneration_per_Second;
    float Discipline_Cost_Reduction;

    int   Maximum_Spirit;
    float Spirit_Regeneration_per_Second; 
    float Spirit_Cost_Reduction;

    int   Maximum_Essence;
    float Essence_Regeneration_per_Second; 
    float Essence_Cost_Reduction;

    int   Maximum_Mana;
    float Mana_Regeneration_per_Second; 
    float Mana_Cost_Reduction;

    int   Maximum_Arcane_Power;
    float Arcane_Regeneration_per_Second; 
    float Arcane_Power_Cost_Reduction;
    int   Arcane_power_On_Critical_Hit;

    //ADVENTURE
    float Movement_Speed;    
    float Gold_Find;
    float Magin_Find;
    float Bonus_Experience;
    float Bonus_Experience_per_Kill;

    //KANAI'S CUBE POWERS
    weapon *KANAI_Weapon;
    item   *KANAI_Armor;
    item   *KANAI_Jewelry;
}class;

// own library
char *strDup (const char* str);
int   strCmp (void* vp1, void* vp2);
void  memCpy (void *dest, void *src, size_t size);

//list of functions
class *Set_Class (class *, int);
class *Set_Level (class *, int);
class *Refresh (class *this);
int   Type_Avility (class *this);
float Type_SDI (class *this);
void  Details (class *);
weapon *Set_Weapon (weapon *, int, int, float, int);
class *Weapon_EQ (class *, weapon *);
class *Set_Paragon (class *this, int *);

// item woring on it ----------
item * Set_Item(item* this, int type, int op_pr, int op_se, int op_sp, int num, ...) {
    this = (item*) malloc(sizeof(item));
    this->Item_Parts = type;
    this->Op_Pr = op_pr;
    this->Op_Se = op_se;
    this->Op_Sp = op_sp;

    this->Primery = (option *)malloc(sizeof(option)*6);
    this->Secondary = (option *)malloc(sizeof(option)*6);
    this->Special = (option *)malloc(sizeof(option)*2);

    /* int n = 2*(op_pr+op_se+op_sp); */
    va_list args;
    va_start(args, num);
    
    for(int i=0; i<op_pr*2; i++){
        this->Primery[i].Option_Index = va_arg(args, int);
        this->Primery[i].Options_Value = va_arg(args, double);}
    for(int i=0; i<op_se*2; i++){
        this->Secondary[i].Option_Index = va_arg(args, int);
        this->Secondary[i].Options_Value = va_arg(args, double);}

    if(op_sp == 1){ 
        this->Special->Option_Index = va_arg(args, int);
        this->Special->Options_Value = va_arg(args, double);}
    va_end(args);
    return this;
}

class * Item_EQ(class *this, item *it) {
    this->Item_Parts[it->Item_Parts] = it->Item_Parts;
    memCpy(this->Parts[this->Item_Parts[it->Item_Parts]], it, sizeof(item **));
    /* memCpy(this->Parts[this->Item_Parts[it->Item_Parts]]->Options, it->Options, sizeof(option ***)); */
    this->Armor_by_Items[it->Item_Parts] = it->Armor; 
    return this;
}

// mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
int main(int argc, char *argv[]){
    setlocale(LC_NUMERIC, "");

    //  ENUM A CHARACTER of CLASSES
    enum {
        Barbarian, Crusader, Demon_Hunter, Monk, 
        Necromancer, Witch_Doctor, Wizard 
    } eClass; 

    // ENUM ITEMS LIST
    enum {
        Helms, Shoulders, Chest, Bracers, Gloves, Belt,
        Pants, Amulet, Ring1, Ring2,
        Shields, One_Handed, Twohand, Ranged 
    } eItems;
    enum {
        Axes1, Ceremonial_Knives, Daggers, Fist_Weapons, 
        Maces1, Mighty_Weapons1, Wwords1 
    } eHandedWeapons;
    enum { 
        Axes2, Dalbos, Maces2, Mighty_Weapons2, 
        Spears, Staves, Swords2
    } eTwoHandedWeapons;
    enum {
        Bow, Crossbows, Hand_Crossbows, Wands, Arrow, Bolt 
    } eRangedWeapons;


    // CREATE CHARACTER & SET CLASS
    class *myName = Set_Class(myName, Necromancer); 

    // INVENTROY
    weapon *nothing_but_punch       = Set_Weapon(nothing_but_punch, 2, 3, 1.0, Ranged);
    weapon *initiates_hand_crossbow = Set_Weapon(initiates_hand_crossbow, 2, 3, 1.20, Ranged);
    weapon *short_bow               = Set_Weapon(short_bow, 1, 8, 1.40, Ranged);
    weapon *apprentices_scythe      = Set_Weapon(apprentices_scythe, 2, 3, 1.20, One_Handed);

    // EQ
    /* myName = Weapon_EQ(myName, nothing_but_punch); */
    /* myName = Weapon_EQ(myName, initiates_hand_crossbow); */
    /* myName = Weapon_EQ(myName, short_bow); */
    myName = Weapon_EQ(myName, apprentices_scythe);


    // Option_Index ;
    /* item* head = Set_Item(head, 0, 1, 1, 0, 4, 1, 1.0, 2, 1.0); */
    /* myName = Set_Item_EQ(myName, head); */


    /* enum { Core, Offensive, Defensive, Utility  eParagon; */
    /* Primery_Avility, Vitality_INC, Movemnet_Speed, Max_Resource  */
    /* Attack_Speed, Cooldown, Critical_Chance, Critical_Damage  */
    /* Life_INC, Armor_INC, Resistances, Life_Regeneration } */
    /* Area_Damage, Cost_Resouce, Life_per_Hit, Gold_Find } */
    int paragon[] = { 
        100, 156, 50, 50, 
        50, 50, 50, 50, 
        50, 50, 50, 50, 
        50, 50, 50, 50 };

    myName = Set_Paragon(myName, paragon);
    myName = Set_Level(myName, 18);
    myName = Refresh(myName);
    // print out detail inform. 
    Details(myName);  

    return 0;
}
// eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee

class* Set_Paragon(class *this, int *par) {
    int tmps=0;
    for(int i=0; i<16; i++) {
            this->Paragon[i] = par[i];
            tmps += this->Paragon[i];}
    this->Total_Paragon = tmps;
    return this;
}


float Type_SDI(class *this) {
    /* this->Damage_Increased_by_Str = (float)this->Strength/100; */
    /* this->Damage_Increased_by_Dex = (float)this->Dexterity/100; */
    /* this->Damage_Increased_by_Int = (float)this->Intelligence/100; */
    switch(this->Class) {
        case(0):case(1):
            return (float)this->Strength/100;
        case(2):case(3):
            return (float)this->Dexterity/100;
        case(4):case(5):case(6):
            return (float)this->Intelligence/100;
        default:
            printf("\nchracter chack\n");
            return 0;
    } 
}

int Type_Avility (class *this) {
    switch(this->Class) {
        case(0):case(1):
            return this->Strength;
        case(2):case(3):
            return this->Dexterity;
        case(4):case(5):case(6):
            return this->Intelligence;
        default:
            printf("\nchracter chack\n");
            return 0;
    }
}

// INIT CHARACTER
class* Set_Class(class *this, int choiracter){
    this=(class*)malloc(sizeof(class));
    // LEVEL1 INIT
    this->Class   = choiracter;
    this->Level   = 1;
    this->Total_Paragon = 0;

    // AVILITY INIT
    this->Strength     = 8;
    this->Dexterity    = 8;
    this->Intelligence = 8;
    this->Vitality     = 9;
    this->Primery_Avility  = Type_Avility(this);

    // PARAGON INIT
    for(int i=0; i<16; i++) this->Paragon[i]=0;

    // PUNCH INIT -  no weapon EQ 
    this->Type_weapon = 11;
    this->Offhand = (weapon*)malloc(sizeof(weapon));
    this->Onehand = (weapon*)malloc(sizeof(weapon));
    this->Twohand = (weapon*)malloc(sizeof(weapon));
    this->Rainged = (weapon*)malloc(sizeof(weapon));

    // ONEHANDED PUNCH
    this->Onehand->Damage_Min = 2;
    this->Onehand->Damage_Max = 3;
    this->Onehand->Attacks_per_Second = 1.00;
    this->Onehand->Damage_per_Second = 
        (this->Onehand->Damage_Min + this->Onehand->Damage_Max) * 
        0.5 * this->Onehand->Attacks_per_Second;


    // ITEMS SPECIPIC AVILITIES INIT
    for(int i=0; i<11; i++) {
        this->Parts[i]            = (item*)   malloc(sizeof(item));
        this->Parts[i]->Primery   = (option*) malloc(sizeof(option)*6);
        this->Parts[i]->Secondary = (option*) malloc(sizeof(option)*6);
        this->Parts[i]->Special   = (option*) malloc(sizeof(option)*2);}

    /// NO ARMOR INIT - no items EQ
    for(int i=0; i<13; i++) this->Armor_by_Items[i] = 0;


    // OFFENSE INIT
    // MAIN AVILITY DAMAGE INCREASE 
    this->Damage_Increased_by_Str = (float)this->Strength/100;
    this->Damage_Increased_by_Dex = (float)this->Dexterity/100;
    this->Damage_Increased_by_Int = (float)this->Intelligence/100; 
    this->Damage_Increased_by_SDI = Type_SDI(this);

    /* this->Damage_Increased_by_Skills = 0; */
    /* this->Bonus_Damage_to_Elites = 0; */
    this->Attacks_per_Second    = 1.0;
    this->Attack_Speed_Increase = 0;

    this->Critical_Hit_Chance   = 0.05;  // 5%
    this->Critical_Hit_Damage   = 0.5;   // 50%
    /* this->Area_Damage =0 ; */
    /* this->Cooldown_Reduction = 0; */

    // ELEMENT DAMAGE INCREASE
    this->Physical_Damage_Increase  = 0;
    this->Cold_Damage_Increase      = 0;
    this->Fire_Damage_Increase      = 0;
    this->Lightning_Damage_Increase = 0;
    this->Poison_Damage_Increase    = 0;
    this->Arcrane_or_Holy_Damage_Increase = 0;

    /* this->Selemic_Slam_Damage_Increase = 0; */
    /* this->Phalanx_Damage_Increase = 0; */


    // DEFENSE INIT ,  부분생략 Refresh()
    /* this->Armor, Armor_by_Str, Armor_by_Dex; */
    this->Toughness = 0;
    /* this->Pysical_Resistance = 1; */    
    /* this->Pysical_Reduction; */
    /* this->Cold_Resistance = 1; */    
    /* this->Cold_Reduction; */
    /* this->Fire_Resistance = 1; */    
    /* this->Fire_Reduction; */
    /* this->Lightning_Resistance = 1; */
    /* this->Lightning_Reduction; */
    /* this->Poison_Resistance = 1; */
    /* this->Poison_Reduction; */
    /* this->Holy_Resistance = 1;  // Arcrane */
    /* this->Holy_Reduction; */ 

    this->Crowd_Control_Reduction  = 0;
    this->Missile_Damage_Reduction = 0;
    this->Melle_Damage_Reduction   = 0;
    this->Elite_Damage_Reduction   = 0;
    this->Damage_Reduction         = 0;

    // LIFE INIT
    /* this->Maximum_Life; */
    this->Total_Life_Bonus              = 0;
    this->Life_per_Second               = 0; 
    this->Life_per_Hit                  = 0;
    this->Life_per_kill                 = 0;
    this->Health_Globe_Healing_Bouns    = 0;
    this->Bonus_to_Gold_or_Globe_Radius = 0;

    //RESOURCE
    // 생략, Refresh();    

    //ADVENTURE
    this->Movement_Speed            = 0;
    this->Gold_Find                 = 0;
    this->Magin_Find                = 0; 
    this->Bonus_Experience          = 0;
    this->Bonus_Experience_per_Kill = 0;

    return this;
}

class * Set_Level(class *this, int lvl) {
    if(lvl >= 1 && lvl <= 70) this->Level = lvl; else return this;

    enum { 
        Strength, Dexterity, Intelligence, Vitality 
    } eAvility;

    enum { 
        Core_Avility, Vitality_INC, Movemnet_Speed, Max_Resource, 
        Attack_Speed, Cooldown, Critical_Chance, Critical_Damage, 
        Life_INC, Armor_INC, Resistances_INC, Life_Regeneration, 
        Area_Damage, Cost_Resource, Life_per_Hit, Gold_Find } 
    eParagon;

    /* DEBUG(this->Class); */
    switch(this->Class) {
        case(0):case(1):
            this->Damage_Increased_by_Str = (float)this->Strength/100;
            this->Damage_Increased_by_SDI = 1+this->Damage_Increased_by_Str;
            break;
        case(2):case(3):
            this->Damage_Increased_by_Dex = (float)this->Dexterity/100;
            this->Damage_Increased_by_SDI = 1+this->Damage_Increased_by_Dex;
            break;
        case(4):case(5):case(6):
            this->Damage_Increased_by_Int = (float)this->Intelligence/100;
            this->Damage_Increased_by_SDI = 1+this->Damage_Increased_by_Int;
            break;
        default:
            printf("error \n"); 
            break;
    }

    // class infor.
    int addPoint[7][3] = { 
        {1, 0, 0}, {1, 0, 0}, 
        {0, 1, 0}, {0, 1, 0},
        {0, 0, 1}, {0, 0, 1}, {0, 0, 1} };
    int level[7][4] = { 
        {3, 1, 1, 2}, {3, 1, 1, 2}, 
        {1, 3, 1, 2}, {1, 2, 3, 2}, 
        {1, 1, 3, 2}, {1, 2, 3, 2}, {1, 1, 3, 2} };

    // instead of switch simple way
    this->Strength     = 8 + 2*addPoint[this->Class][Strength];
    this->Dexterity    = 8 + 2*addPoint[this->Class][Dexterity];
    this->Intelligence = 8 + 2*addPoint[this->Class][Intelligence];;
    this->Vitality     = 9 + 5*this->Paragon[Vitality_INC];

    // add 5% per paragon
    this->Strength     += (this->Paragon[Core_Avility]*5)*addPoint[this->Class][Strength];
    this->Dexterity    += (this->Paragon[Core_Avility]*5)*addPoint[this->Class][Dexterity];
    this->Intelligence += (this->Paragon[Core_Avility]*5)*addPoint[this->Class][Intelligence];
    this->Primery_Avility   = Type_Avility(this);

    // adding avilites per levels by each class
    this->Strength     += (this->Level-1)*level[this->Class][Strength];
    this->Dexterity    += (this->Level-1)*level[this->Class][Dexterity];
    this->Intelligence += (this->Level-1)*level[this->Class][Intelligence];
    this->Vitality     += (this->Level-1)*level[this->Class][Vitality];

    return this;
}

weapon * Set_Weapon(weapon* this, int min, int max, float as, int type) {
    this = (weapon*)malloc(sizeof(weapon));
    this->Type_weapon        = type;
    this->Damage_Min         = min;
    this->Damage_Max         = max;
    this->Attacks_per_Second = as;
    this->Damage_per_Second  = (this->Damage_Min + this->Damage_Max) * 
        0.5 * this->Attacks_per_Second;
    return this;
}

class * Weapon_EQ(class *this, weapon *weaponEQ) {
    this->Type_weapon = weaponEQ->Type_weapon;
    switch(this->Type_weapon) {
        case(1): case(2): case(3): case(4): case(5): 
        case(6): case(7): case(8): case(9):
            break;
        case(10):
            this->Offhand->Damage_Min = weaponEQ->Damage_Min;
            this->Offhand->Damage_Max = weaponEQ->Damage_Max;
            this->Offhand->Attacks_per_Second = weaponEQ->Attacks_per_Second;
            this->Offhand->Damage_per_Second  = weaponEQ->Damage_per_Second;
            break;
        case(11):
            this->Onehand->Damage_Min = weaponEQ->Damage_Min;
            this->Onehand->Damage_Max = weaponEQ->Damage_Max;
            this->Onehand->Attacks_per_Second = weaponEQ->Attacks_per_Second;
            this->Onehand->Damage_per_Second  = weaponEQ->Damage_per_Second;
            break;
        case(12):
            this->Twohand->Damage_Min = weaponEQ->Damage_Min;
            this->Twohand->Damage_Max = weaponEQ->Damage_Max;
            this->Twohand->Attacks_per_Second = weaponEQ->Attacks_per_Second;
            this->Twohand->Damage_per_Second  = weaponEQ->Damage_per_Second;
            break;
        case(13):
            this->Rainged->Damage_Min = weaponEQ->Damage_Min;
            this->Rainged->Damage_Max = weaponEQ->Damage_Max;
            this->Rainged->Attacks_per_Second = weaponEQ->Attacks_per_Second;
            this->Rainged->Damage_per_Second  = weaponEQ->Damage_per_Second;
            break;
        default:
            printf("nothing\n");
            break;
    }

    return this;
}


// DETAILS UPDATE 
class * Refresh(class *this) {  
    enum { 
        Core_Avility, Vitality_INC, Movemnet_Speed, Max_Resource, 
        Attack_Speed, Cooldown, Critical_Chance, Critical_Damage, 
        Life_INC, Armor_INC, Resistances_INC, Life_Regeneration, 
        Area_Damage, Cost_Resource, Life_per_Hit, Gold_Find } ;

    /* DEBUG(this->Class); */
    switch(this->Class) {
        case(0):case(1):
            this->Damage_Increased_by_Str = (float)this->Strength/100;
            this->Damage_Increased_by_SDI = 1+this->Damage_Increased_by_Str;
            break;
        case(2):case(3):
            this->Damage_Increased_by_Dex = (float)this->Dexterity/100;
            this->Damage_Increased_by_SDI = 1+this->Damage_Increased_by_Dex;
            break;
        case(4):case(5):case(6):
            this->Damage_Increased_by_Int = (float)this->Intelligence/100;
            this->Damage_Increased_by_SDI = 1+this->Damage_Increased_by_Int;
            break;
        default:
            printf("nothing\n");
            break;
    } 

    // LIFE +% per Paragon 
    this->Total_Life_Bonus += (float)this->Paragon[Life_INC]/200.0;
    this->Maximum_Life      = (1 + this->Total_Life_Bonus) *
        (40 + this->Vitality*10 + 4*(this->Level-1));

    // point per paragon
    float lps = 0.156;
    float lph = 0.11696666;

    this->Life_per_Second += roundf(lps*this->Paragon[Life_Regeneration]*100)/100; 
    this->Life_per_Hit    += roundf(lph*this->Paragon[Life_per_Hit]*100)/100;

    /* this->Life_per_kill; */
    /* this->Health_Globe_Healing_Bouns; */

    // working on int ----------- A + B 
    this->Recovery = 0;



    // ADD POINT per PARAGON
    this->Attack_Speed_Increase += (float)this->Paragon[Attack_Speed]/500; // + sikll .....
    this->Area_Damage           += (this->Paragon[Area_Damage]/100);
    this->Critical_Hit_Chance   += ((float)this->Paragon[Critical_Chance]/1000);
    this->Critical_Hit_Damage   += ((float)this->Paragon[Critical_Damage]/100); 

    // if Type_weapon - working on it ......
    float tmp;
    switch(this->Type_weapon) {
        case(1): case(2): case(3): case(4): case(5): 
        case(6): case(7): case(8): case(9):
            break;
        case(10):
            this->Attacks_per_Second = this->Offhand->Attacks_per_Second *
                (1.0 + this->Attack_Speed_Increase);
            tmp = (this->Offhand->Damage_Min + this->Offhand->Damage_Max) * 
                0.5 * this->Attacks_per_Second * this->Damage_Increased_by_SDI;
            this->Damage = tmp + 
                tmp * this->Critical_Hit_Damage * this->Critical_Hit_Chance;
            break;
        case(11):
            this->Attacks_per_Second = this->Onehand->Attacks_per_Second *
                (1.0 + this->Attack_Speed_Increase);
            tmp = (this->Onehand->Damage_Min + this->Onehand->Damage_Max) * 
                0.5 * this->Attacks_per_Second * this->Damage_Increased_by_SDI;
            this->Damage = tmp + 
                tmp * this->Critical_Hit_Damage * this->Critical_Hit_Chance;
            break;
        case(12):
            this->Attacks_per_Second = this->Twohand->Attacks_per_Second *
                (1.0 + this->Attack_Speed_Increase);
            tmp = (this->Twohand->Damage_Min + this->Twohand->Damage_Max) * 
                0.5 * this->Attacks_per_Second * this->Damage_Increased_by_SDI;
            this->Damage = tmp + 
                tmp * this->Critical_Hit_Damage * this->Critical_Hit_Chance;
            break;
        case(13):
            this->Attacks_per_Second = this->Rainged->Attacks_per_Second *
                (1.0 + this->Attack_Speed_Increase);
            tmp = (this->Rainged->Damage_Min + this->Rainged->Damage_Max) * 
                0.5 * this->Attacks_per_Second * this->Damage_Increased_by_SDI;
            this->Damage = tmp + 
                tmp * this->Critical_Hit_Damage * this->Critical_Hit_Chance;
            break;
        default:
            printf("nothing\n");
            break;
    }

    // ARMOR CALC
    this->Armor_by_Str = this->Strength;
    this->Armor_by_Dex = this->Dexterity;
    this->Armor = this->Armor_by_Str + this->Armor_by_Dex;
    for(int i=0; i<13; i++) this->Armor += this->Armor_by_Items[i];
    this->Total_Armor_Bonus += (float)this->Paragon[Armor_INC]/200;
    this->Armor = this->Armor*(1 + this->Total_Armor_Bonus);

    // Armor by skill ...... working on it


    // DAMAGE REDUCTION by ARMOR  (acc; floorf, roundff, ceilf, f=float)
    int enermyLevel = this->Level, acc = 1000000;
    this->Damage_Reduction = this->Armor / (this->Armor + (float)enermyLevel*50.0);
    /* DEBUG(this->Armor); DEBUG(this->Damage_Reduction); */
    this->Damage_Reduction = roundf(this->Damage_Reduction*acc)/acc;
    /* DEBUG(this->Damage_Reduction); */


    // DAMAGE REDUCTION by RESISTANCE (pysical_resistance)
    this->Pysical_Resistance = (float)this->Intelligence/10.0 + (float)this->Paragon[Resistances_INC]*5;
    this->Pysical_Reduction = this->Pysical_Resistance / ((float)enermyLevel*5.0 + this->Pysical_Resistance);
    /* DEBUG(this->Pysical_Reduction); */
    this->Pysical_Reduction = roundf(this->Pysical_Reduction*acc)/acc;
    /* DEBUG(this->Pysical_Reduction); */


    // DODGE FACTOR
    /* float Dodge_Chance = 1 - 1/this->Dexterity; */
    /* float Dodge_Factor = 1 / (1 - Dodge_Chance); */
    /* DEBUG(Dodge_Factor); */
    /* Dodge_Factor = roundf(Dodge_Factor*acc)/acc; */
    /* DEBUG(Dodge_Factor); */

    this->Toughness = this->Maximum_Life * 
        (1/(1-this->Damage_Reduction)) * 
        (1/(1-this->Pysical_Reduction));
    /* DEBUG(this->Maximum_Life); DEBUG(this->Toughness); */
    this->Toughness = roundf(this->Toughness);
    /* DEBUG(this->Toughness); */

    // DP 
    this->DP = 1 - ( 1 - this->Damage_Reduction) * ( 1 - this->Pysical_Reduction);


    // RESOURCES 
    switch(this->Class) {
        case(0):
            this->Maximum_Fury = 100 + this->Paragon[Max_Resource];
            break;
        case(1):
            this->Maximum_Wrath = 100 + this->Paragon[Max_Resource];
            break;
        case(2):
            this->Maximum_Hated = 125 + this->Paragon[Max_Resource];
            this->Maximum_Discipline = 30 + this->Paragon[Max_Resource];
            break;
        case(3):
            this->Maximum_Spirit = 100 + this->Paragon[Max_Resource];
            break;
        case(4):
            this->Maximum_Essence = 200 + this->Paragon[Max_Resource];
            break;
        case(5):
            this->Maximum_Mana = 100 + this->Paragon[Max_Resource];
            break;
        case(6):
            this->Maximum_Arcane_Power = 100 + this->Paragon[Max_Resource];
            break;
        default:
            printf("nothing\n");
    }

    // RESOURCES by skill ... working on it.....

    // ADVANTURE
    this->Movement_Speed += (this->Paragon[Movemnet_Speed]/200);    
    this->Gold_Find      += (this->Paragon[Gold_Find]/100);;
    /* this->Magin_Find; */
    /* this->Bonus_Experience; */
    /* this->Bonus_Experience_per_Kill; */

    return this;
}

void Details(class *this) {
    char *class[] = { 
        "Barbarian", "Crusader", "Demon_Hunter", "Monk", 
        "Necromancer", "Witch_Doctor", "Wizard" 
    };

    printf("Class:   %17s\n", class[this->Class]);
    printf("==========================\n");
    printf("level:          %10d\n", this->Level);
    printf("Paragon:        %10d\n", this->Total_Paragon);
    printf("--------------------------\n");
    printf("Strength:       %10d\n", this->Strength);
    printf("Dexterity:      %10d\n", this->Dexterity);
    printf("Intelligence:   %10d\n", this->Intelligence);
    printf("Vitality:       %10d\n", this->Vitality);
    printf("--------------------------\n");
    printf("Damage:         %10.2f\n", this->Damage);
    printf("Toughness:      %10'd\n", (int)this->Toughness);
    /* printf("Toughness:      %10.2f\n", this->Toughness); */
    printf("Recovery:       %10.0f\n\n", roundf(this->Recovery) );

    // OFFENCE
    printf("OFFENCE\n");
    printf("==========================\n");
    printf("1\n");
    printf("2\n");
    printf("3\n");
    printf("4\n");
    printf("5\n");
    printf("6\n");
    printf("7\n");
    printf("8\n");
    printf("9\n\n");

    // DEFFENCE
    printf("DEFENCE\n");
    printf("==========================\n");
    printf("\n");
    printf("Armor:          %10.0f\n", this->Armor );
    /* printf("(%.2f%%)\n", this->Damage_Reduction*100 ); */
    printf("Damage Reduction: (%4.2f%%)\n", this->Damage_Reduction*100 );
    printf("Pysical Resistance:   %4.0f\n", this->Pysical_Resistance );
    /* printf("(%.2f%%)\n", this->Pysical_Reduction*100 ); */
    printf("Pysical Reduction:(%4.2f%%)\n", this->Pysical_Reduction*100 );
    printf("A\n"); 
    printf("A\n"); 
    printf("A\n"); 
    printf("A\n"); 
    printf("A\n"); 
    printf("B\n"); 
    printf("B\n"); 
    printf("B\n"); 
    printf("B\n"); 
    printf("DP: %4.2f%%\n\n", this->DP*100 );

    // LIFE
    printf("LIFE\n");
    printf("========================\n");
    printf("Maximun Life: %10d\n", this->Maximum_Life);
    printf("Life %%+:     %10.2f%%\n", this->Total_Life_Bonus*100);
    printf("Life Second : %10.2f\n", this->Life_per_Second);
    printf("Life/Hit:     %10.2f\n", this->Life_per_Hit);
    printf("A\n");
    printf("A\n");
    printf("A\n\n");

    // RESOURCE
    printf("RESOURCE\n");
    printf("========================\n");
    switch(this->Class) {
        case 0:
            printf("Fury:      %10d\n", this->Maximum_Fury);
            break;
        case 1:
            printf("Wrath:      %10d\n", this->Maximum_Wrath);
            break;
        case 2:
            printf("Hatried:      %10d\n", this->Maximum_Hated);
            printf("Discipline:   %10d\n\n", this->Maximum_Discipline);
            break;
        case 3:
            printf(":Spirit      %10d\n", this->Maximum_Spirit);
            break;
        case 4:
            printf("Essence:      %10d\n", this->Maximum_Essence);
            break;
        case 5:
            printf("Mana:      %10d\n", this->Maximum_Mana);
            break;
        case 6:
            printf("Arcane Power:      %10d\n", this->Maximum_Arcane_Power);
            break;
        default:
            break;
    }
    printf("A\n"); 
    printf("A\n\n"); 

    printf("ADVANTURE\n");
    printf("========================\n");
    printf("A\n"); 
    printf("A\n"); 
    printf("A\n"); 
    printf("A\n"); 
    printf("A\n"); 
    printf("\n\n");

    printf("KANAI\n");
    printf("========================\n");
    printf("A\n"); 
    printf("A\n"); 
    printf("A\n\n"); 
}

char * strDup(const char *str){
    int n = strlen(str) + 1;
    char * dup = (char *)malloc(sizeof(char)*n);
    if (dup) strcpy(dup, str);
    return dup;
}

int strCmp(void *vp1, void *vp2) {
    char * s1 = (char *)vp1;
    char * s2 = (char *)vp2;
    return strcmp(s1, s2);
}

void memCpy(void *dest, void *src, size_t size) { 
    char *c_src = (char *)src; 
    char *c_dest = (char *)dest; 
    for (int i=0; i<size; i++) 
        c_dest[i] = c_src[i]; 
} 

