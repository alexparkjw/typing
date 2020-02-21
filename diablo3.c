#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <locale.h>
#include <math.h>

typedef struct Weapons{
    int Type_weapon;
    int Damage_Min;
    int Damage_Max;
    double Attacks_per_Second;
    double Damage_per_Second;
}weapons;

typedef struct Options{
    int Option_Index;
    double Options_Value;
}options;

typedef struct Items{
    int Item_Parts, Op_Pr, Op_Se, Op_Sp;
    double Armor;
    options *Primery, *Secondary, *Special;
}items;

typedef struct Class{
    int Class, Level, Paragon, P[4][4];
    int Strength, Dexterity, Intelligence, Vitality;
    int Life, Resource;
    int Fury, Hatried, Discipline; 
    int Arcane, Spirit, Mana, Wrath; 
    double DP, Damage,Toughness, Recovery;

    //ITEMS
    int Type_weapon;
    weapons* Onehand;    
    weapons* Twohand;    
    weapons* Offhand;    

    /* items *Head, *Shoulders, *Toros, *Hands, *Wrists, */ 
    /* *Waist, *Legs, *Feet, *Neck, *Finger1, *Finger2; */
    int Item_Parts[11];
    items *Parts[11];

    //OFFENSE
    double Damage_Increased_by_Str;
    double Damage_Increased_by_Dex;
    double Damage_Increased_by_Int;
    double Damage_Increased_by_SDI;
    double Damage_Increased_by_Skills;
    double Attacks_per_Second;
    double Critical_Hit_Chance;
    double Critical_Hit_Damage;
    double Area_Damage;
    double Cooldown_Reduction;

    double Physical_Damage_Increase;
    double Cold_Damage_Increase;
    double Fire_Damage_Increase;
    double Lightning_Damage_Increase;
    double Poison_Damage_Increase;
    double Arcrane_or_Holy_Damage_Increase;

    double Selemic_Slam_Damage_Increase;
    double Phalanx_Damage_Increase;
    /* double _Increase; */
    double Bone_Spear_Damage_Increase;
    double Siphon_Blood_Damage_Increase;
    double Corpse_Lance_Damage_Increase;

    //DEFENSE
    double Armor, Armor_by_Str, Armor_by_Dex;
    double Armor_by_Items[13];
    double Damage_Reduction;
    double Pysical_Resistance;    
    double Pysical_Reduction;
    double g1, g2;

    int Cold_Resistance;    
    double Cold_Reduction;
    int Fire_Resistance;    
    double Fire_Reduction;
    int Lightning_Resistance;    
    double Lightning_Reduction;
    int Poison_Resistance;    
    double Poison_Reduction;
    int Holy_Resistance;  // Arcrane
    double Holy_Reduction;  // Arcrane

    double Crowd_Control_Reduction;
    double Missile_Damage_Reduction;
    double Melle_Damage_Reduction;
    double Elite_Damage_Reduction;
    int Thorns;

    //LIFE
    int Maximum_Life;
    double Total_Life_Bonus;
    double Life_per_Second; 
    double Life_per_Hit;
    double Life_per_kill;
    double Health_Globe_Healing_Bouns;
    double Bonus_to_Gold_or_Globe_Radius;

    //RESOURCE
    int Maximum_Fury;
    double Fury_Regeneration_per_Second; 
    double Fury_Cost_Reduction;

    int Maximum_Wrath;
    double Wrath_Regeneration_per_Second; 
    double Wrath_Cost_Reduction;

    int Maximum_Hated;
    double Hatred_Regeneration_per_Second; 
    double Hatred_Cost_Reduction;
    int Maximum_Discipline;
    int Discipline_Regeneration_per_Second;
    double Discipline_Cost_Reduction;

    int Maximum_Spirit;
    double Spirit_Regeneration_per_Second; 
    double Spirit_Cost_Reduction;

    int Maximum_Essence;
    double Essence_Regeneration_per_Second; 
    double Essence_Cost_Reduction;

    int Maximum_Mana;
    double Mana_Regeneration_per_Second; 
    double Mana_Cost_Reduction;

    int Maximum_Arcane_Power;
    double Arcane_Regeneration_per_Second; 
    double Arcane_Power_Cost_Reduction;
    int Arcane_power_On_Critical_Hit;

    //ADVENTURE
    double Movement_Speed;    
    double Gold_Find;
    double Magin_Find;
    double Bonus_Experience;
    double Bonus_Experience_per_Kill;

    //KANAI'S CUBE POWERS
    weapons *KANAI_Weapon;
    items *KANAI_Armor;
    items *KANAI_Jewelry;
}class;

char * strDup(const char* str);
int strCmp(void* vp1, void* vp2);
void memCpy(void *dest, void *src, size_t size);

//list of functions
class * Set_Class(class *, int);
class * Set_Level(class *, int);
class * refresh(class *this);
void Details(class *);
weapons * Set_Weapon(weapons *, int, int, double, int);
class * Set_Weapon_eq(class *, weapons *);
class * Set_Paragon(class *this, int par[4][4]);


// item woring on it ----------
items* Set_Item(items* this, int type, int op_pr, int op_se, int op_sp, int num, ...) {
    this = (items*) malloc(sizeof(items));
    this->Item_Parts = type;
    this->Op_Pr = op_pr;
    this->Op_Se = op_se;
    this->Op_Sp = op_sp;

    this->Primery = (options *)malloc(sizeof(options)*6);
    this->Secondary = (options *)malloc(sizeof(options)*6);
    this->Special = (options *)malloc(sizeof(options)*2);

    /* int n = 2*(op_pr+op_se+op_sp); */
    va_list args;
    va_start(args, num);
    
    for(int i=0; i<op_pr*2; i++) {
        this->Primery[i].Option_Index = va_arg(args, int);
        this->Primery[i].Options_Value = va_arg(args, double);
    }
    for(int i=0; i<op_se*2; i++) {
        this->Secondary[i].Option_Index = va_arg(args, int);
        this->Secondary[i].Options_Value = va_arg(args, double);
    }

    if(op_sp == 1) { 
        this->Special->Option_Index = va_arg(args, int);
        this->Special->Options_Value = va_arg(args, double);
    }
    va_end(args);
    return this;
}

class* Set_Item_eq(class *this, items *it) {
    this->Item_Parts[it->Item_Parts] = it->Item_Parts;
    memCpy(this->Parts[this->Item_Parts[it->Item_Parts]], it, sizeof(items **));
    /* memCpy(this->Parts[this->Item_Parts[it->Item_Parts]]->Options, it->Options, sizeof(options ***)); */
    this->Armor_by_Items[it->Item_Parts] = it->Armor; 
    return this;
}

// MAIN 
int main(void) {
    setlocale(LC_NUMERIC, "");

    // LIST of CLASSES
    enum {Barbarian, Crusader, Demon_Hunter, Monk, Necromancer, Witch_Doctor, Wizard } choCLA; 
    // CREATE CHARACTER & SET CLASS
    class *myName = Set_Class(myName, Necromancer); 



    // LIST ITEMS
    enum {helms,shoulders,chest,bracers,gloves,belt,pants,amulet,ring1,ring2,shields,oneHanded,Twohand,ranged } itemsEQ;
    enum {axes1, ceremonialKnives, daggers, fistWeapons, maces1, mightyWeapons1, swords1 } oneHandedWeapons;
    enum {axes2, dalbos, maces2, mightyWeapons2, spears, staves, swords2 } twoHandedWeapons;
    enum {bow, crossbows, handCrossbows, wands } rangedWeapons;
    // INVENTROY
    weapons* nothing_but_punch = Set_Weapon(nothing_but_punch, 2, 3, 1.0, 11);
    weapons* initiates_hand_crossbow = Set_Weapon(initiates_hand_crossbow, 2, 3, 1.20, 11);
    weapons* short_bow = Set_Weapon(short_bow, 1, 8, 1.40, 12);

    weapons* base_nat = Set_Weapon(base_nat, 2, 3, 1.20, 11);


    // EQ
    /* myName = Set_Weapon_eq(myName, nothing_but_punch); */
    /* myName = Set_Weapon_eq(myName, initiates_hand_crossbow); */
    /* myName = Set_Weapon_eq(myName, short_bow); */
    myName = Set_Weapon_eq(myName, base_nat);


    // Option_Index ;
    /* items* head = Set_Item(head, 0, 1, 1, 0, 4, 1, 1.0, 2, 1.0); */
    /* myName = Set_Item_eq(myName, head); */

    int paragon[4][4] = { 
        {100, 100, 50, 50}, // Aviltiy, Vitality, Movemnet, Resource
        {50, 50, 50, 50},   // Attack Speed, Cool Down, Critical Rate, Critical Damage
        {50, 50, 50, 50},   // Life%, Armor%, Resist%, Life per Sec
        {50, 50, 50, 50}    // Area Damage, Resouce Down, Life per Hit, Gold%
    };
    myName = Set_Paragon(myName, paragon);


    // level up testing 
    for(int i=0; i<1; i++) {
        /* printf("===========%d=============\n", i+1); */
        // for Level Up
        myName = Set_Level(myName, i+1);
        myName = refresh(myName);
        // print out detail inform. 
        Details(myName);  
    }

    return 0;
}

class* Set_Paragon(class *this, int par[4][4]) {
    for(int i=0; i<4; i++) 
        for(int j=0; j<4; j++)
            this->P[i][j] = par[i][j];
    return this;
}

class* Set_Class(class *this, int p){
    this=(class*)malloc(sizeof(class));
    this->Class = p;
    this->Level = 1;
    this->Paragon = 0;

    this->Strength = 8;
    this->Dexterity = 8;
    this->Intelligence = 8;
    this->Vitality = 9;

    for(int i=0; i<4; i++) 
        for(int j=0; j<4; j++)
            this->P[i][j]=0;

    switch(this->Class) {
        case(0):case(1):
            this->Strength = 10;
            break;
        case(2):case(3):
            this->Dexterity = 10;
            break;
        case(4):case(5):case(6):
            this->Intelligence = 10;
            break;
            break;
    } 

    this->Critical_Hit_Chance = 0.05;
    this->Critical_Hit_Damage = 0.5; 

    // by default
    this->Type_weapon = 11;
    this->Onehand = (weapons*)malloc(sizeof(weapons));
    this->Twohand = (weapons*)malloc(sizeof(weapons));
    this->Offhand = (weapons*)malloc(sizeof(weapons));
    this->Onehand->Damage_Min = 2;
    this->Onehand->Damage_Max = 3;
    this->Onehand->Attacks_per_Second = 1.00;
    this->Onehand->Damage_per_Second = 
        (this->Onehand->Damage_Min + this->Onehand->Damage_Max) * 
        0.5 * this->Onehand->Attacks_per_Second;

    for(int i=0; i<11; i++) {
        this->Parts[i] = (items*) malloc(sizeof(items));
        this->Parts[i]->Primery = (options*) malloc(sizeof(options)*6);
        this->Parts[i]->Secondary = (options*) malloc(sizeof(options)*6);
        this->Parts[i]->Special = (options*) malloc(sizeof(options)*2);
    }

    for(int i=0; i<13; i++) 
        this->Armor_by_Items[i] = 0;

    this->Life_per_Second = 0; 
    this->Life_per_Hit = 0;

    this->Total_Life_Bonus = 0;
    this->Movement_Speed = 0;
    return this;
}

class* Set_Level(class *this, int lvl) {
    if(lvl > 1 && lvl < 71) 
        this->Level = lvl;
    return this;
}

weapons* Set_Weapon(weapons* this, int min, int max, double as, int type) {
    this = (weapons*)malloc(sizeof(weapons));
    this->Type_weapon = type;
    this->Damage_Min = min;
    this->Damage_Max = max;
    this->Attacks_per_Second = as;
    this->Damage_per_Second = (this->Damage_Min + this->Damage_Max) * 
        0.5 * this->Attacks_per_Second;
    return this;
}

class* Set_Weapon_eq(class* this, weapons* w) {
    this->Type_weapon = w->Type_weapon;
    switch(this->Type_weapon) 
    {
        case(1): case(2): case(3): case(4): case(5): 
        case(6): case(7): case(8): case(9): case(10):
            break;
        case(11):
            this->Onehand->Damage_Min = w->Damage_Min;
            this->Onehand->Damage_Max = w->Damage_Max;
            this->Onehand->Attacks_per_Second = w->Attacks_per_Second;
            this->Onehand->Damage_per_Second = w->Damage_per_Second;
            break;
        case(12):
            this->Twohand->Damage_Min = w->Damage_Min;
            this->Twohand->Damage_Max = w->Damage_Max;
            this->Twohand->Attacks_per_Second = w->Attacks_per_Second;
            this->Twohand->Damage_per_Second = w->Damage_per_Second;
            break;
        case(13):
            this->Offhand->Damage_Min = w->Damage_Min;
            this->Offhand->Damage_Max = w->Damage_Max;
            this->Offhand->Attacks_per_Second = w->Attacks_per_Second;
            this->Offhand->Damage_per_Second = w->Damage_per_Second;
            break;
        default:
            printf("nothing\n");
            break;
    }

    return this;
}

class* refresh(class *this) {
    int tmps=0;
    for(int i=0; i<4; i++) 
        for(int j=0; j<4; j++) 
            tmps += this->P[i][j];
    this->Paragon = tmps;

    this->Strength = 8;
    this->Dexterity = 8;
    this->Intelligence = 8;
    this->Vitality = 9 + 5*this->P[0][1];

    switch(this->Class) {
        case(0):case(1):
            this->Strength = 10;
            this->Strength += (this->P[0][0]*5);
            break;
        case(2):case(3):
            this->Dexterity = 10;
            this->Dexterity += (this->P[0][0]*5);
            break;
        case(4):case(5):case(6):
            this->Intelligence = 10;
            this->Intelligence += (this->P[0][0]*5);
            break;
        default:
            printf("nothing\n");
            break;
    } 

    int level[7][4] = { {3, 1, 1, 2}, {3, 1, 1, 2}, {1, 3, 1, 2}, 
        {1, 2, 3, 2}, {1, 1, 3, 2}, {1, 2, 3, 2}, {1, 1, 3, 2} };
    this->Strength += (this->Level-1)*level[this->Class][0];
    this->Dexterity += (this->Level-1)*level[this->Class][1];
    this->Intelligence += (this->Level-1)*level[this->Class][2];
    this->Vitality += (this->Level-1)*level[this->Class][3];


    switch(this->Class) 
    {
        case(0):case(1):
            this->Damage_Increased_by_Str = (double)this->Strength/100;
            this->Damage_Increased_by_SDI = 1+this->Damage_Increased_by_Str;
            break;
        case(2):case(3):
            this->Damage_Increased_by_Dex = (double)this->Dexterity/100;
            this->Damage_Increased_by_SDI = 1+this->Damage_Increased_by_Dex;
            break;
        case(4):case(5):case(6):
            this->Damage_Increased_by_Int = (double)this->Intelligence/100;
            this->Damage_Increased_by_SDI = 1+this->Damage_Increased_by_Int;
            break;
        default:
            printf("nothing\n");
            break;
    } 

    // add items
    this->Critical_Hit_Chance += ((double)this->P[1][2]/1000.0);
    this->Critical_Hit_Damage += ((double)this->P[1][3]/100.0); 

    // if Type_weapone
    double tmp, tmp2,tmp3;
    switch(this->Type_weapon) 
    {
        case(1): case(2): case(3): case(4): case(5): 
        case(6): case(7): case(8): case(9): case(10):
            break;
        case(11):
            this->Attacks_per_Second = this->Onehand->Attacks_per_Second *
                (1.0 + (double)this->P[1][0]/500.0);
            tmp = (this->Onehand->Damage_Min + this->Onehand->Damage_Max) * 
                0.5 * this->Attacks_per_Second * this->Damage_Increased_by_SDI;
            this->Damage = tmp + 
                tmp * this->Critical_Hit_Damage * this->Critical_Hit_Chance;
            break;
        case(12):
            this->Attacks_per_Second = this->Onehand->Attacks_per_Second *
                (1.0 + (double)this->P[1][0]/500.0);
            tmp =(this->Twohand->Damage_Min + this->Twohand->Damage_Max) * 
                0.5 * this->Attacks_per_Second * this->Damage_Increased_by_SDI;
            this->Damage = tmp + 
                tmp * this->Critical_Hit_Damage * this->Critical_Hit_Chance;
            break;
        default:
            printf("nothing\n");
            break;
    }

    this->Armor_by_Str = this->Strength;
    this->Armor_by_Dex = this->Dexterity;
    this->Armor = this->Armor_by_Str + this->Armor_by_Dex;
    for(int i=0; i<13; i++) this->Armor += this->Armor_by_Items[i];
    this->Armor = this->Armor*(1+(double)this->P[2][1]/200.0);

    //--------- floorf(), roundf(), ceilf() ------------------
    int enermyLevel = this->Level;
    this->Damage_Reduction = this->Armor / (this->Armor + (double)enermyLevel*50.0);
    this->Damage_Reduction = roundf(this->Damage_Reduction*100000)/100000;

    this->Pysical_Resistance = (double)this->Intelligence/10.0 
        + (double)this->P[2][2]*5.0;
    this->Pysical_Reduction = this->Pysical_Resistance / 
        ((double)enermyLevel*5.0 + this->Pysical_Resistance);

    this->Pysical_Reduction = ceilf(this->Pysical_Reduction*100000)/100000;
    /* printf("this->Pysical_Reduction = %f\n", this->Pysical_Reduction); */

    this->Toughness = this->Maximum_Life * 
        (1/(1-this->Damage_Reduction)) *
        (1/(1-this->Pysical_Reduction));

    this->Toughness = ceilf(this->Toughness);
    // DP 
    this->DP = 1 - ( 1 - this->Damage_Reduction) * ( 1 - this->Pysical_Reduction);


    // LIFE  
    this->Total_Life_Bonus += (double)this->P[2][0]/200.0;
    this->Maximum_Life = (40 + this->Vitality*10 + 4*(this->Level-1))
        *(1 + this->Total_Life_Bonus);

    double lps = 0.156;
    double lph = 0.11696666;

    this->Life_per_Second += roundf(lps*this->P[2][3]*100)/100; 
    this->Life_per_Hit += roundf(lph*this->P[3][3]*100)/100;

    /* this->Life_per_kill; */
    /* this->Health_Globe_Healing_Bouns; */

    // working on int ----------- A + B 
    this->Recovery = 0;

    // RESOURCES 
    switch(this->Class) {
        case(0):
            this->Maximum_Fury = 100 + this->P[0][3];
            break;
        case(1):
            this->Maximum_Wrath = 100 + this->P[0][3];
            break;
        case(2):
            this->Maximum_Hated = 125 + this->P[0][3];
            this->Maximum_Discipline = 30 + this->P[0][3];
            break;
        case(3):
            this->Maximum_Spirit = 100 + this->P[0][3];
            break;
        case(4):
            this->Maximum_Essence = 200 + this->P[0][3];
            break;
        case(5):
            this->Maximum_Mana = 100 + this->P[0][3];
            break;
        case(6):
            this->Maximum_Arcane_Power = 100 + this->P[0][3];
            break;
        default:
            printf("nothing\n");
    }

    // ADVANTURE
    this->Movement_Speed += (this->P[0][2]/200);    
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
    printf("Paragon:        %10d\n", this->Paragon);
    printf("--------------------------\n");
    printf("Strength:       %10d\n", this->Strength);
    printf("Dexterity:      %10d\n", this->Dexterity);
    printf("Intelligence:   %10d\n", this->Intelligence);
    printf("Vitality:       %10d\n", this->Vitality);
    printf("--------------------------\n");
    printf("Damage:         %10.2f\n", this->Damage);
    printf("Toughness:      %10'd\n", (int)this->Toughness);
    printf("Recovery:       %10.0f\n\n", round(this->Recovery) );

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
    printf("Life +%:      %10.2f%%\n", this->Total_Life_Bonus);
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








