/*
    Mines

    Copyright (c) 1979, 2005, 2006, 2007 James L. Dean

    Version 1.1 released 2/24/2007

    This C program and associated data files may be distributed or used without
payment to its author.  Derivative works must credit its author.

    "Mines" lets you explore mines. The mine you explore is determined by a mine
number specified at the beginning of a game.

    The object of a game is to visit all of the rooms and return all of the
treasures to the entrance without making too many moves.

    In a mine, the passages are straight.  So, for example, if you go North to
leave a room, you can go South to reenter it.  The rooms are not evenly spaced.
However, the distance between adjacent rooms is always a multiple of the minimum
distance between adjacent rooms.
*/

#include <stdio.h>

#define TRUE  1
#define FALSE 0

#define MAX_LINE_LEN     79
#define MSG_BUFFER_LEN 1024
#define NUM_ROOMS        99
#define NUM_TREASURES    15

typedef char int8;
typedef short int int16;
typedef int intC;

typedef struct RoomNode
          {
            char  *szDescription;
            int16 nChokepoint;
            int8  bMined;
            int8  bVisited;
            union
              {
                struct
                  {
                    int16 pPassageNorth;
                    int16 pPassageSouth;
                    int16 pPassageEast;
                    int16 pPassageWest;
                    int16 pPassageUp;
                    int16 pPassageDown;
                  }                Passage;
                int16 pPassage [6];
              }   Adjacent;
           } RoomRec;

typedef struct TreasureNode
          {
            char   *szTreasure;
            char   *szGuardian;
            char   *szWeapon;
            int8   bSlain;
            int16  pRoomTreasure;
            int16  pRoomWeapon;
          } TreasureRec;

typedef struct PassageNode
          {
            int8  bBlocked;
            int16 pTreasureGuardian;
            int16 pRoom1;
            int16 pRoom2;
          } PassageRec;

typedef struct PathStackNode
          {
            int16 pRoom;
            int16 pPassageUsedToEnterRoom;
            int8  nDirectionUsedToEnterRoom;
            int16 pNext;
          } PathStackRec;

typedef struct FindPathLocalsStackNode
          {
            int8  bRoomAlreadyInStack;
            int8  nDirection1;
            int8  nDirection2;
            int8  nDirection3;
            int8  nDirectionRandom [6];
            int16 nPathLen;
            int16 pPathStack;
            int16 pNext;
          } FindPathLocalsStackRec;

typedef struct FindPathParametersStackNode
          {
            int16 pRoom;
            int8  nDirectionUsedToEnterRoom;
            int8  nReturnAddress;
            int16 pNext;
          } FindPathParametersStackRec;

static void  Add(int16 *,int16 *,int16 *);
static void  Carry(int16,int8);
static intC  Command(void);
static int16 Compare(int16 *,int16 *);
static void  ConvertInt16ToBase256(int16,int16 *);
static void  DisplayHelp(int8);
static void  DisplayText(char *,int8,int8);
static void  ExcavateMine(int16,int16,int8,int16 *,int16 *,int8 *);
static void  FindPathToEntrance(int16,int16,char **);
static void  HideTreasuresAndWeapons(int16,int16 *,int8);
static void  Inventory(int16,int8,int8 *);
static void  IToA(int16,char *);
static void  Leave(int16,int8);
static void  ListCommands(void);
static void  ListPassages(int16,int8 *);
static void  ListTreasures(int16,int8,int8 *);
static void  ListWeapons(int16,int8,int8 *);
       int   main(void);
static void  Move(int16 *,int8,int16 *,int8 *);
static void  Multiply(int16 *,int16 *,int16 *);
static void  PlayGame(int16,int16,int16 *,int16,int8,int8 *);
static void  Points(int16,int16,int16 *,int8,int16,int8,int8 *);
static int16 RandomNumber(void);
static intC  ReadChar(void);
static void  SeedRandomNumberGenerator(void);
static void  ShuffleRoomDescriptions(int16,char **);
static void  StrCat(char *,char *);
static void  StrCpy(char *,char *);
static int16 StrLen(char *);
static void  Subtract(int16 *,int16 *,int16 *);
static intC  ToUpper(intC);
static void  WayOut(int16,int16,int16,int16 *,int8,int8 *);
static void  WriteChar(char);

static FindPathLocalsStackRec         m_FindPathLocal [3*NUM_ROOMS-2];
static FindPathParametersStackRec     m_FindPathParameter [3*NUM_ROOMS-2];
static int8                           m_nAddIndex;
static int8                           m_nDirectionOpposite [6] = {1,0,3,2,5,4};
static int16                          m_nFindPathLocals = 0;
static int16                          m_nFindPathParameters = 0;
static int16                          m_nMaxInt16 [4] = { 255, 127, 0, 0 }; /* 32767 */
static int16                          m_nMaxInt16Plus1 [4] = { 0, 128, 0, 0 }; /* 32768 */
static int16                          m_nPassages = 0;
static int16                          m_nPaths = 0;
static int16                          m_nPrime [4] = { 3, 128, 0, 0 }; /* 32771 */
static int8                           m_nReplaceIndex;
static int16                          m_nRooms = 0;
static int16                          m_nRN [8] [4];
static int16                          m_nRNPartialSum [4];
static PassageRec                     m_Passage [9*NUM_ROOMS-8];
static PathStackRec                   m_Path [6*NUM_ROOMS];
static int16                          m_pRoomWithName [NUM_ROOMS+1];
static RoomRec                        m_Room [3*NUM_ROOMS-2];
static char                           *m_szDirection[6]
 = {"north","south","east","west","up","down"};
static char                           m_szMsg [MSG_BUFFER_LEN];
static char                           *m_szRoom [NUM_ROOMS]
 = {
     "You\'re in a spherical room.",
     "You\'re in the hall of bones.  Dinosaur bones are everywhere.",
     "You\'re in a subway tunnel.  Don't touch that third rail!",
     "You\'re in a railroad tunnel.  A whistle wails in the distance.",
     "You\'re in an elfin quiche factory.  The elves are out mowing spinach.",
     "You\'re in an abandoned Plymouth plant.  Beware of Road Runners and Barracudas.",
     "You\'re in an abandoned Dodge plant.  There is a Dart embedded in the North wall.",
     "You\'re in a mouse's nest.  You'd best exhale; this is a small room.",
     "You\'re in a giant crayfish hole.  An immense chicken neck is hanging from a rope.",
     "You\'re in an abandoned coal mine.  Beware of methane.",
     "You\'re in the hall of winds.  Presently, the wind is from the south.",
     "You\'re in a stove pipe!",
     "You\'re in a totally darkened room.  Although you can see nothing, the sound of dripping water echoes from the walls.",
     "You\'re in an industrial waste site.  Hold your breath and don't touch anything.",
     "You\'re in the warehouse for an extremely popular brand of home computer.  Tacky plastic enclosures abound.",
     "You\'re in a hobbit's bedroom.  The hobbit does not look pleased!",
     "You\'re in a troll sewerage processing plant.  The air quality is not good.",
     "You\'re in a rabbit hole.  There is a jar of marmalade on a shelf in the wall.",
     "You\'re in the giant's oven.  Fortunately, it hasn't been used for years.",
     "You\'re in a hobbit's drying room.  Tobacco leaves abound.",
     "You\'re in a large circular room.  It is dark in here.",
     "You\'re in the Crystal Palace.  Quartz crystals cover the walls.",
     "You\'re in the Emerald Room.  Green crystals cover the ceiling.",
     "You\'re in a curtained room.",
     "You\'re in an air conditioning duct!",
     "You\'re in a giant kiln.  Smoke stains the walls.",
     "You\'re in the Hall of Mists.  Wisps of white vapor rise from the floor.",
     "You\'re in an Aztec pyramid.  A mummy lies in the northwest corner.",
     "You\'re in the Room of the Eternal Flame.  A large natural gas flame burns in the center of the room.  Coming from the west wall you can barely hear the words, 'Fee Fye Foe Fum'.",
     "You\'re in the Giant's store room.  You are surrounded by old rugs, books, chairs, etc.",
     "You\'re in the Leprechaun's Treasure Room.  Unfortunately, the leprechaun's treasure was stolen years ago.",
     "You\'re in a large tiled room.  A girl is inquiring after a rabbit.  Feel free to ignore her.",
     "You\'re in a former nuclear test site.  There is a large pile of rubble in the center of the floor.  The walls are streaked with a multitude of colors.",
     "You\'re in a drainage system holding tank.  Either it is no longer used, or it hasn't rained in a long time; the room is dry.",
     "You\'re in Mayor Schiro's bomb shelter.",
     "You\'re in a room with a waterfall.  It is not very impressive; it looks like someone left a water faucet running.",
     "You\'re in an abandoned Neanderthal home.",
     "You\'re in a volcanic chimney.  The air is getting warmer.",
     "You\'re in a pit full of snakes.  Let's get out of here!!!!",
     "You\'re in a salt dome.",
     "You\'re in Eleanor Roosevelt's privy.  Wendell Wilkie buttons cover the wall.",
     "You\'re in Napoleon's wine cellar.  German wines line the shelves.",
     "You\'re in the space behind the giant's bathroom wall. Large razor blades litter the floor.  Step lightly.",
     "You\'re in the room where all old toe nail clipping come to rest.  Used corn pads litter the floor.",
     "You\'re in the Den of the Horta.  The walls are covered with a sticky fluid.  Don't touch it; it is extremely corrosive.",
     "You\'re in a damp room.  A small creek runs into a crack in the West wall.",
     "You\'re in what appears to be a NOPSI manhole.",
     "You\'re in the cafeteria of Mammoth Cave.  The aroma of rancid corned beef assails your nostrils.",
     "You\'re in a small room with a large table.  On the table is a bottle that says, 'Drink me.'",
     "You\'re in a Confederate States of America bank vault.  Once worthless currency papers the walls.",
     "You\'re in an abandoned subway station.",
     "You\'re in a mine shaft.  In the distance you can hear seven high pitched voices singing, 'Hi Ho, Hi Ho, ...'",
     "You\'re in a Minuteman missile silo.",
     "You\'re in the giant's mouse trap.  Fortunately, you are small enough to escape.",
     "You\'re in Adolph Hitler's summer bunker.",
     "You\'re in a dwarf work site.  A sign says, \"Under construction. Enter at your own risk.\"",
     "You\'re in the giant's refrigerator.  Dwarf bodies hang from hooks.",
     "You\'re in the Dragon's Lair.  Slightly melted suits of armor litter the floor.",
     "You\'re in a nuclear waste depository.  The walls glow faintly.",
     "You\'re in Millard Fillmore's tomb.  It is dull.",
     "You\'re in an abandoned corridor of the Strategic Air Command Headquarters.  A graffito reads, \"Beware of Bat Guano.\"",
     "You\'re in a gnome's workshop.  Half-completed whoopee cushions line the tables.",
     "You\'re in the Mummy's Tomb.  You've triggered some mechanism and the ceiling is slowly descending.",
     "You\'re in the Underground Gourmet's retreat.  Twinkie and King Don wrappers are piled knee deep.",
     "You\'re in a Hoola Hoop warehouse.  The words \"Shoop Shoop\" echo from the walls.",
     "You\'re in the first circle of hell.  The living are not allowed here.",
     "You\'re in the hall of the pixies.  The furniture appears to have been made from cradles.",
     "You\'re in a sulfur mine.  Molten sulfur churns in a nearby pit. It is becoming difficult to breath.",
     "You\'re in a fairy mushroom farm.  Brilliantly colored mushrooms cover the floor.",
     "You\'re in an ice cave.  Along the western wall, a brontosaurus is defrosting.",
     "You\'re in the giant's stove.  Fortunately, the giant now owns a microwave oven.",
     "You\'re in the rib cage of a long deceased whale.",
     "You\'re in a room with six walls.  The walls appear to be made of wax.  A loud buzzing noise can be heard.",
     "You\'re in the tomb of a Pharaoh.  It has obviously been visited by others; the tomb is in a state of total disarray.",
     "You\'re in a coal bin.  There is a fossilized fern stump here.",
     "You\'re in a diamond mine.  It is uncomfortably hot here.",
     "You\'re in the bottom of an oil well.  The walls are slick.",
     "You\'re in the lowest level of Project Mohole.  The funding bubble burst before the earth did.",
     "You\'re in the giant's cesspool.  Fortunately, the giant was connected to the city sewerage system years ago.",
     "You\'re in an eighteenth century sewer.  The walls are crumbling brick.  Small alligators snap at your feet.",
     "You\'re in the lair of a giant trapdoor spider.",
     "You\'re in a giant gopher tunnel.",
     "You\'re in a shell -- the sole remains of a prehistoric turtle.",
     "You\'re in a small chamber.  The walls are made of earth.  The air smells of formic acid.  A strange squeaking noise can be heard in the distance.",
     "You\'re in a chamber of columns.  The stalagmites and stalactites join here.",
     "You\'re in a service tunnel.  Ducts, pipes, and cables are everywhere.",
     "You\'re in a gas tank below an abandoned service station.  No smoking!",
     "You\'re in a huge dark chamber.  To one side, a dark, muddy river moves sluggishly.  A sign written in ancient Greek says, \"Ferry out of order.\"",
     "You\'re in a small chamber. It is brightly lit by a peculiar lichen growing on the walls and ceiling.  The floor is rocky and steeply sloping.  Nearby, a cold, clear creek boils into the floor and out of sight.",
     "You\'re in the nest of a very large pack rat.  There are discarded aluminum Christmas trees, broken steel utility poles, and other shiny, worthless items here.",
     "You\'re in a dungeon.  An iron maiden, a rack, a piano, and other machines of torture can be seen.",
     "You\'re in the hall of bats.  Thousands of bats hang from the ceiling.  Watch your step; the droppings are quite deep in places.",
     "You\'re in a hobgoblin's hideaway.",
     "You\'re in an electrical substation.  A transformer is humming loudly.  Nearby, cables crackle with high voltage.",
     "You\'re in the \"gold\" room.  The walls are covered with iron pyrite.",
     "You\'re in a room with one of Dracula's emergency coffins.  The Count is out.",
     "You\'re in a saltpeter mine.  To one side there is a huge wooden evaporation vat.  Small crystals of saltpeter cause the walls to sparkle.",
     "You\'re in the basement cafeteria of a local hospital.  Some say that there has been nothing edible here for years.",
     "You\'re in a troll arsenal.  Kegs of gun powder surround you."
   };
static char                           m_szWayOut [NUM_ROOMS+1];
static TreasureRec                    m_Treasure [NUM_TREASURES]
 = {
     {
       "bag full of gold coins",
       "gorgon",
       "mirror",
       0,
       -1,
       -1
     },
     {
       "large blue-white diamond",
       "grizzly bear",
       "pepper spray dispenser",
       0,
       -1,
       -1
     },
     {
       "sixty carat emerald",
       "vampire",
       "wooden stake",
       0,
       -1,
       -1
     },
     {
       "platinum crucifix",
       "crocodile",
       "roll of duct tape",
       0,
       -1,
       -1
     },
     {
       "string of pearls",
       "giant crayfish",
       "jar of Zatarain's Crab Boil",
       0,
       -1,
       -1
     },
     {
       "Ming vase",
       "troll",
       "elfin sword",
       0,
       -1,
       -1
     },
     {
       "list of the next ten winners of the Kentucky Derby",
       "werewolf",
       "silver bullet",
       0,
       -1,
       -1
     },
     {
       "pile of rubies",
       "harpy",
       "crossbow",
       0,
       -1,
       -1
     },
     {
       "previously undiscovered Rembrandt painting",
       "cobra",
       "flute fashioned from a dried-out pumpkin",
       0,
       -1,
       -1
     },
     {
       "ounce of antimatter in magnetic containment",
       "ferocious snail",
       "bag of salt",
       0,
       -1,
       -1
     },
     {
       "stack of silver bars",
       "vicious Doberman pinscher",
       "soporific-laced dog biscuit",
       0,
       -1,
       -1
     },
     {
       "set of ivory tusks",
       "colossal cockroach",
       "block of boric acid",
       0,
       -1,
       -1
     },
     {
       "Holland and Holland double rifle chambered for .600 Nitro Express",
       "giant",
       "slingshot",
       0,
       -1,
       -1
     },
     {
       "ancient Greek statue",
       "Trekkie",
       "recording of Leonard Nimoy singing \"Proud Mary\"",
       0,
       -1,
       -1
     },
     {
       "five kilograms of plutonium in a lead container",
       "titanic ant",
       "Fresnel lens",
       0,
       -1,
       -1
     }
   };

static intC ReadChar(void)
  {
    return fgetc(stdin);
  }

static void WriteChar(char c)
  {
    fputc((int) c,stdout);
    return;
  }

static intC ToUpper(intC n)
  {
    intC nResult;

    if (n >= (intC) 'a')
      if (n <= (intC) 'z')
        nResult=(n-((intC) 'a'))+((intC) 'A');
      else
        nResult=n;
    else
      nResult=n;
    return nResult;
  }

static int16 StrLen(char *sz)
  {
    int16 nResult;
    char  *pc;

    nResult=0;
    pc=sz;
    while (*pc)
      {
        ++nResult;
        ++pc;
      }
    return nResult;
  }

static void StrCpy(char *szDestination, char *szSource)
  {
    char *pcDestination;
    char *pcSource;

    pcDestination=szDestination;
    pcSource=szSource;
    while (*pcSource)
      {
        *pcDestination=*pcSource;
        ++pcDestination;
        ++pcSource;
      }
    *pcDestination='\0';
    return;
  }

static void StrCat(char *szDestination, char *szSource)
  {
    char *pcDestination;

    pcDestination=szDestination;
    while (*pcDestination)
      ++pcDestination;
    StrCpy(pcDestination,szSource);
    return;
  }

static void IToA(int16 n, char  *sz)
  {
    char  c [10];
    char  cDigit [10] = {'0','1','2','3','4','5','6','7','8','9'};
    int8  nChars;
    int16 nDividend;
    int16 nQuotient;
    int16 nRemainder;
    char  *pc;

    nChars=0;
    nDividend=n;
    do
      {
        nQuotient=nDividend/10;
        nRemainder=nDividend-10*nQuotient;
        c[nChars]=cDigit[nRemainder];
        ++nChars;
        nDividend=nQuotient;
      }
    while (nDividend);
    pc=sz;
    while (nChars)
      {
        --nChars;
        *pc=c[nChars];
        ++pc;
      }
    *pc='\0';
    return;
  }

static void DisplayText(char *szText, int8 bPrintBlankLine, int8 bOutputCRLF)
  {
    int8  nColumn;
    int16 nPendingSpaces;
    int16 nTokenLen;
    int16 nTokenStart;
    int16 nTokenStop;

    nColumn=1;
    nPendingSpaces=0;
    nTokenStart=0;
    while (szText[nTokenStart] && (szText[nTokenStart] == ' '))
      {
        WriteChar(' ');
        if (++nColumn > MAX_LINE_LEN)
          {
            WriteChar('\n');
            nColumn=1;
          }
        ++nTokenStart;
      }
    while (szText[nTokenStart])
      {
        while (szText[nTokenStart] && (szText[nTokenStart] == ' '))
          ++nTokenStart;
        if (nColumn+nPendingSpaces > MAX_LINE_LEN)
          {
            WriteChar('\n');
            nPendingSpaces=0;
            nColumn=1;
          }
        else
          while (nPendingSpaces)
            {
              WriteChar(' ');
              ++nColumn;
              --nPendingSpaces;
            }
        if (szText[nTokenStart])
          {
            nTokenStop=nTokenStart;
            while (szText[nTokenStop] && (szText[nTokenStop] != ' '))
              ++nTokenStop;
            nTokenLen=nTokenStop-nTokenStart;
            if (nColumn+nTokenLen > MAX_LINE_LEN)
              if (nTokenLen > MAX_LINE_LEN)
                {
                  while (nColumn < MAX_LINE_LEN)
                    {
                      WriteChar(szText[nTokenStart]);
                      ++nColumn;
                      ++nTokenStart;
                    }
                  WriteChar('\n');
                  nColumn=1;
                  while (nTokenStart < nTokenStop)
                    {
                      WriteChar(szText[nTokenStart]);
                      if (++nColumn > MAX_LINE_LEN)
                        {
                          WriteChar('\n');
                          nColumn=1;
                        }
                      ++nTokenStart;
                    }
                }
              else
                {
                  WriteChar('\n');
                  nColumn=1;
                  while (nTokenStart < nTokenStop)
                    {
                      WriteChar(szText[nTokenStart]);
                      ++nColumn;
                      ++nTokenStart;
                    }
                }
            else
              while (nTokenStart < nTokenStop)
                {
                  WriteChar(szText[nTokenStart]);
                  ++nColumn;
                  ++nTokenStart;
                }
            switch (szText[nTokenStop-1])
              {
                case '.':
                  nPendingSpaces=2;
                  break;
                case '?':
                  nPendingSpaces=2;
                  break;
                case '!':
                  nPendingSpaces=2;
                  break;
                default:
                  nPendingSpaces=1;
                  break;
              }
          }
      }
    if (bOutputCRLF)
      {
        if (nColumn > 1)
          WriteChar('\n');
      }
    if (bPrintBlankLine)
      WriteChar('\n');
    return;
  }

static void ListCommands(void)
  {
    DisplayText(
     "Commands in the game are \"N\", \"E\", \"S\", \"W\", \"U\", and "
     "\"D\" to move North, East, South, West, Up, or Down, "
     "respectively.  Other commands are \"C\" to carry things, \"I\" to "
     "inventory what you are carrying, \"L\" to leave treasures, \"P\" to "
     "get the points you\'ve scored, \"O\" for help getting out of the "
     "mine, \"H\" for help, and \"Q\" to quit.",TRUE,TRUE);
    return;
  }

static void DisplayHelp(int8 bPrintBlankLine)
  {
    ListCommands();
    DisplayText(
     "In a mine, the passages are straight.  So, for example, if "
     "you go North to leave a room, you can go South to reenter it.  "
     "The rooms are not evenly spaced.  However, the distance between "
     "adjacent rooms is always a multiple of the minimum distance "
     "between adjacent rooms.",bPrintBlankLine,TRUE);
    return;
  }

static void Add(int16 *nAddend1, int16 *nAddend2, int16 *nSum)
  {
    int8  nByte;
    int16 nCarry;
    int16 nTem;
    int16 *pnAddend1;
    int16 *pnAddend2;
    int16 *pnSum;

    nCarry=0;
    pnAddend1=nAddend1;
    pnAddend2=nAddend2;
    pnSum=nSum;
    for (nByte=4; nByte--;)
      {
        nTem=(*pnAddend1)+(*pnAddend2)+nCarry;
        if (nTem > 256)
          {
            nTem-=256;
            nCarry=1;
          }
        else
          nCarry=0;
        *pnSum=nTem;
        ++pnAddend1;
        ++pnAddend2;
        ++pnSum;
      }
    return;
  }

static void Subtract( int16 *nMinuend, int16 *nSubtrahend, int16 *nDifference)
  {
    int8  nByte;
    int16 nMinuendTem [4];
    int16 *pnDifference;
    int16 *pnMinuend;
    int16 *pnSubtrahend;

    nMinuendTem[0]=nMinuend[0];
    nMinuendTem[1]=nMinuend[1];
    nMinuendTem[2]=nMinuend[2];
    nMinuendTem[3]=nMinuend[3];
    pnMinuend=&nMinuendTem[0];
    pnSubtrahend=nSubtrahend;
    pnDifference=nDifference;
    for (nByte=4; nByte--;)
      {
        if (*pnMinuend < *pnSubtrahend)
          {
            *pnMinuend+=256;
            if (nByte)
              --(*(pnMinuend+1));
          }
        *pnDifference=(*pnMinuend)-(*pnSubtrahend);
        ++pnMinuend;
        ++pnSubtrahend;
        ++pnDifference;
      }
    return;
  }

static int16 Compare( int16 *n1, int16 *n2)
  {
    int16 nResult;

    nResult=0;
    nResult=n1[3]-n2[3];
    if (nResult == 0)
      {
        nResult=n1[2]-n2[2];
        if (nResult == 0)
          {
            nResult=n1[1]-n2[1];
            if (nResult == 0)
              nResult=n1[0]-n2[0];
          }
      }
    return nResult;
  }

static void Multiply(int16 *nMultiplicand, int16 *nMultiplier, int16 *nProduct)
  {
    int16 nByte;
    int16 nIteration;
    int16 nIterations;
    int16 nShift [4];

    for (nByte=0; nByte < 4; ++nByte)
      {
        nProduct[nByte]=0;
        nShift[nByte]=nMultiplicand[nByte];
      }
    for (nByte=0; nByte < 4; ++nByte)
      {
        nIterations=nMultiplier[nByte];
        for (nIteration=0; nIteration < nIterations; ++nIteration)
          Add(nProduct,nShift,nProduct);
        nShift[3]=nShift[2];
        nShift[2]=nShift[1];
        nShift[1]=nShift[0];
        nShift[0]=0;
      }
    return;
  }

static void ConvertInt16ToBase256(int16 n, int16 *nBase64)
  {
    int8  nByte;
    int16 nDividend;
    int16 nQuotient;
    int16 nRemainder;

    nDividend=n;
    for (nByte=0; nByte < 4; ++nByte)
      {
        nQuotient=nDividend/256;
        nRemainder=nDividend-256*nQuotient;
        nBase64[nByte]=nRemainder;
        nDividend=nQuotient;
      }
    return;
  }

static int16 RandomNumber(void)
  {
    int16 nResult;
    int16 nSum [4];
    int16 nTem [4];
   /*
       Each pseudo-random number is the modulo sum of the
     previous eight pseudo-random numbers.  A prime modulus
     makes it likely that the pseudo-random numbers will be
     uniformly distributed.  To speed computation, a partial
     sum of 7 of the 8 previous pseudo-random numbers is maintained.
          For a given set of initial values m_nRN[i], i=0,1,2,...,7,
     this random number generator should produce the same sequence
     of random numbers, no matter what 32-bit C Compiler it is compiled
     under.
   */
    do
      {
        Add(&m_nRNPartialSum[0],&m_nRN[m_nAddIndex][0],&nSum[0]);
        if (Compare(&nSum[0],&m_nPrime[0]) >= 0)
          Subtract(&nSum[0],&m_nPrime[0],&nSum[0]);
        if (Compare(&nSum[0],&m_nRN[m_nReplaceIndex][0]) < 0)
          {
            Add(&nSum[0],&m_nPrime[0],&nTem[0]);
            Subtract(&nTem[0],&m_nRN[m_nReplaceIndex][0],&m_nRNPartialSum[0]);
          }
        else
          Subtract(&nSum[0],&m_nRN[m_nReplaceIndex][0],&m_nRNPartialSum[0]);
        m_nRN[m_nReplaceIndex][0]=nSum[0];
        m_nRN[m_nReplaceIndex][1]=nSum[1];
        m_nRN[m_nReplaceIndex][2]=nSum[2];
        m_nRN[m_nReplaceIndex][3]=nSum[3];
        m_nAddIndex=m_nReplaceIndex;
        if (++m_nReplaceIndex >= 8)
          m_nReplaceIndex=0;
      }
    while (Compare(&nSum[0],&m_nMaxInt16[0]) > 0);
    nResult=256*nSum[1]+nSum[0];
    return nResult;
  }

static void SeedRandomNumberGenerator(void)
  {
    int16 nKeyIndex;
    int16 nKeyStart;
    int16 nKeyLength;
    intC  nTem;
    char  *pc;
    char  szMine [9];

    nKeyLength=0;
    DisplayText("Mine number? ",FALSE,FALSE);
    do
      {
        nTem=ReadChar();
        if ((nTem != EOF) && (nTem != (intC) '\n'))
          {
            if (nTem != (intC) '\r')
              {
                if (nTem == 8)
                   --nKeyLength;
                else
                  {
                    if (nKeyLength < 8)
                      szMine[nKeyLength]=(char) nTem;
                    ++nKeyLength;
                  }
              }
          }
      }
    while ((nTem != EOF) && (nTem != (intC) '\n'));
    WriteChar('\n');
    if (nKeyLength > 8)
      nKeyLength=8;
    szMine[nKeyLength]='\0';
    nKeyIndex=0;
    if (nKeyLength < 8)
      {
        for (nKeyStart=nKeyLength; nKeyStart < 8; ++nKeyStart)
          {
            m_nRN[nKeyIndex][0]=1+(int) '0';
            m_nRN[nKeyIndex][1]=0;
            m_nRN[nKeyIndex][2]=0;
            m_nRN[nKeyIndex][3]=0;
            ++nKeyIndex;
          }
        pc=szMine;
      }
    else
      pc=szMine+nKeyLength-8;
    while (*pc)
      {
        m_nRN[nKeyIndex][0]=1+(int) *pc++;
        m_nRN[nKeyIndex][1]=0;
        m_nRN[nKeyIndex][2]=0;
        m_nRN[nKeyIndex][3]=0;
        ++nKeyIndex;
      }
    m_nRNPartialSum[0]=0;
    m_nRNPartialSum[1]=0;
    m_nRNPartialSum[2]=0;
    m_nRNPartialSum[3]=0;
    for (nKeyIndex=7; nKeyIndex > 0; --nKeyIndex)
      Add(&m_nRNPartialSum[0],&m_nRN[nKeyIndex][0],&m_nRNPartialSum[0]);
    m_nReplaceIndex=1;
    m_nAddIndex=0;
    for (nKeyIndex=256; nKeyIndex--;)
      RandomNumber();
    return;
  }

static void ShuffleRoomDescriptions(int16 nRooms, char  **pszRoom)
  {
    int16 nRoom1;
    int16 nRoom2;
    char  *sz;

    for (nRoom1=nRooms-1; nRoom1 > 0; -- nRoom1)
      {
        nRoom2=RandomNumber()%nRoom1;
        sz=pszRoom[nRoom1];
        pszRoom[nRoom1]=pszRoom[nRoom2];
        pszRoom[nRoom2]=sz;
      }
    return;
  }

static void ExcavateMine(int16 nRooms, int16 pRoomEntrance, int8  nTreasures, int16 *nChokepoints, int16 *ppRoomWithName, int8  *bErr)
  {
    int16 nChokepoint;
    int8  nDirectionOpposite;
    int8  nDirection1;
    int8  nDirection2;
    int16 nDistance;
    int8  nHeight;
    int8  nLen;
    int8  nMax;
    int16 nRoom;
    int16 nStep;
    int16 nTem1 [4];
    int16 nTem2 [4];
    int16 nTem3 [4];
    int16 nTem4 [4];
    int8  nTreasure;
    int16 nUnblocked;
    int8  nWidth;
    int8  nX;
    int8  nXFirstRoom;
    int8  nY;
    int8  nYFirstRoom;
    int8  nZ;
    int8  nZFirstRoom;
    int16 pPassage;
    int16 pRoom;
    int16 pRoomDown;
    int16 pRoomNext;
    int16 pRoomSouth;
    int16 pRoomSouthBase;
    int16 pRoomWest;
    int16 pRoomWestBase;
    int16 pRoomWestBaseBase;

    /* nMax=(int) (10.0+exp(log((double) nRooms)/3.0)); */
    nMax=0;
    while (nMax*nMax*nMax < nRooms)
      ++nMax;
    if (nMax*nMax*nMax == nRooms)
      nMax+=10;
    else
      nMax+=9;
    do
      {
        nLen=RandomNumber()%(nMax-3)+3;
        nWidth=RandomNumber()%(nMax-3)+3;
        nHeight=RandomNumber()%(nMax-3)+3;
      }
    while ((nLen*nWidth*nHeight < 2*(nRooms-1)) || (nLen*nWidth*nHeight > 3*(nRooms-1)));
    nXFirstRoom=0;
    nYFirstRoom=RandomNumber()%nLen;
    nZFirstRoom=RandomNumber()%nHeight;
    pRoomWestBase=-1;
    pRoomWestBaseBase=-1;
    for (nX=0; ((! *bErr) && (nX < nWidth)); ++nX)
      {
        pRoomSouthBase=-1;
        for (nY=0; ((! *bErr) && (nY < nLen)); ++nY)
          {
            pRoomWest=pRoomWestBase;
            pRoomSouth=pRoomSouthBase;
            pRoomDown=-1;
            for (nZ=0; ((! *bErr) && (nZ < nHeight)); ++nZ)
              {
                if (m_nRooms < 3*NUM_ROOMS-2)
                  {
                    pRoom=m_nRooms;
                    ++m_nRooms;
                    m_Room[pRoom].szDescription=NULL;
                    m_Room[pRoom].bMined=FALSE;
                    m_Room[pRoom].nChokepoint=-1;
                    m_Room[pRoom].bVisited=FALSE;
                    m_Room[pRoom].Adjacent.Passage.pPassageNorth=-1;
                    m_Room[pRoom].Adjacent.Passage.pPassageSouth=-1;
                    m_Room[pRoom].Adjacent.Passage.pPassageEast=-1;
                    m_Room[pRoom].Adjacent.Passage.pPassageWest=-1;
                    m_Room[pRoom].Adjacent.Passage.pPassageUp=-1;
                    m_Room[pRoom].Adjacent.Passage.pPassageDown=-1;
/*
(X,Y,Z) gets linked to (X-1,Y,Z); (X,Y-1,Z); and (X,Y,Z-1)
                       pRoomWest  pRoomSouth     pRoomDown
*/
                    if (nZ == 0)
                      {
                        pRoomSouthBase=pRoom;
                        if (nY == 0)
                          pRoomWestBaseBase=pRoom;
                      }
                    if (pRoomDown != -1)
                      {
                      /* link room to one below it */
                        if (m_nPassages < (9*NUM_ROOMS-8))
                          {
                            pPassage=m_nPassages;
                            ++m_nPassages;
                            m_Passage[pPassage].bBlocked=TRUE;
                            m_Passage[pPassage].pTreasureGuardian=-1;
                            m_Passage[pPassage].pRoom1=pRoom;
                            m_Passage[pPassage].pRoom2=pRoomDown;
                            m_Room[pRoom].Adjacent.Passage.pPassageDown=pPassage;
                            m_Room[pRoomDown].Adjacent.Passage.pPassageUp=pPassage;
                          }
                        else
                          {
                            *bErr=TRUE;
                            DisplayText("Fatal error:  out of memory",FALSE,TRUE);
                          }
                      }
                    if (pRoomSouth != -1)
                      {
                        /* link to room to the south */
                        if (m_nPassages < 9*NUM_ROOMS-8)
                          {
                            pPassage=m_nPassages;
                            ++m_nPassages;
                            m_Passage[pPassage].bBlocked=TRUE;
                            m_Passage[pPassage].pTreasureGuardian=-1;
                            m_Passage[pPassage].pRoom1=pRoom;
                            m_Passage[pPassage].pRoom2=pRoomSouth;
                            m_Room[pRoom].Adjacent.Passage.pPassageSouth=pPassage;
                            m_Room[pRoomSouth].Adjacent.Passage.pPassageNorth=pPassage;
                          }
                        else
                          {
                            *bErr=TRUE;
                            DisplayText("Fatal error:  out of memory",FALSE,TRUE);
                          }
                        if (m_Room[pRoomSouth].Adjacent.Passage.pPassageUp != -1)
                          if (m_Passage[m_Room[pRoomSouth].Adjacent.Passage.pPassageUp].pRoom1 == pRoomSouth)
                            pRoomSouth=m_Passage[m_Room[pRoomSouth].Adjacent.Passage.pPassageUp].pRoom2;
                          else
                            pRoomSouth=m_Passage[m_Room[pRoomSouth].Adjacent.Passage.pPassageUp].pRoom1;
                        else
                          pRoomSouth=-1;
                      }
                    if (pRoomWest != -1)
                      {
                      /* link to the room to the west */
                        if (m_nPassages < 9*NUM_ROOMS-8)
                          {
                            pPassage=m_nPassages;
                            ++m_nPassages;
                            m_Passage[pPassage].bBlocked=TRUE;
                            m_Passage[pPassage].pTreasureGuardian=-1;
                            m_Passage[pPassage].pRoom1=pRoom;
                            m_Passage[pPassage].pRoom2=pRoomWest;
                            m_Room[pRoom].Adjacent.Passage.pPassageWest=pPassage;
                            m_Room[pRoomWest].Adjacent.Passage.pPassageEast=pPassage;
                          }
                        else
                          {
                            *bErr=TRUE;
                            DisplayText("Fatal error:  out of memory",FALSE,TRUE);
                          }
                        if (m_Room[pRoomWest].Adjacent.Passage.pPassageUp != -1)
                          if (m_Passage[m_Room[pRoomWest].Adjacent.Passage.pPassageUp].pRoom1 == pRoomWest)
                            pRoomWest=m_Passage[m_Room[pRoomWest].Adjacent.Passage.pPassageUp].pRoom2;
                          else
                            pRoomWest=m_Passage[m_Room[pRoomWest].Adjacent.Passage.pPassageUp].pRoom1;
                        else
                          pRoomWest=-1;

                      }
                    /* If this is the first room, connect it to the entrance. */
                    if ((nX == nXFirstRoom) && (nY == nYFirstRoom) && (nZ == nZFirstRoom))
                      {
                        if (m_nPassages < 9*NUM_ROOMS-8)
                          {
                            pPassage=m_nPassages;
                            ++m_nPassages;
                            m_Passage[pPassage].bBlocked=FALSE;
                            m_Passage[pPassage].pTreasureGuardian=-1;
                            m_Passage[pPassage].pRoom1=pRoom;
                            m_Passage[pPassage].pRoom2=pRoomEntrance;
                            m_Room[pRoom].Adjacent.Passage.pPassageWest=pPassage;
                            m_Room[pRoomEntrance].Adjacent.Passage.pPassageEast=pPassage;
                          }
                        else
                          {
                            *bErr=TRUE;
                            DisplayText("Fatal error:  out of memory",FALSE,TRUE);
                          }
                      }
                    pRoomDown=pRoom;
                  }
                else
                  {
                    *bErr=TRUE;
                    DisplayText("Fatal error:  out of memory",FALSE,TRUE);
                  }
              }
            if (pRoomWestBase != -1)
              {
                if (m_Room[pRoomWestBase].Adjacent.Passage.pPassageNorth != -1)
                  {
                    if (m_Passage[m_Room[pRoomWestBase].Adjacent.Passage.pPassageNorth].pRoom1 == pRoomWestBase)
                      pRoomWestBase=m_Passage[m_Room[pRoomWestBase].Adjacent.Passage.pPassageNorth].pRoom2;
                    else
                      pRoomWestBase=m_Passage[m_Room[pRoomWestBase].Adjacent.Passage.pPassageNorth].pRoom1;
                  }
              }
          }
        pRoomWestBase=pRoomWestBaseBase;
      }
    nChokepoint=-1;
    nTreasure=0;
    pRoom=pRoomEntrance;
    nRoom=0;
    while (nRoom < nRooms)
      {
        nDirection1=RandomNumber()%6;
        nDirectionOpposite=m_nDirectionOpposite[nDirection1];
        nDistance=RandomNumber()%3+1;
        nStep=0;
        while ((m_Room[pRoom].Adjacent.pPassage[nDirection1] != -1) && (nStep < nDistance))
          {
            pPassage=m_Room[pRoom].Adjacent.pPassage[nDirection1];
            if (m_Passage[pPassage].pRoom1 == pRoom)
              pRoomNext=m_Passage[pPassage].pRoom2;
            else
              pRoomNext=m_Passage[pPassage].pRoom1;
            if (m_Passage[pPassage].bBlocked)
              if ((m_Room[pRoomNext].nChokepoint == nChokepoint) || (! m_Room[pRoomNext].bMined))
                {
                  pRoom=pRoomNext;
                  m_Room[pRoom].bMined=TRUE;
                  m_Room[pRoom].nChokepoint=nChokepoint;
                  m_Passage[pPassage].bBlocked=FALSE;
                  if (m_Room[pRoom].szDescription)
                    ++nStep;
                  else
                    {
                      nUnblocked=0;
                      for (nDirection2=6; ((nUnblocked < 2) && (nDirection2--));)
                        if (nDirection2 != nDirectionOpposite)
                          if (m_Room[pRoom].Adjacent.pPassage[nDirection2] != -1)
                            if (! m_Passage[m_Room[pRoom].Adjacent.pPassage[nDirection2]].bBlocked)
                              ++nUnblocked;
                      if (nUnblocked < 2)
                        ++nStep;
                      else
                        nStep=nDistance;
                    }
                }
              else
                nStep=nDistance;
            else
              {
                ++nStep;
                pRoom=pRoomNext;
                nChokepoint=m_Room[pRoom].nChokepoint;
              }
          }
        if (nStep)
          {
            if (m_Room[pRoom].szDescription == NULL)
              {
                m_Room[pRoom].szDescription=m_szRoom[nRoom];
                nDirection1=0;
                nUnblocked=0;
                for (nDirection2=6; ((nUnblocked < 2) && (nDirection2--));)
                  if (m_Room[pRoom].Adjacent.pPassage[nDirection2] != -1)
                    if (! m_Passage[m_Room[pRoom].Adjacent.pPassage[nDirection2]].bBlocked)
                      {
                        nDirection1=nDirection2;
                        ++nUnblocked;
                      }
                if (nUnblocked == 1)
                  {
                    ConvertInt16ToBase256((int16) (nRooms-nRoom),&nTem1[0]);
                    ConvertInt16ToBase256(RandomNumber(),&nTem2[0]);
                    Multiply(&nTem2[0],&nTem1[0],&nTem3[0]);
                    ConvertInt16ToBase256((int16) (nTreasures-nTreasure),&nTem1[0]);
                    Multiply(&nTem1[0],&m_nMaxInt16Plus1[0],&nTem4[0]);
                    if (Compare(&nTem3[0],&nTem4[0]) < 0)
                      {
                        nChokepoint=nTreasure;
                        m_Passage[m_Room[pRoom].Adjacent.pPassage[nDirection1]].pTreasureGuardian=nTreasure;
                        ++nTreasure;
                      }
                  }
                m_Room[pRoom].nChokepoint=nChokepoint;
                ppRoomWithName[nRoom]=pRoom;
                ++nRoom;
              }
          }
      }
    ppRoomWithName[nRoom]=pRoomEntrance;
    *nChokepoints=nTreasure;
    return;
  }

static void FindPathToEntrance(int16 pRoomStart, int16 pRoomEntrance, char  **pszWayOut)
  {
    int8  bOutOfMemory;
    int8  bReenter;
    int8  bResume;
    int16 nIterations;
    int8  nReturnAddress;
    int16 pFindPathLocals;
    int16 pFindPathLocalsHead;
    int16 pFindPathParameters;
    int16 pFindPathParametersHead;
    int16 pPathStackHead;

    bOutOfMemory=FALSE;
    pFindPathLocals=-1;
    pFindPathLocalsHead=-1;
    pFindPathParameters=-1;
    pFindPathParametersHead=-1;
    if (m_nFindPathParameters < 3*NUM_ROOMS-2)
      {
        pFindPathParameters=m_nFindPathParameters;
        ++m_nFindPathParameters;
        m_FindPathParameter[pFindPathParameters].pNext=pFindPathParametersHead;
        m_FindPathParameter[pFindPathParameters].pRoom=pRoomStart;
        m_FindPathParameter[pFindPathParameters].nDirectionUsedToEnterRoom=0;
        m_FindPathParameter[pFindPathParameters].nReturnAddress=0;
        pFindPathParametersHead=pFindPathParameters;
        pPathStackHead=-1;
        bResume=FALSE;
        nIterations=0;
        do
          {
            ++nIterations;
            bReenter=FALSE;
            if (bResume)
              {
                for (;((! bOutOfMemory) && (! bReenter) && (! *pszWayOut) && ((m_FindPathLocal[pFindPathLocalsHead].nDirection1) > 0));)
                  {
                    --(m_FindPathLocal[pFindPathLocalsHead].nDirection1);
                    m_FindPathLocal[pFindPathLocalsHead].nDirection2=(m_FindPathLocal[pFindPathLocalsHead].nDirectionRandom)[m_FindPathLocal[pFindPathLocalsHead].nDirection1];
                    if (m_Room[m_FindPathParameter[pFindPathParametersHead].pRoom].Adjacent.pPassage[m_FindPathLocal[pFindPathLocalsHead].nDirection2] != -1)
                      {
                        if (! m_Passage[m_Room[m_FindPathParameter[pFindPathParametersHead].pRoom].Adjacent.pPassage[m_FindPathLocal[pFindPathLocalsHead].nDirection2]].bBlocked)
                          {
                            if (m_Passage[m_Room[m_FindPathParameter[pFindPathParametersHead].pRoom].Adjacent.pPassage[m_FindPathLocal[pFindPathLocalsHead].nDirection2]].pRoom1 == m_FindPathParameter[pFindPathParametersHead].pRoom)
                              if (m_nFindPathParameters < 3*NUM_ROOMS-2)
                                {
                                  pFindPathParameters=m_nFindPathParameters;
                                  ++m_nFindPathParameters;
                                  m_FindPathParameter[pFindPathParameters].pNext=pFindPathParametersHead;
                                  m_FindPathParameter[pFindPathParameters].pRoom=m_Passage[m_Room[m_FindPathParameter[pFindPathParametersHead].pRoom].Adjacent.pPassage[m_FindPathLocal[pFindPathLocalsHead].nDirection2]].pRoom2;
                                  m_FindPathParameter[pFindPathParameters].nDirectionUsedToEnterRoom=m_FindPathLocal[pFindPathLocalsHead].nDirection2;
                                  m_FindPathParameter[pFindPathParameters].nReturnAddress=1;
                                  pFindPathParametersHead=pFindPathParameters;
                                  bReenter=TRUE;
                                }
                              else
                                bOutOfMemory=TRUE;
                            else
                              if (m_nFindPathParameters < 3*NUM_ROOMS-2)
                                {
                                  pFindPathParameters=m_nFindPathParameters;
                                  ++m_nFindPathParameters;
                                  m_FindPathParameter[pFindPathParameters].pNext=pFindPathParametersHead;
                                  m_FindPathParameter[pFindPathParameters].pRoom=m_Passage[m_Room[m_FindPathParameter[pFindPathParametersHead].pRoom].Adjacent.pPassage[m_FindPathLocal[pFindPathLocalsHead].nDirection2]].pRoom1;
                                  m_FindPathParameter[pFindPathParameters].nDirectionUsedToEnterRoom=m_FindPathLocal[pFindPathLocalsHead].nDirection2;
                                  m_FindPathParameter[pFindPathParameters].nReturnAddress=1;
                                  pFindPathParametersHead=pFindPathParameters;
                                  bReenter=TRUE;
                                }
                              else
                                bOutOfMemory=TRUE;
                          }
                      }
                  }
                if (! bOutOfMemory)
                  {
                    if (! bReenter)
                      {
                        pPathStackHead=m_Path[m_FindPathLocal[pFindPathLocalsHead].pPathStack].pNext;
                        m_FindPathLocal[pFindPathLocalsHead].pPathStack=-1;
                        pFindPathLocals=pFindPathLocalsHead;
                        pFindPathLocalsHead=m_FindPathLocal[pFindPathLocals].pNext;
                        --m_nFindPathLocals;
                        pFindPathLocals=-1;
                      }
                  }
              }
            else
              bReenter=TRUE;
            if (! bOutOfMemory)
              {
                if (bReenter)
                  do
                    {
                      bReenter=FALSE;
                      if (m_nFindPathLocals < 3*NUM_ROOMS-2)
                        {
                          pFindPathLocals=m_nFindPathLocals;
                          ++m_nFindPathLocals;
                          m_FindPathLocal[pFindPathLocals].pNext=pFindPathLocalsHead;
                          pFindPathLocalsHead=pFindPathLocals;
                          if (m_FindPathParameter[pFindPathParametersHead].pRoom == pRoomEntrance)
                            {
                              m_FindPathLocal[pFindPathLocalsHead].nPathLen=0;
                              m_FindPathLocal[pFindPathLocalsHead].pPathStack=pPathStackHead;
                              while (m_FindPathLocal[pFindPathLocalsHead].pPathStack != -1)
                                {
                                  if (m_Room[m_Path[m_FindPathLocal[pFindPathLocalsHead].pPathStack].pRoom].szDescription)
                                    ++(m_FindPathLocal[pFindPathLocalsHead].nPathLen);
                                  m_FindPathLocal[pFindPathLocalsHead].pPathStack=m_Path[m_FindPathLocal[pFindPathLocalsHead].pPathStack].pNext;
                                }
                              if (m_FindPathLocal[pFindPathLocalsHead].nPathLen <= NUM_ROOMS)
                                {
                                  *pszWayOut=&m_szWayOut[0];
                                  (*pszWayOut)[m_FindPathLocal[pFindPathLocalsHead].nPathLen]='\0';
                                  (*pszWayOut)[--(m_FindPathLocal[pFindPathLocalsHead].nPathLen)]=(char) ToUpper((intC) *m_szDirection[m_FindPathParameter[pFindPathParametersHead].nDirectionUsedToEnterRoom]);
                                  m_FindPathLocal[pFindPathLocalsHead].pPathStack=pPathStackHead;
                                  while (m_FindPathLocal[pFindPathLocalsHead].pPathStack != -1)
                                    {
                                      if (m_Path[m_FindPathLocal[pFindPathLocalsHead].pPathStack].pNext != -1)
                                        {
                                          if (m_Room[m_Path[m_FindPathLocal[pFindPathLocalsHead].pPathStack].pRoom].szDescription)
                                            (*pszWayOut)[--(m_FindPathLocal[pFindPathLocalsHead].nPathLen)]=(char) ToUpper((intC) *m_szDirection[m_Path[m_FindPathLocal[pFindPathLocalsHead].pPathStack].nDirectionUsedToEnterRoom]);
                                        }
                                      m_FindPathLocal[pFindPathLocalsHead].pPathStack=m_Path[m_FindPathLocal[pFindPathLocalsHead].pPathStack].pNext;
                                    }
                                }
                              else
                                bOutOfMemory=TRUE;
                            }
                          else
                            {
                              m_FindPathLocal[pFindPathLocalsHead].bRoomAlreadyInStack=FALSE;
                              m_FindPathLocal[pFindPathLocalsHead].pPathStack=pPathStackHead;
                              while ((m_FindPathLocal[pFindPathLocalsHead].pPathStack != -1) && (! (m_FindPathLocal[pFindPathLocalsHead].bRoomAlreadyInStack)))
                                {
                                  m_FindPathLocal[pFindPathLocalsHead].bRoomAlreadyInStack=(m_Path[m_FindPathLocal[pFindPathLocalsHead].pPathStack].pRoom == m_FindPathParameter[pFindPathParametersHead].pRoom);
                                  m_FindPathLocal[pFindPathLocalsHead].pPathStack=m_Path[m_FindPathLocal[pFindPathLocalsHead].pPathStack].pNext;
                                }
                              if (! (m_FindPathLocal[pFindPathLocalsHead].bRoomAlreadyInStack))
                                {
                                  if (m_nPaths < 6*NUM_ROOMS)
                                    {
                                      m_FindPathLocal[pFindPathLocalsHead].pPathStack=m_nPaths;
                                      ++m_nPaths;
                                      m_Path[m_FindPathLocal[pFindPathLocalsHead].pPathStack].pRoom=m_FindPathParameter[pFindPathParametersHead].pRoom;
                                      m_Path[m_FindPathLocal[pFindPathLocalsHead].pPathStack].nDirectionUsedToEnterRoom=m_FindPathParameter[pFindPathParametersHead].nDirectionUsedToEnterRoom;
                                      m_Path[m_FindPathLocal[pFindPathLocalsHead].pPathStack].pNext=pPathStackHead;
                                      pPathStackHead=m_FindPathLocal[pFindPathLocalsHead].pPathStack;
                                      for (m_FindPathLocal[pFindPathLocalsHead].nDirection1=6; (m_FindPathLocal[pFindPathLocalsHead].nDirection1)--;)
                                        (m_FindPathLocal[pFindPathLocalsHead].nDirectionRandom)[m_FindPathLocal[pFindPathLocalsHead].nDirection1]=m_FindPathLocal[pFindPathLocalsHead].nDirection1;
                                      for (m_FindPathLocal[pFindPathLocalsHead].nDirection1=5; m_FindPathLocal[pFindPathLocalsHead].nDirection1 > 0; --(m_FindPathLocal[pFindPathLocalsHead].nDirection1))
                                        {
                                          m_FindPathLocal[pFindPathLocalsHead].nDirection2=RandomNumber()%(m_FindPathLocal[pFindPathLocalsHead].nDirection1+1);
                                          m_FindPathLocal[pFindPathLocalsHead].nDirection3=(m_FindPathLocal[pFindPathLocalsHead].nDirectionRandom)[m_FindPathLocal[pFindPathLocalsHead].nDirection1];
                                          (m_FindPathLocal[pFindPathLocalsHead].nDirectionRandom)[m_FindPathLocal[pFindPathLocalsHead].nDirection1]=(m_FindPathLocal[pFindPathLocalsHead].nDirectionRandom)[m_FindPathLocal[pFindPathLocalsHead].nDirection2];
                                          (m_FindPathLocal[pFindPathLocalsHead].nDirectionRandom)[m_FindPathLocal[pFindPathLocalsHead].nDirection2]=m_FindPathLocal[pFindPathLocalsHead].nDirection3;
                                        }
                                      for (m_FindPathLocal[pFindPathLocalsHead].nDirection1=6; ((! bOutOfMemory) && (! bReenter) && (! *pszWayOut) && ((m_FindPathLocal[pFindPathLocalsHead].nDirection1) > 0));)
                                        {
                                          --(m_FindPathLocal[pFindPathLocalsHead].nDirection1);
                                          m_FindPathLocal[pFindPathLocalsHead].nDirection2=(m_FindPathLocal[pFindPathLocalsHead].nDirectionRandom)[m_FindPathLocal[pFindPathLocalsHead].nDirection1];
                                          if (m_Room[m_FindPathParameter[pFindPathParametersHead].pRoom].Adjacent.pPassage[m_FindPathLocal[pFindPathLocalsHead].nDirection2] != -1)
                                            {
                                              if (! m_Passage[m_Room[m_FindPathParameter[pFindPathParametersHead].pRoom].Adjacent.pPassage[m_FindPathLocal[pFindPathLocalsHead].nDirection2]].bBlocked)
                                                {
                                                  if (m_Passage[m_Room[m_FindPathParameter[pFindPathParametersHead].pRoom].Adjacent.pPassage[m_FindPathLocal[pFindPathLocalsHead].nDirection2]].pRoom1 == m_FindPathParameter[pFindPathParametersHead].pRoom)
                                                    if (m_nFindPathParameters < 3*NUM_ROOMS-2)
                                                      {
                                                        pFindPathParameters=m_nFindPathParameters;
                                                        ++m_nFindPathParameters;
                                                        m_FindPathParameter[pFindPathParameters].pNext=pFindPathParametersHead;
                                                        m_FindPathParameter[pFindPathParameters].pRoom=m_Passage[m_Room[m_FindPathParameter[pFindPathParametersHead].pRoom].Adjacent.pPassage[m_FindPathLocal[pFindPathLocalsHead].nDirection2]].pRoom2;
                                                        m_FindPathParameter[pFindPathParameters].nDirectionUsedToEnterRoom=m_FindPathLocal[pFindPathLocalsHead].nDirection2;
                                                        m_FindPathParameter[pFindPathParameters].nReturnAddress=1;
                                                        pFindPathParametersHead=pFindPathParameters;
                                                        bReenter=TRUE;
                                                      }
                                                    else
                                                      bOutOfMemory=TRUE;
                                                  else
                                                    if (m_nFindPathParameters < 3*NUM_ROOMS-2)
                                                      {
                                                        pFindPathParameters=m_nFindPathParameters;
                                                        ++m_nFindPathParameters;
                                                        m_FindPathParameter[pFindPathParameters].pNext=pFindPathParametersHead;
                                                        m_FindPathParameter[pFindPathParameters].pRoom=m_Passage[m_Room[m_FindPathParameter[pFindPathParametersHead].pRoom].Adjacent.pPassage[m_FindPathLocal[pFindPathLocalsHead].nDirection2]].pRoom1;
                                                        m_FindPathParameter[pFindPathParameters].nDirectionUsedToEnterRoom=m_FindPathLocal[pFindPathLocalsHead].nDirection2;
                                                        m_FindPathParameter[pFindPathParameters].nReturnAddress=1;
                                                        pFindPathParametersHead=pFindPathParameters;
                                                        bReenter=TRUE;
                                                      }
                                                    else
                                                      bOutOfMemory=TRUE;
                                                }
                                            }
                                        }
                                      if (! bReenter)
                                        {
                                          pPathStackHead=m_Path[m_FindPathLocal[pFindPathLocalsHead].pPathStack].pNext;
                                          m_FindPathLocal[pFindPathLocalsHead].pPathStack=-1;
                                        }
                                    }
                                  else
                                    bOutOfMemory=TRUE;
                                }
                            }
                          if (! bReenter)
                            {
                              pFindPathLocals=pFindPathLocalsHead;
                              pFindPathLocalsHead=m_FindPathLocal[pFindPathLocals].pNext;
                              --m_nFindPathLocals;
                              pFindPathLocals=-1;
                            }
                        }
                      else
                        bOutOfMemory=TRUE;
                    }
                  while ((! bOutOfMemory) && bReenter);
              }
            if (! bOutOfMemory)
              {
                nReturnAddress=m_FindPathParameter[pFindPathParametersHead].nReturnAddress;
                pFindPathParameters=pFindPathParametersHead;
                pFindPathParametersHead=m_FindPathParameter[pFindPathParameters].pNext;
                --m_nFindPathParameters;
                pFindPathParameters=-1;
                bResume=TRUE;
              }
          }
        while ((! bOutOfMemory) && nReturnAddress && (nIterations < 1000));
      }
    else
      bOutOfMemory=TRUE;
    m_nPaths=0;
    m_nFindPathLocals=0;
    m_nFindPathParameters=0;
    return;
  }

static void WayOut(int16 nRooms, int16 pRoom, int16 pRoomEntrance, int16 *ppRoomWithName, int8 nTreasures, int8 *bErr)
  {
    int8  bCarryingTreasure;
    int16 nRoom;
    int8  nTreasure;
    char  *szText;
    char  *szWayOut;

    if (pRoom == pRoomEntrance)
      DisplayText("You\'re already at the entrance.",TRUE,TRUE);
    else
      {
        bCarryingTreasure=FALSE;
        for (nTreasure=nTreasures; ((! bCarryingTreasure) && (nTreasure--));)
          bCarryingTreasure=(m_Treasure[nTreasure].pRoomTreasure == -1);
        if (bCarryingTreasure)
          {
            szWayOut=NULL;
            FindPathToEntrance(pRoom,pRoomEntrance,&szWayOut);
            if (szWayOut)
              {
                do
                  nRoom=RandomNumber()%nRooms;
                while (ppRoomWithName[nRoom] == pRoom);
                m_Treasure[nTreasure].pRoomTreasure=ppRoomWithName[nRoom];
                if (1+StrLen("The pirate takes one of your treasures.  As he leaves, he shouts the letters \"")+StrLen(szWayOut)+StrLen("\".") < MSG_BUFFER_LEN)
                  {
                    szText=&m_szMsg[0];
                    StrCpy(szText,"The pirate takes one of your treasures.  ");
                    if (StrLen(szWayOut) == 1)
                      StrCat(szText,"As he leaves, he shouts the letter \"");
                    else
                      StrCat(szText,"As he leaves, he shouts the letters \"");
                    StrCat(szText,szWayOut);
                    StrCat(szText,"\".");
                    DisplayText(szText,TRUE,TRUE);
                  }
                else
                  {
                    *bErr=TRUE;
                    DisplayText("Fatal error:  out of memory",FALSE,TRUE);
                  }
              }
            else
              DisplayText("Nothing happens.",TRUE,TRUE);
          }
        else
          DisplayText("Nothing happens.",TRUE,TRUE);
      }
    return;
  }

static void HideTreasuresAndWeapons(int16 nRooms, int16 *ppRoomWithName, int8  nTreasures)
  {
    int8  bChokepointFound;
    int16 nRoom;
    int8  nTreasure;

    for (nTreasure=0; nTreasure < nTreasures; ++nTreasure)
      {
        bChokepointFound=FALSE;
        nRoom=0;
        while ((! bChokepointFound) && (nRoom < nRooms))
          if (m_Room[ppRoomWithName[nRoom]].nChokepoint == nTreasure)
            bChokepointFound=TRUE;
          else
            ++nRoom;
        if (bChokepointFound)
          {
            do
              nRoom=RandomNumber()%nRooms;
            while (m_Room[ppRoomWithName[nRoom]].nChokepoint != nTreasure);
            m_Treasure[nTreasure].pRoomTreasure=ppRoomWithName[nRoom];
            do
              nRoom=RandomNumber()%(nRooms+1);
            while (m_Room[ppRoomWithName[nRoom]].nChokepoint >= nTreasure);
            m_Treasure[nTreasure].pRoomWeapon=ppRoomWithName[nRoom];
          }
        else
          {
            nRoom=RandomNumber()%nRooms;
            m_Treasure[nTreasure].pRoomTreasure=ppRoomWithName[nRoom];
            m_Treasure[nTreasure].pRoomWeapon=ppRoomWithName[nRooms];
          }
      }
    return;
  }

static intC Command(void)
  {
    intC nResult;
    intC nTem;

    DisplayText("Command? ",FALSE,FALSE);
    nResult=ToUpper(ReadChar());
    nTem=nResult;
    while ((nTem != EOF) && (nTem != (intC) '\n'))
      nTem=ReadChar();
    WriteChar('\n');
    return nResult;
  }

static void ListPassages(int16 pRoom, int8  *bErr)
  {
    int8  nDirection;
    intC  nFirstChar;
    int16 nPassage;
    int16 nPassages;
    int16 pRoomAlongPassage;
    int16 pTreasure;
    char  *szDescription;
    char  szPassages [] = "There are passages North, South, East, West, Up, and Down.";
    char  *szText;

    nPassages=0;
    for (nDirection=6; nDirection--;)
      if ((m_Room[pRoom].Adjacent.pPassage[nDirection] != -1) && (! m_Passage[m_Room[pRoom].Adjacent.pPassage[nDirection]].bBlocked))
        ++nPassages;
    if (nPassages > 1)
      StrCpy(&szPassages[0],"There are passages");
    else
      StrCpy(&szPassages[0],"There is a passage");
    nPassage=0;
    for (nDirection=0; nDirection < 6; ++nDirection)
      if ((m_Room[pRoom].Adjacent.pPassage[nDirection] != -1) && (! m_Passage[m_Room[pRoom].Adjacent.pPassage[nDirection]].bBlocked))
        {
          if (++nPassage > 1)
            if (nPassages != 2)
              StrCat(&szPassages[0],",");
          StrCat(&szPassages[0]," ");
          if (nPassage == nPassages)
            if (nPassages > 1)
              StrCat(&szPassages[0],"and ");
          StrCat(&szPassages[0],m_szDirection[nDirection]);
        }
    StrCat(&szPassages[0],".");
    DisplayText(&szPassages[0],TRUE,TRUE);
    for (nDirection=0; nDirection < 6; ++nDirection)
      {
        szDescription=NULL;
        pRoomAlongPassage=pRoom;
        while ((m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection] != -1) && (! m_Passage[m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection]].bBlocked) && (szDescription == NULL))
          {
            if ((pTreasure=m_Passage[m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection]].pTreasureGuardian) != -1)
              {
                nFirstChar=ToUpper((intC) *(m_Treasure[pTreasure].szGuardian));
                if ((nFirstChar == 'A') || (nFirstChar == 'E') || (nFirstChar == 'I') || (nFirstChar == 'O') || (nFirstChar == 'U'))
                  if (1+StrLen("The passage ")+StrLen(m_szDirection[nDirection])+StrLen(" is guarded by an ")+StrLen(m_Treasure[pTreasure].szGuardian)+StrLen(".") < MSG_BUFFER_LEN)
                    {
                      szText=&m_szMsg[0];
                      StrCpy(szText,"The passage ");
                      StrCat(szText,m_szDirection[nDirection]);
                      StrCat(szText," is guarded by an ");
                      StrCat(szText,m_Treasure[pTreasure].szGuardian);
                      StrCat(szText,".");
                      DisplayText(szText,TRUE,TRUE);
                    }
                  else
                    {
                      *bErr=TRUE;
                      DisplayText("Fatal error:  out of memory",FALSE,TRUE);
                    }
                else
                  if (1+StrLen("The passage ")+StrLen(m_szDirection[nDirection])+StrLen(" is guarded by a ")+StrLen(m_Treasure[pTreasure].szGuardian)+StrLen(".") < MSG_BUFFER_LEN)
                    {
                      szText=&m_szMsg[0];
                      StrCpy(szText,"The passage ");
                      StrCat(szText,m_szDirection[nDirection]);
                      StrCat(szText," is guarded by a ");
                      StrCat(szText,m_Treasure[pTreasure].szGuardian);
                      StrCat(szText,".");
                      DisplayText(szText,TRUE,TRUE);
                    }
                  else
                    {
                      *bErr=TRUE;
                      DisplayText("Fatal error:  out of memory",FALSE,TRUE);
                    }
              }
            if (m_Passage[m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection]].pRoom1 == pRoomAlongPassage)
              pRoomAlongPassage=m_Passage[m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection]].pRoom2;
            else
              pRoomAlongPassage=m_Passage[m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection]].pRoom1;
            szDescription=m_Room[pRoomAlongPassage].szDescription;
          }
      }
    return;
  }

static void ListTreasures(int16 pRoom, int8  nTreasures, int8  *bErr)
  {
    intC nFirstChar;
    int8 nTreasure;
    char *szText;

    for (nTreasure=nTreasures; nTreasure--;)
      if (m_Treasure[nTreasure].pRoomTreasure == pRoom)
        {
          nFirstChar=ToUpper((intC) *(m_Treasure[nTreasure].szTreasure));
          if ((nFirstChar == 'A') || (nFirstChar == 'E') || (nFirstChar == 'I') || (nFirstChar == 'O') || (nFirstChar == 'U'))
            if (1+StrLen("There is an ")+StrLen(m_Treasure[nTreasure].szTreasure)+StrLen(" here.") < MSG_BUFFER_LEN)
              {
                szText=&m_szMsg[0];
                StrCpy(szText,"There is an ");
                StrCat(szText,m_Treasure[nTreasure].szTreasure);
                StrCat(szText," here.");
                DisplayText(szText,TRUE,TRUE);
              }
            else
              {
                *bErr=TRUE;
                DisplayText("Fatal error:  out of memory",FALSE,TRUE);
              }
          else
            if (1+StrLen("There is a ")+StrLen(m_Treasure[nTreasure].szTreasure)+StrLen(" here.") < MSG_BUFFER_LEN)
              {
                szText=&m_szMsg[0];
                StrCpy(szText,"There is a ");
                StrCat(szText,m_Treasure[nTreasure].szTreasure);
                StrCat(szText," here.");
                DisplayText(szText,TRUE,TRUE);
              }
            else
              {
                *bErr=TRUE;
                DisplayText("Fatal error:  out of memory",FALSE,TRUE);
              }
        }
    return;
  }

static void ListWeapons(int16 pRoom, int8  nTreasures, int8  *bErr)
  {
    intC nFirstChar;
    int8 nTreasure;
    char *szText;

    for (nTreasure=nTreasures; nTreasure--;)
      if (m_Treasure[nTreasure].pRoomWeapon == pRoom)
        {
          nFirstChar=ToUpper((intC) *(m_Treasure[nTreasure].szWeapon));
          if ((nFirstChar == 'A') || (nFirstChar == 'E') || (nFirstChar == 'I') || (nFirstChar == 'O') || (nFirstChar == 'U'))
            if (1+StrLen("There is an ")+StrLen(m_Treasure[nTreasure].szWeapon)+StrLen(" here.") < MSG_BUFFER_LEN)
              {
                szText=&m_szMsg[0];
                StrCpy(szText,"There is an ");
                StrCat(szText,m_Treasure[nTreasure].szWeapon);
                StrCat(szText," here.");
                DisplayText(szText,TRUE,TRUE);
              }
            else
              {
                *bErr=TRUE;
                DisplayText("Fatal error:  out of memory",FALSE,TRUE);
              }
          else
            if (1+StrLen("There is a ")+StrLen(m_Treasure[nTreasure].szWeapon)+StrLen(" here.") < MSG_BUFFER_LEN)
              {
                szText=&m_szMsg[0];
                StrCpy(szText,"There is a ");
                StrCat(szText,m_Treasure[nTreasure].szWeapon);
                StrCat(szText," here.");
                DisplayText(szText,TRUE,TRUE);
              }
            else
              {
                *bErr=TRUE;
                DisplayText("Fatal error:  out of memory",FALSE,TRUE);
              }
        }
    return;
  }

static void Carry(int16 pRoom, int8  nTreasures)
  {
    int8 nPickedUp;
    int8 nTreasure;

    nPickedUp=0;
    for (nTreasure=nTreasures; nTreasure--;)
      {
        if (m_Treasure[nTreasure].pRoomWeapon == pRoom)
          {
            ++nPickedUp;
            m_Treasure[nTreasure].pRoomWeapon=-1;
          }
        if (m_Treasure[nTreasure].pRoomTreasure == pRoom)
          {
            ++nPickedUp;
            m_Treasure[nTreasure].pRoomTreasure=-1;
          }
      }
    if (nPickedUp == 0)
      DisplayText("There is nothing to carry.",TRUE,TRUE);
    return;
  }

static void Leave(int16 pRoom, int8  nTreasures)
  {
    int8 nLeft;
    int8 nTreasure;

    nLeft=0;
    for (nTreasure=nTreasures; nTreasure--;)
      if (m_Treasure[nTreasure].pRoomTreasure == -1)
        {
          ++nLeft;
          m_Treasure[nTreasure].pRoomTreasure=pRoom;
        }
    if (nLeft == 0)
      DisplayText("You aren\'t carrying anything to leave.",TRUE,TRUE);
    return;
  }

static void Inventory(int16 nChokepoints, int8  nTreasures, int8  *bErr)
  {
    int8 bFirstLine;
    int8 nTreasure;
    char *szText;

    bFirstLine=TRUE;
    for (nTreasure=nTreasures; nTreasure--;)
      if (m_Treasure[nTreasure].pRoomTreasure == -1)
        {
          if (bFirstLine)
            {
              DisplayText("You are carrying the following:",TRUE,TRUE);
              bFirstLine=FALSE;
            }
          if (1+StrLen("     ")+StrLen(m_Treasure[nTreasure].szTreasure) < MSG_BUFFER_LEN)
            {
              szText=&m_szMsg[0];
              StrCpy(szText,"     ");
              StrCat(szText,m_Treasure[nTreasure].szTreasure);
              DisplayText(szText,FALSE,TRUE);
            }
          else
            {
              *bErr=TRUE;
              DisplayText("Fatal error:  out of memory",FALSE,TRUE);
            }
        }
    for (nTreasure=(char) nChokepoints; nTreasure--;)
      if ((m_Treasure[nTreasure].pRoomWeapon == -1) && (! m_Treasure[nTreasure].bSlain))
        {
          if (bFirstLine)
            {
              DisplayText("You are carrying the following:",TRUE,TRUE);
              bFirstLine=FALSE;
            }
          if (1+StrLen("     ")+StrLen(m_Treasure[nTreasure].szWeapon) < MSG_BUFFER_LEN)
            {
              szText=&m_szMsg[0];
              StrCpy(szText,"     ");
              StrCat(szText,m_Treasure[nTreasure].szWeapon);
              DisplayText(szText,FALSE,TRUE);
            }
          else
            {
              *bErr=TRUE;
              DisplayText("Fatal error:  out of memory",FALSE,TRUE);
            }
        }
    if (bFirstLine)
      DisplayText("You aren\'t carrying anything.",TRUE,TRUE);
    else
      DisplayText(" ",FALSE,TRUE);
    return;
  }

static void Move(int16 *nMoves, int8  nDirection, int16 *pRoom, int8  *bErr)
  {
    int8  bGuarded;
    int16 pRoomAlongPassage;
    char  *szDescription;
    char  *szText;

    if ((m_Room[(*pRoom)].Adjacent.pPassage[nDirection] != -1) && (! m_Passage[m_Room[(*pRoom)].Adjacent.pPassage[nDirection]].bBlocked))
      {
        bGuarded=FALSE;
        szDescription=NULL;
        pRoomAlongPassage=*pRoom;
        while ((m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection] != -1) && (szDescription == NULL))
          {
            if (m_Passage[m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection]].pTreasureGuardian != -1)
              if (m_Treasure[m_Passage[m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection]].pTreasureGuardian].pRoomWeapon != -1)
                if (1+StrLen("You carry nothing to overcome the ")+StrLen(m_Treasure[m_Passage[m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection]].pTreasureGuardian].szGuardian)+StrLen(".") < MSG_BUFFER_LEN)
                  {
                    szText=&m_szMsg[0];
                    bGuarded=TRUE;
                    StrCpy(szText,"You carry nothing to overcome the ");
                    StrCat(szText,m_Treasure[m_Passage[m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection]].pTreasureGuardian].szGuardian);
                    StrCat(szText,".");
                    DisplayText(szText,TRUE,TRUE);
                  }
                else
                  {
                    *bErr=TRUE;
                    DisplayText("Fatal error:  out of memory",FALSE,TRUE);
                  }
              else
                if (1+StrLen("Your ")+StrLen(m_Treasure[m_Passage[m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection]].pTreasureGuardian].szWeapon)+StrLen(" overcomes the ")+StrLen(m_Treasure[m_Passage[m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection]].pTreasureGuardian].szGuardian)+StrLen(".") < MSG_BUFFER_LEN)
                  {
                    szText=&m_szMsg[0];
                    StrCpy(szText,"Your ");
                    StrCat(szText,m_Treasure[m_Passage[m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection]].pTreasureGuardian].szWeapon);
                    StrCat(szText," overcomes the ");
                    StrCat(szText,m_Treasure[m_Passage[m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection]].pTreasureGuardian].szGuardian);
                    StrCat(szText,".");
                    DisplayText(szText,TRUE,TRUE);
                    m_Treasure[m_Passage[m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection]].pTreasureGuardian].bSlain=TRUE;
                    m_Passage[m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection]].pTreasureGuardian=-1;
                  }
                else
                  {
                    *bErr=TRUE;
                    DisplayText("Fatal error:  out of memory",FALSE,TRUE);
                  }
            if (m_Passage[m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection]].pRoom1 == pRoomAlongPassage)
              pRoomAlongPassage=m_Passage[m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection]].pRoom2;
            else
              pRoomAlongPassage=m_Passage[m_Room[pRoomAlongPassage].Adjacent.pPassage[nDirection]].pRoom1;
            szDescription=m_Room[pRoomAlongPassage].szDescription;
          }
        if (! bGuarded)
          {
            ++(*nMoves);
            m_Room[pRoomAlongPassage].bVisited=TRUE;
            *pRoom=pRoomAlongPassage;
          }
      }
    else
      DisplayText("You can\'t go that way.",TRUE,TRUE);
    return;
  }

static void Points(int16 nRooms, int16 pRoomEntrance, int16 *ppRoomWithName, int8  nTreasures, int16 nMoves, int8  bRank, int8  *bErr)
  {
    int16 nRoom;
    int16 nRoomsVisited;
    int16 nScore;
    int8  nTreasure;
    int16 nTreasuresCarried;
    int16 nTreasuresRecovered;
    char  szMoves [12];
    char  szRooms [12];
    char  szRoomsVisited [12];
    char  szScore [12];
    char  *szText;
    char  szTreasures [12];
    char  szTreasuresCarried [12];
    char  szTreasuresRecovered [12];

    IToA((int16) (nRooms+1),&szRooms[0]);
    nScore=0;
    nTreasuresRecovered=0;
    nTreasuresCarried=0;
    nRoomsVisited=0;
    for (nRoom=0; nRoom <= nRooms; ++nRoom)
      if (m_Room[ppRoomWithName[nRoom]].bVisited)
        ++nRoomsVisited;
    IToA(nRoomsVisited,&szRoomsVisited[0]);
    IToA(nMoves,&szMoves[0]);
    if (1+StrLen("You have moved ")+StrLen(&szMoves[0])+StrLen(" times to visit ")+StrLen(&szRoomsVisited[0])+StrLen(" of ")+StrLen(&szRooms[0])+StrLen(" locations.") < MSG_BUFFER_LEN)
      {
        szText=&m_szMsg[0];
        StrCpy(szText,"You have moved ");
        StrCat(szText,&szMoves[0]);
        StrCat(szText," times to visit ");
        StrCat(szText,&szRoomsVisited[0]);
        StrCat(szText," of ");
        StrCat(szText,&szRooms[0]);
        StrCat(szText," locations.");
        DisplayText(szText,FALSE,TRUE);
      }
    else
      {
        *bErr=TRUE;
        DisplayText("Fatal error:  out of memory",FALSE,TRUE);
      }
    if (! *bErr)
      {
        IToA(nTreasures,&szTreasures[0]);
        for (nTreasure=nTreasures; nTreasure--;)
          if (m_Treasure[nTreasure].pRoomTreasure == -1)
            ++nTreasuresCarried;
        IToA(nTreasuresCarried,&szTreasuresCarried[0]);
        if (1+StrLen("You hold ")+StrLen(&szTreasuresCarried[0])+StrLen(" of ")+StrLen(&szTreasures[0])+StrLen(" treasures.") < MSG_BUFFER_LEN)
          {
            szText=&m_szMsg[0];
            StrCpy(szText,"You hold ");
            StrCat(szText,&szTreasuresCarried[0]);
            StrCat(szText," of ");
            StrCat(szText,&szTreasures[0]);
            StrCat(szText," treasures.");
            DisplayText(szText,FALSE,TRUE);
          }
        else
          {
            *bErr=TRUE;
            DisplayText("Fatal error:  out of memory",FALSE,TRUE);
          }
      }
    if (! *bErr)
      {
        for (nTreasure=nTreasures; nTreasure--;)
          if (m_Treasure[nTreasure].pRoomTreasure == pRoomEntrance)
            ++nTreasuresRecovered;
        IToA(nTreasuresRecovered,&szTreasuresRecovered[0]);
        if (1+StrLen("You have returned ")+StrLen(&szTreasuresRecovered[0])+StrLen(" of ")+StrLen(&szTreasures[0])+StrLen(" treasures to the entrance of the mine.") < MSG_BUFFER_LEN)
          {
            szText=&m_szMsg[0];
            StrCpy(szText,"You have returned ");
            StrCat(szText,&szTreasuresRecovered[0]);
            StrCat(szText," of ");
            StrCat(szText,&szTreasures[0]);
            StrCat(szText," treasures to the entrance of the mine.");
            DisplayText(szText,FALSE,TRUE);
          }
        else
          {
            *bErr=TRUE;
            DisplayText("Fatal error:  out of memory",FALSE,TRUE);
          }
      }
    if (! *bErr)
      {
        DisplayText(" ",FALSE,TRUE);
        nScore=25*nRoomsVisited/(nRooms+1)+75*nTreasuresRecovered/nTreasures+45*nTreasuresCarried/nTreasures;
        if (nMoves > 5*nRooms)
          {
            nScore=nScore-nMoves/(5*nRooms);
            if (nScore < 0)
              nScore=0;
          }
        IToA(nScore,&szScore[0]);
        if (1+StrLen("You have scored ")+StrLen(&szScore[0])+StrLen(" of 100 points.") < MSG_BUFFER_LEN)
          {
            szText=&m_szMsg[0];
            StrCpy(szText,"You have scored ");
            StrCat(szText,&szScore[0]);
            StrCat(szText," of 100 points.");
            DisplayText(szText,TRUE,TRUE);
          }
        else
          {
            *bErr=TRUE;
            DisplayText("Fatal error:  out of memory",FALSE,TRUE);
          }
      }
    if (! *bErr)
      {
        if (bRank)
          {
            if (nScore < 25)
              DisplayText("Your score ranks you as a beginner.",FALSE,TRUE);
            else if (nScore < 50)
              DisplayText("Your score ranks you as a novice adventurer.",FALSE,TRUE);
            else if (nScore < 75)
              DisplayText("Your score ranks you as a seasoned explorer.",FALSE,TRUE);
            else if (nScore < 100)
              DisplayText("Your score ranks you as a grissly old prospector.",FALSE,TRUE);
            else
              DisplayText("Your score ranks you as an expert treasure hunter; there is no higher rating.",FALSE,TRUE);
          }
      }
    return;
  }

static void PlayGame(int16 nRooms, int16 pRoomEntrance, int16 *ppRoomWithName, int16 nChokepoints, int8  nTreasures, int8  *bErr)
  {
    intC  nCommand;
    int16 nMoves;
    int16 pRoom;

    nMoves=0;
    pRoom=pRoomEntrance;
    nCommand=(int) 'Q';
    do
      {
        DisplayText(m_Room[pRoom].szDescription,TRUE,TRUE);
        ListTreasures(pRoom,nTreasures,bErr);
        if (! *bErr)
          ListWeapons(pRoom,nTreasures,bErr);
        if (! *bErr)
          ListPassages(pRoom,bErr);
        if (! *bErr)
          {
            nCommand=Command();
            switch (nCommand)
              {
                case (int) 'N':
                  Move(&nMoves,0,&pRoom,bErr);
                  break;
                case (int) 'S':
                  Move(&nMoves,1,&pRoom,bErr);
                  break;
                case (int) 'E':
                  Move(&nMoves,2,&pRoom,bErr);
                  break;
                case (int) 'W':
                  Move(&nMoves,3,&pRoom,bErr);
                  break;
                case (int) 'U':
                  Move(&nMoves,4,&pRoom,bErr);
                  break;
                case (int) 'D':
                  Move(&nMoves,5,&pRoom,bErr);
                  break;
                case (int) 'C':
                  Carry(pRoom,nTreasures);
                  break;
                case (int) 'I':
                  Inventory(nChokepoints,nTreasures,bErr);
                  break;
                case (int) 'L':
                  Leave(pRoom,nTreasures);
                  break;
                case (int) 'P':
                  Points(nRooms,pRoomEntrance,ppRoomWithName,nTreasures,nMoves,FALSE,bErr);
                  break;
                case (int) 'O':
                  WayOut(nRooms,pRoom,pRoomEntrance,ppRoomWithName,nTreasures,bErr);
                  break;
                case (int) 'Q':
                  Points(nRooms,pRoomEntrance,ppRoomWithName,nTreasures,nMoves,TRUE,bErr);
                  break;
                case (int) 'H':
                  DisplayHelp(TRUE);
                  break;
                default:
                  DisplayText("I don\'t recognize that command.",TRUE,TRUE);
                  ListCommands();
                  break;
              }
          }
      }
    while ((! *bErr) && (nCommand != (int) 'Q'));
    return;
  }

int main(void)
  {
    int8  bErr;
    int16 nChokepoints;
    int16 nRooms;
    int8  nTreasures;
    int16 pRoomEntrance;
    int16 *ppRoomWithName;

    bErr=FALSE;
    if (m_nRooms < 3*NUM_ROOMS-2)
      {
        pRoomEntrance=m_nRooms;
        ++m_nRooms;
        m_Room[pRoomEntrance].szDescription="You\'re in the entrance to the mine.";
        m_Room[pRoomEntrance].bMined=TRUE;
        m_Room[pRoomEntrance].nChokepoint=-1;
        m_Room[pRoomEntrance].bVisited=TRUE;
        m_Room[pRoomEntrance].Adjacent.Passage.pPassageNorth=-1;
        m_Room[pRoomEntrance].Adjacent.Passage.pPassageSouth=-1;
        m_Room[pRoomEntrance].Adjacent.Passage.pPassageEast=-1;
        m_Room[pRoomEntrance].Adjacent.Passage.pPassageWest=-1;
        m_Room[pRoomEntrance].Adjacent.Passage.pPassageUp=-1;
        m_Room[pRoomEntrance].Adjacent.Passage.pPassageDown=-1;
        SeedRandomNumberGenerator();
        nRooms=NUM_ROOMS;
        if (! bErr)
          ShuffleRoomDescriptions(nRooms,&m_szRoom[0]);
        nTreasures=NUM_TREASURES;
        nChokepoints=0;
        if (! bErr)
          {
            ppRoomWithName=&m_pRoomWithName[0];
            ExcavateMine(nRooms,pRoomEntrance,nTreasures,&nChokepoints,ppRoomWithName,&bErr);
            if (! bErr)
              HideTreasuresAndWeapons(nRooms,ppRoomWithName,nTreasures);
            if (! bErr)
              PlayGame(nRooms,pRoomEntrance,ppRoomWithName,nChokepoints,nTreasures,&bErr);
          }
      }
    else
      {
        bErr=TRUE;
        DisplayText("Fatal error:  out of memory",FALSE,TRUE);
      }
    return bErr;
  }
