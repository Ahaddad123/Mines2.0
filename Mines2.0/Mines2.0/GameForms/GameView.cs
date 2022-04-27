using Mines2._0.Boundary;
using Mines2._0.Control;

namespace Mines2._0.GameForms
{
	/// <summary>
	/// This class is responsible for handling the events triggered on the
	/// GameView form
	/// </summary>
	public partial class GameView : Form
	{
		private IOManager IO;
		private MinesController controller;
		private const int MIN_PLAYER_MAX_TURNS = 30;
		/// <summary>
		/// Default constructor for 
		/// </summary>
		public GameView()
		{
			InitializeComponent();
			controller = new MinesController(consoleTextBox, userInputTextBox, turnLabel);
			IO = IOManager.getInstance();
			turnLabel.Text = "Turn " + controller.playerTurns.ToString();
			IO.getOutputStream().writeToTextBox("Enter the maximum number of turns to spend in the mine.", consoleTextBox);
		}
		/// <summary>
		///  Handles where the map panel should be located when a user presses
		///  a button.If the map is at the back, bring it to the front
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void mapButton_Click(object sender, EventArgs e)
		{
			if (mapInFront)
			{
				consoleBox.BringToFront();
				consoleBox.Size = new Size(mapBox.Width, mapBox.Bounds.Height);
				consoleBox.Location = new Point(mapBox.Location.X, mapBox.Location.Y);
				mapInFront = false;
			}
			else
			{
				//mapBox.BringToFront();
				RedrawSmallConsoleBox();
				mapInFront = true;
			}
		}
		/// <summary>
		/// Responsible for drawing the lines on the map for showing the rooms
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void mapBox_Paint(object sender, PaintEventArgs e)
		{
			
			controller.drawMap(e, this.mapBox.Width, this.mapBox.Height);
		}
		/// <summary>
		/// Event triggers when the user presses a key in the userInputTextBox
		/// If the user presses enter, this function will have unique functionality 
		/// revolving around parsing the command
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void userInputTextBox_KeyDown(object sender, KeyEventArgs e)
		{
			if (controller.maximumPlayerTurns == 0)
            {
				GetTurnAmountFromUser(sender, e);
			}
			else if (controller.getCaveSeed() < 0)
			{
				GetSeedFromUser(sender, e);
			}
			else if (e.KeyCode==Keys.Enter)
			{
				if (userInputTextBox.Text.Length > 0)
				{
					String sample = IO.getInputStream().readTextBox(userInputTextBox); 

					IO.getOutputStream().writeToTextBox(sample, consoleTextBox);
					controller.parseCommand(sample);
					if (Char.ToUpper(sample[0]) != 'Q' && !controller.dropItemFlag && !controller.dropTreasureFlag)
						controller.outputRoomInfo();
					if (controller.playerTurns >= controller.maximumPlayerTurns)
					{
						turnLabel.Text = "Turn " + controller.playerTurns.ToString();
						turnLabel.Refresh();
						IOManager.getInstance().getOutputStream().writeToTextBox(Environment.NewLine, consoleTextBox);
						IOManager.getInstance().getOutputStream().writeToTextBox("After spending too many turns in the mine, your free trial of life has ended!", consoleTextBox);
						IOManager.getInstance().getOutputStream().writeToTextBox("Thanks for playing! Here are your results: ", consoleTextBox);
						controller.Points();
						IOManager.getInstance().getOutputStream().writeToTextBox("Game will close automatically in 10 seconds", consoleTextBox);
						Thread.Sleep(10000);
						Environment.Exit(0);
					}
					inventoryBox.Text = controller.getPlayerTreasures();
				}
				e.Handled = true;	
				e.SuppressKeyPress = true;
	
			}
			if (controller.getPlayer() != null)
			{
				inventoryBox.Items.Clear();
				inventoryBox.Items.Add("TREASURES");
				inventoryBox.Items.AddRange(controller.getPlayer().getTreasureList().Select(x => x.getDescription()).ToArray());
				inventoryBox.Items.Add("ITEMS");
				inventoryBox.Items.AddRange(controller.getPlayer().getItemList().Select(x => x.getDescription()).ToArray());
			}
			turnLabel.Text = "Turn " + controller.playerTurns.ToString();
			this.Refresh();
		}
		/// <summary>
		/// Gets the seed from the user. If the seed is not an int, the program will
		/// tell the user to enter a valid input
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void GetSeedFromUser(object sender, KeyEventArgs e)
		{
			// -------- The first command must be a valid mine number for seeding cave generator -------- \\
		
			if (e.KeyCode == Keys.Enter)
			{
				if (userInputTextBox.Text.Length > 0)
				{
					String sample = IO.getInputStream().readTextBox(userInputTextBox);
					IO.getOutputStream().writeToTextBox(sample, consoleTextBox);

					if (sample.ToLower().StartsWith("q"))
						Application.Exit();

					if (controller.convertToInt(sample) == -1 || controller.convertToInt(sample) < 0 || !(int.TryParse(sample, out int value)))
					{
						IO.getOutputStream().writeToTextBox("Please enter a valid number", consoleTextBox);
					}
					else
					{
						controller.initializeGame(controller.convertToInt(sample));
						controller.outputRoomInfo();
					}

				}
				e.Handled = true;
				e.SuppressKeyPress = true;
			}
		}
		/// <summary>
		/// Gets the number of turns the player can spend in the mine from the user. If the number is not an int or is too small, the program will
		/// tell the user to enter a valid input
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void GetTurnAmountFromUser(object sender, KeyEventArgs e)
        {
			if (e.KeyCode == Keys.Enter)
			{
				if (userInputTextBox.Text.Length > 0)
				{
					String sample = IO.getInputStream().readTextBox(userInputTextBox);
					IO.getOutputStream().writeToTextBox(sample, consoleTextBox);

					if (sample.ToLower().StartsWith("q"))
						Application.Exit();

					if (controller.convertToInt(sample) == -1 || controller.convertToInt(sample) < MIN_PLAYER_MAX_TURNS || !(int.TryParse(sample, out int value)))
					{
						IO.getOutputStream().writeToTextBox($"Please enter a valid number. Number must be at least {MIN_PLAYER_MAX_TURNS}.", consoleTextBox);
					}
					else
					{
						controller.maximumPlayerTurns = controller.convertToInt(sample);
						IO.getOutputStream().writeToTextBox("Enter Mine Number.", consoleTextBox);
					}

				}
				e.Handled = true;
				e.SuppressKeyPress = true;
			}
		}

		private void GameView_Resize(object sender, EventArgs e)
		{
			//Force redraw
			mapBox.Invalidate();
			consoleBox.Invalidate();
			if(mapInFront)
				RedrawSmallConsoleBox();
		}
		private void RedrawSmallConsoleBox()
		{	
			consoleBox.Size = new Size(mapBox.Width, mapBox.Bounds.Height / 6);
			consoleBox.Location = new Point(mapBox.Location.X, mapBox.Location.Y + mapBox.Height - (mapBox.Bounds.Height / 6));

		}
	}
}
