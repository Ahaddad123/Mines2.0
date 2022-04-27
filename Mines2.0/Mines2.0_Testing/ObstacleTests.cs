using Xunit;
using Moq;
using Mines2._0;
using Mines2._0.Entity;

namespace Mines2._0Test
{
    /// <summary>
    /// Testing methods from the Obstacle class
    /// </summary>
    public class ObstacleTests
    {
        /// <summary>
        /// Testing the RemoveObstacle method when the player has the valid
        /// item to remove the obstacle
        /// </summary>
        [Fact]
        public void Test_Obstacle_RemoveObstacle_PlayerHasCorrectItem()
        {
            Cave cave = new Cave(1, 1, 1);
            Mock<Player> mockPlayer = new Mock<Player>(cave);
            Obstacle obstacle = new Obstacle(1, "Test Obstacle");
            mockPlayer.Object.getItemInventory().Add(new Item(1, "Item"));
            Assert.Single(mockPlayer.Object.getItemInventory());

            mockPlayer.Setup(x => x.canRemoveObstacle(obstacle)).Returns(true);
            Assert.True(obstacle.removeObstacle(mockPlayer.Object));


            mockPlayer.Verify(x => x.canRemoveObstacle(obstacle), Times.Once());
            Assert.Empty(mockPlayer.Object.getItemInventory());
        }
        /// <summary>
        /// Testing the RemoveObstacle method when the player does not has the 
        /// valid item to remove the obstacle
        /// </summary>
        [Fact]
        public void Test_Obstacle_RemoveObstacle_PlayerLacksCorrectItem()
        {
            Cave cave = new Cave(1, 1, 1);
            Mock<Player> mockPlayer = new Mock<Player>(cave);
            Obstacle obstacle = new Obstacle(1, "Test Obstacle");
            mockPlayer.Object.getItemInventory().Add(new Item(2, "Item"));
            Assert.Single(mockPlayer.Object.getItemInventory());

            mockPlayer.Setup(x => x.canRemoveObstacle(obstacle)).Returns(false);

            Assert.False(obstacle.removeObstacle(mockPlayer.Object));


            mockPlayer.Verify(x => x.canRemoveObstacle(obstacle), Times.Once());
            Assert.Single(mockPlayer.Object.getItemInventory());
        }
        [Fact]
        public void Test_Obstacle_RemoveObstacle_PlayerHasNoItems()
        {
            Cave cave = new Cave(1, 1, 1);
            Mock<Player> mockPlayer = new Mock<Player>(cave);
            Obstacle obstacle = new Obstacle(1, "Test Obstacle");
            Assert.Empty(mockPlayer.Object.getItemInventory());

            mockPlayer.Setup(x => x.canRemoveObstacle(obstacle)).Returns(false);

            Assert.False(obstacle.removeObstacle(mockPlayer.Object));


            mockPlayer.Verify(x => x.canRemoveObstacle(obstacle), Times.Once());
            Assert.Empty(mockPlayer.Object.getItemInventory());
        }
        [Fact]
        public void Test_Obstacle_RemoveObstacle_PlayerHasCorrectItemAndHasMultipleItems()
        {
            Cave cave = new Cave(1, 1, 1);
            Mock<Player> mockPlayer = new Mock<Player>(cave);
            Obstacle obstacle = new Obstacle(1, "Test Obstacle");
            mockPlayer.Object.getItemInventory().Add(new Item(1, "Item"));
            mockPlayer.Object.getItemInventory().Add(new Item(2, "Item"));
            Assert.Equal(2, mockPlayer.Object.getItemInventory().Count);

            mockPlayer.Setup(x => x.canRemoveObstacle(obstacle)).Returns(true);

            Assert.True(obstacle.removeObstacle(mockPlayer.Object));
                
            mockPlayer.Verify(x => x.canRemoveObstacle(obstacle), Times.Once());
            Assert.Single(mockPlayer.Object.getItemInventory());
        }
        [Fact]
        public void Test_Obstacle_RemoveObstacle_PlayerLacksCorrectItemAndHasMultipleItems()
        {
            Cave cave = new Cave(1, 1, 1);
            Mock<Player> mockPlayer = new Mock<Player>(cave);
            Obstacle obstacle = new Obstacle(1, "Test Obstacle");
            mockPlayer.Object.getItemInventory().Add(new Item(3, "Item"));
            mockPlayer.Object.getItemInventory().Add(new Item(2, "Item"));
            Assert.Equal(2, mockPlayer.Object.getItemInventory().Count);

            mockPlayer.Setup(x => x.canRemoveObstacle(obstacle)).Returns(false);

            Assert.False(obstacle.removeObstacle(mockPlayer.Object));


            mockPlayer.Verify(x => x.canRemoveObstacle(obstacle), Times.Once());
            Assert.Equal(2, mockPlayer.Object.getItemInventory().Count);
        }
    }
}
