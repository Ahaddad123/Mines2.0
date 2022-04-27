using Xunit;
using Mines2._0.Entity;
using Mines2._0;
using System.Collections.Generic;

namespace Mines2._0Test
{
    public class CaveTests
    {
        [Fact]
        public void TestEqualsTrueSameEverything()
        {
            Cave cave1 = new Cave(2, 5, 3);
            Cave cave2 = new Cave(2, 5, 3);
            Assert.True(cave1.Equals(cave2));
        }

        [Fact]
        public void TestEqualsTrueDifferentItemList()
        {
            Cave cave1 = new Cave(2, 5, 3);
            Cave cave2 = new Cave(2, 5, 3);
            cave1.items.Add(new Item(0, "Sample Item Description 1"));
            cave2.items.Add(new Item(1, "Sample Item Description 2"));
            Assert.True(cave1.Equals(cave2));
        }

        [Fact]
        public void TestEqualsTrueDifferentTreasureList()
        {
            Cave cave1 = new Cave(2, 5, 3);
            Cave cave2 = new Cave(2, 5, 3);
            cave1.treasures.Add(new Treasure(0, "Sample Treasure Description 1"));
            cave2.treasures.Add(new Treasure(1, "Sample Treasure Description 2"));
            Assert.True(cave1.Equals(cave2));
        }

        [Fact]
        public void TestEqualsTrueDifferentAdjacentCaves()
        {
            Cave cave1 = new Cave(2, 5, 3);
            Cave cave2 = new Cave(2, 5, 3);
            cave1.getAdjacentCaves()[0] = new Cave(4, 0, 9);
            cave2.getAdjacentCaves()[0] = new Cave(4, 0, 4);
            Assert.True(cave1.Equals(cave2));
        }

        [Fact]
        public void TestEqualsTrueDifferentIsExplored()
        {
            Cave cave1 = new Cave(2, 5, 3);
            Cave cave2 = new Cave(2, 5, 3);
            cave1.isExplored = false;
            cave2.isExplored = true;
            Assert.True(cave1.Equals(cave2));
        }

        [Fact]
        public void TestEqualsTrueDifferentObstacles()
        {
            Cave cave1 = new Cave(2, 5, 3);
            Cave cave2 = new Cave(2, 5, 3);
            cave1.caveObstacle = new Obstacle(0, "Sample Obstacle Description 1");
            cave2.caveObstacle = new Obstacle(1, "Sample Obstacle Description 2");
            Assert.True(cave1.Equals(cave2));
        }

        [Fact]
        public void TestEqualsTrueDifferentDescriptionIDs()
        {
            Cave cave1 = new Cave(2, 5, 3);
            Cave cave2 = new Cave(2, 5, 3);
            cave1.descriptionID = "Sample Description ID 1";
            cave2.descriptionID = "Sample Description ID 2";
            Assert.True(cave1.Equals(cave2));
        }

        [Fact]
        public void TestEqualsFalseDifferentX()
        {
            Cave cave1 = new Cave(2, 5, 3);
            Cave cave2 = new Cave(4, 5, 3);
            Assert.False(cave1.Equals(cave2));
        }

        [Fact]
        public void TestEqualsFalseDifferentY()
        {
            Cave cave1 = new Cave(2, 5, 3);
            Cave cave2 = new Cave(2, 0, 3);
            Assert.False(cave1.Equals(cave2));
        }

        [Fact]
        public void TestEqualsFalseDifferentZ()
        {
            Cave cave1 = new Cave(2, 5, 3);
            Cave cave2 = new Cave(2, 5, -3);
            Assert.NotEqual(cave1, cave2);
        }

        [Fact]
        public void TestAddItem()
        {
            Cave cave = new Cave();
            Item item = new Item(0, "Sample Item Description");
            cave.addDroppedItem(item);
            Assert.Contains(item, cave.items);
        }

        [Fact]
        public void TestAddTreasure()
        {
            Cave cave = new Cave();
            Treasure treasure = new Treasure(0, "Sample Treasure Description");
            cave.addDroppedTreasure(treasure);
            Assert.Contains(treasure, cave.treasures);
        }

        [Fact]
        public void TestPickUpItemNoItems()
        {
            Cave cave = new Cave();
            List<Item> itemList = cave.pickUpItem();
            Assert.Empty(cave.items);
            Assert.Empty(itemList);
        }

        [Fact]
        public void TestPickUpItemOneItem()
        {
            Cave cave = new Cave();
            Item item = new Item(0, "Sample Item Description");
            cave.items.Add(item);
            List<Item> itemList = cave.pickUpItem();
            Assert.Contains(item, itemList);
            Assert.DoesNotContain(item, cave.items);
        }

        [Fact]
        public void TestPickUpItemMultipleItems()
        {
            Cave cave = new Cave();
            Item item1 = new Item(0, "Sample Item Description 1");
            Item item2 = new Item(1, "Sample Item Description 2");
            cave.items.Add(item1);
            cave.items.Add(item2);
            List<Item> itemList = cave.pickUpItem();
            Assert.Contains(item1, itemList);
            Assert.Contains(item2, itemList);
            Assert.DoesNotContain(item1, cave.items);
            Assert.DoesNotContain(item2, cave.items);
        }

        [Fact]
        public void TestPickUpTreasureNoTreasures()
        {
            Cave cave = new Cave();
            List<Treasure> treasureList = cave.pickUpTreasure();
            Assert.Empty(cave.treasures);
            Assert.Empty(treasureList);
        }

        [Fact]
        public void TestPickUpTreasureOneTreasure()
        {
            Cave cave = new Cave();
            Treasure treasure = new Treasure(0, "Sample Treasure Description");
            cave.treasures.Add(treasure);
            List<Treasure> treasureList = cave.pickUpTreasure();
            Assert.Contains(treasure, treasureList);
            Assert.DoesNotContain(treasure, cave.treasures);
        }

        [Fact]
        public void TestPickUpTreasureMultipleTreasures()
        {
            Cave cave = new Cave();
            Treasure treasure1 = new Treasure(0, "Sample Treasure Description 1");
            Treasure treasure2 = new Treasure(1, "Sample Treasure Description 2");
            cave.treasures.Add(treasure1);
            cave.treasures.Add(treasure2);
            List<Treasure> treasureList = cave.pickUpTreasure();
            Assert.Contains(treasure1, treasureList);
            Assert.Contains(treasure2, treasureList);
            Assert.DoesNotContain(treasure1, cave.treasures);
            Assert.DoesNotContain(treasure2, cave.treasures);
        }
    }
}
