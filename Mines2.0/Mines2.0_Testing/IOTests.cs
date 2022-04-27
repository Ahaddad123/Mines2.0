using Xunit;
using Moq;
using Mines2._0.Boundary;
using System.Windows.Forms;

namespace Mines2._0Test
{
    /// <summary>
    /// These unit tests cover reading/writing from our IOManager subclasses.
    /// </summary>
    public class IOTests
    {
        [Fact]
        public void testReadTextBox()
        {
         var text = "test";
         var mockTextBox = new Mock<TextBox>();
         mockTextBox.Setup(x => x.Text).Returns(text);

         ConsoleReader console = new ConsoleReader();

         Assert.Equal(text, console.readTextBox(mockTextBox.Object));
        }

      [Fact]
        public void testWriteToTextBox()
        {
         var expected = "test";
         TextBox textBox = new TextBox();
         ConsoleWriter console = new ConsoleWriter();

         console.writeToTextBox(expected.TrimEnd(), textBox);
         var actual = textBox.Text.TrimEnd();

         Assert.Equal(expected, actual);
        }
   }
}
