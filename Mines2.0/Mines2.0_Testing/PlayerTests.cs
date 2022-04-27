using Xunit;
using Moq;
using Mines2._0.Entity;
using Mines2._0;

namespace Mines2._0Test
{
    /// <summary>
    /// Testing methods from the Obstacle class
    /// </summary>
    public class PlayerTests
    {
        /// <summary>
        /// Tests picking up one item in cave
        /// </summary>
        [Fact]
        public void Test_PickUpItem_ItemInCave()
        {
            int actualItemCount = -1;
            int expectedItemCount = 1;
            Cave cave = new Cave();
            Item item = new Item(0, "Item1");
            cave.addDroppedItem(item);

            var mock = new Mock<Player>(cave);
            Player player = mock.Object;

            player.pickUpItems();
            actualItemCount = player.getItemInventory().Count;

            Assert.Equal(expectedItemCount, actualItemCount);
        }
        /// <summary>
        /// Test picking up no items in cave
        /// </summary>
        [Fact]
        public void Test_PickUpItem_NoItemInCave()
        {
            int actualItemCount = -1;
            int expectedItemCount = 0;
            Cave cave = new Cave();

            var mock = new Mock<Player>(cave);
            Player player = mock.Object;

            player.pickUpItems();
            actualItemCount = player.getItemInventory().Count;

            Assert.Equal(expectedItemCount, actualItemCount);
        }
        /// <summary>
        /// Tests picking up multiple items in cave
        /// </summary>
        [Fact]
        public void Test_PickUpItem_MultipleItemInCave()
        {
            int actualItemCount = -1;
            int expectedItemCount = 3;
            Cave cave = new Cave();
            Item item1 = new Item(0, "Item1");
            Item item2 = new Item(1, "Item2");
            Item item3 = new Item(2, "Item3");
            cave.addDroppedItem(item1);
            cave.addDroppedItem(item2);
            cave.addDroppedItem(item3);

            var mock = new Mock<Player>(cave);
            Player player = mock.Object;

            player.pickUpItems();
            actualItemCount = player.getItemInventory().Count;

            Assert.Equal(expectedItemCount, actualItemCount);
        }
        /// <summary>
        /// Test picking up with one treasure in cave
        /// </summary>
        [Fact]
        public void Test_PickUpTresure_TreasureInCave()
        {
            int actualTresureCount = -1;
            int expectedTresureCount = 1;
            Cave cave = new Cave();
            Treasure treasure = new Treasure(0, "Treasure1");
            cave.addDroppedTreasure(treasure);

            var mock = new Mock<Player>(cave);
            Player player = mock.Object;

            player.pickUpTreasure();
            actualTresureCount = player.getTreasureInventory().Count;

            Assert.Equal(expectedTresureCount, actualTresureCount);
        }
        /// <summary>
        /// Test picking up with no tresures in cave
        /// </summary>
        [Fact]
        public void Test_PickUpTresure_NoTreasureInCave()
        {
            int actualTresureCount = -1;
            int expectedTresureCount = 0;
            Cave cave = new Cave();

            var mock = new Mock<Player>(cave);
            Player player = mock.Object;

            player.pickUpTreasure();
            actualTresureCount = player.getTreasureInventory().Count;

            Assert.Equal(expectedTresureCount, actualTresureCount);
        }
        /// <summary>
        /// Tests picking up with multiple tresures in cave
        /// </summary>
        [Fact]
        public void Test_PickUpTresure_MultipleTreasureInCave()
        {
            int actualTresureCount = -1;
            int expectedTresureCount = 3;
            Cave cave = new Cave();
            Treasure treasure1 = new Treasure(0, "Treasure1");
            Treasure treasure2 = new Treasure(1, "Treasure2");
            Treasure treasure3 = new Treasure(2, "Treasure3");
            cave.addDroppedTreasure(treasure1);
            cave.addDroppedTreasure(treasure2);
            cave.addDroppedTreasure(treasure3);

            var mock = new Mock<Player>(cave);
            Player player = mock.Object;

            player.pickUpTreasure();
            actualTresureCount = player.getTreasureInventory().Count;

            Assert.Equal(expectedTresureCount, actualTresureCount);
        }
        /// <summary>
        /// Testing the canRemoveObstacle method when the player has the 
        /// correct item
        /// </summary>
        [Fact]
        public void Test_Player_CanRemoveObstacle_PlayerHasCorrectItem()
		{
            Cave cave = new Cave(1, 1, 1);
            Player player = new Player(cave);
            player.getItemInventory().Add(new Item(1, "Item"));
            Obstacle obstacle = new Obstacle(1, "Obstacle");

            Assert.True(player.canRemoveObstacle(obstacle));
		}
        /// <summary>
        /// Testing the canRemoveObstacle method when the player does
        /// not have the correct item
        /// </summary>
        [Fact]
        public void Test_Player_CanRemoveObstacle_PlayerLacksCorrectItem()
		{
            Cave cave = new Cave(1, 1, 1);
            Player player = new Player(cave);
            
            Obstacle obstacle = new Obstacle(1, "Obstacle");

            Assert.False(player.canRemoveObstacle(obstacle));
        }

        [Fact]
        public void Test_Player_CanRemoveObstacle_PlayerHasCorrectItemFromMultipleItems()
        {
            Cave cave = new Cave(1, 1, 1);
            Player player = new Player(cave);
            Obstacle obstacle = new Obstacle(1, "Obstacle");

            player.getItemInventory().Add(new Item(1, "Item"));
            player.getItemInventory().Add(new Item(2, "Item"));
            Assert.Equal(2, player.getItemInventory().Count);


            Assert.True(player.canRemoveObstacle(obstacle));
        }
        [Fact]
        public void Test_Player_CanRemoveObstacle_PlayerLacksCorrectItemFromMultipleItems()
        {
            Cave cave = new Cave(1, 1, 1);
            Player player = new Player(cave);
            Obstacle obstacle = new Obstacle(1, "Obstacle");

            player.getItemInventory().Add(new Item(3, "Item"));
            player.getItemInventory().Add(new Item(2, "Item"));
            Assert.Equal(2, player.getItemInventory().Count);


            Assert.False(player.canRemoveObstacle(obstacle));
            Assert.Equal(2, player.getItemInventory().Count);
        }

        [Fact]
        public void TestPointCalculationZeroAll()
        {
            int numTreasures = 1; // can't be zero due to division.
            int numMoves = 0;
            int numRooms = 0;
            int treasures_recovered = 0;

            Cave cave = new Cave();
            Player player = new Player(cave);
            Assert.Equal(0, player.calculatePoints(numTreasures, numMoves, numRooms, treasures_recovered));
        }


        [Fact]
        public void TestPointCalculation_AllRoomsVisited_AllTreasuresRecovered_MoveRedution()
        {
            int numTreasures = 10;
            int numMoves = 60;          // Must be > 5 * numRooms.
            int numRooms = 10;
            int treasures_recovered = 10;

            Cave cave = new Cave();
            var mock = new Mock<Player>(cave);
            mock.SetupGet(x => x.roomsVisited).Returns(numRooms);  // Force the player object to have "visited" all rooms.
            Player player = mock.Object;
            Assert.Equal(96, player.calculatePoints(numTreasures, numMoves, numRooms, treasures_recovered));
        }

        [Fact]
        public void TestPointCalculation_AllRoomsVisited_AllTreasuresRecovered_ExactMoves()
        {
            int numTreasures = 10;
            int numMoves = 50;          // Right on the edge of reducing points, results in no point reduction.
            int numRooms = 10; 
            int treasures_recovered = 10;

            Cave cave = new Cave();
            var mock = new Mock<Player>(cave);
            mock.SetupGet(x => x.roomsVisited).Returns(numRooms);  // Force the player object to have "visited" all rooms.
            Player player = mock.Object;
            Assert.Equal(97, player.calculatePoints(numTreasures, numMoves, numRooms, treasures_recovered));
        }

        [Fact]
        public void TestPointCalculation_AllRoomsVisited_NoTreasuresRecovered_NoReduction()
        {
            int numTreasures = 10;
            int numMoves = 45;          // Results in no point reduction.
            int numRooms = 10;
            int treasures_recovered = 0;

            Cave cave = new Cave();
            var mock = new Mock<Player>(cave);
            mock.SetupGet(x => x.roomsVisited).Returns(numRooms);  // Force the player object to have "visited" all rooms.
            Player player = mock.Object;
            Assert.Equal(22, player.calculatePoints(numTreasures, numMoves, numRooms, treasures_recovered));
        }


        [Fact]
        public void Test_Player_DropItem_OneItem()
        {
         int actualItemCount = -1;
         int expectedItemCount = 2;
         Cave cave = new Cave();
         Item item1 = new Item(0, "Item1");
         Item item2 = new Item(1, "Item2");
         Item item3 = new Item(2, "Item3");
         cave.addDroppedItem(item1);
         cave.addDroppedItem(item2);
         cave.addDroppedItem(item3);
       
         var mock = new Mock<Player>(cave);
         Player player = mock.Object;
         player.pickUpItems();

         player.dropItem(2);
         actualItemCount = player.getItemInventory().Count;

         Assert.Equal(expectedItemCount, actualItemCount);
        }

        [Fact]
        public void Test_Player_DropItem_EmptyInventory()
        {
         int actualItemCount = -1;
         int expectedItemCount = 0;
         Cave cave = new Cave();

         var mock = new Mock<Player>(cave);
         Player player = mock.Object;
         player.pickUpItems();

         player.dropItem(2);
         actualItemCount = player.getItemInventory().Count;

         Assert.Equal(expectedItemCount, actualItemCount);
        }

        [Fact]
        public void Test_Player_DropTreasure_EmptyInventory()
        {
         int actualTresureCount = -1;
         int expectedTresureCount = 0;
         Cave cave = new Cave();

         var mock = new Mock<Player>(cave);
         Player player = mock.Object;

         player.dropTreasures(1);

         actualTresureCount = player.getTreasureInventory().Count;

         Assert.Equal(expectedTresureCount, actualTresureCount);
        }

        [Fact]
        public void TestDropTreasures_TreasureInInventory()
        {
         Cave cave = new Cave();
         Treasure treasure1 = new Treasure(0, "Treasure1");
         Treasure treasure2 = new Treasure(1, "Treasure2");
         Treasure treasure3 = new Treasure(2, "Treasure3");
         cave.addDroppedTreasure(treasure1);
         cave.addDroppedTreasure(treasure2);
         cave.addDroppedTreasure(treasure3);

         var mock = new Mock<Player>(cave);
         Player player = mock.Object;

         player.pickUpTreasure();
         player.dropTreasures(0); // drops treasure at first index in list.
         player.dropTreasures(0); // drops new treasure that was shifted into the first index.

         int endingTreasureCount = player.getTreasureInventory().Count;
         string endingTreasureDescription = player.getTreasureInventory()[0].getDescription();

         int expectedTreasureCount = 1;
         string expectedTreasureDescription = "Treasure3";

         Assert.Equal(expectedTreasureCount, endingTreasureCount);
         Assert.Equal(expectedTreasureDescription, endingTreasureDescription);
        }
   }
}
