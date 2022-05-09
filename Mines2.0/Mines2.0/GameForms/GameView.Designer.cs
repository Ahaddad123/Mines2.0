using Mines2._0.Boundary;

namespace Mines2._0.GameForms
{
	partial class GameView
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.label1 = new System.Windows.Forms.Label();
            this.userInputTextBox = new System.Windows.Forms.TextBox();
            this.treasureBox = new System.Windows.Forms.ListBox();
            this.turnLabel = new System.Windows.Forms.Label();
            this.inventoryLabel = new System.Windows.Forms.Label();
            this.weaponBox = new System.Windows.Forms.ListBox();
            this.inputLabel = new System.Windows.Forms.Label();
            this.moveLabel = new System.Windows.Forms.Label();
            this.mapBox = new System.Windows.Forms.Panel();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.consoleTextBox = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.BackColor = System.Drawing.Color.Pink;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 36F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point);
            this.label1.Location = new System.Drawing.Point(2, 3);
            this.label1.MaximumSize = new System.Drawing.Size(11429, 133333);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(1108, 76);
            this.label1.TabIndex = 5;
            this.label1.Text = "Mines: Hunt for the Unicorn";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // userInputTextBox
            // 
            this.userInputTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.userInputTextBox.Location = new System.Drawing.Point(142, 638);
            this.userInputTextBox.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.userInputTextBox.Name = "userInputTextBox";
            this.userInputTextBox.Size = new System.Drawing.Size(333, 27);
            this.userInputTextBox.TabIndex = 2;
            this.userInputTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.userInputTextBox_KeyDown);
            // 
            // treasureBox
            // 
            this.treasureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.treasureBox.BackColor = System.Drawing.Color.Thistle;
            this.treasureBox.FormattingEnabled = true;
            this.treasureBox.HorizontalScrollbar = true;
            this.treasureBox.ItemHeight = 20;
            this.treasureBox.Location = new System.Drawing.Point(872, 465);
            this.treasureBox.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.treasureBox.Name = "treasureBox";
            this.treasureBox.Size = new System.Drawing.Size(129, 224);
            this.treasureBox.TabIndex = 4;
            // 
            // turnLabel
            // 
            this.turnLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.turnLabel.AutoSize = true;
            this.turnLabel.Location = new System.Drawing.Point(578, 641);
            this.turnLabel.Name = "turnLabel";
            this.turnLabel.Size = new System.Drawing.Size(50, 20);
            this.turnLabel.TabIndex = 6;
            this.turnLabel.Text = "label2";
            // 
            // inventoryLabel
            // 
            this.inventoryLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.inventoryLabel.AutoSize = true;
            this.inventoryLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.inventoryLabel.Location = new System.Drawing.Point(794, 423);
            this.inventoryLabel.Margin = new System.Windows.Forms.Padding(0);
            this.inventoryLabel.Name = "inventoryLabel";
            this.inventoryLabel.Size = new System.Drawing.Size(109, 29);
            this.inventoryLabel.TabIndex = 7;
            this.inventoryLabel.Text = "Inventory";
            // 
            // weaponBox
            // 
            this.weaponBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.weaponBox.BackColor = System.Drawing.Color.Thistle;
            this.weaponBox.FormattingEnabled = true;
            this.weaponBox.ItemHeight = 20;
            this.weaponBox.Location = new System.Drawing.Point(697, 465);
            this.weaponBox.Margin = new System.Windows.Forms.Padding(1);
            this.weaponBox.Name = "weaponBox";
            this.weaponBox.Size = new System.Drawing.Size(129, 224);
            this.weaponBox.TabIndex = 8;
            // 
            // inputLabel
            // 
            this.inputLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.inputLabel.AutoSize = true;
            this.inputLabel.Location = new System.Drawing.Point(28, 641);
            this.inputLabel.Margin = new System.Windows.Forms.Padding(1, 0, 1, 0);
            this.inputLabel.Name = "inputLabel";
            this.inputLabel.Size = new System.Drawing.Size(119, 20);
            this.inputLabel.TabIndex = 9;
            this.inputLabel.Text = "Command Input:";
            // 
            // moveLabel
            // 
            this.moveLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.moveLabel.AutoSize = true;
            this.moveLabel.Location = new System.Drawing.Point(484, 641);
            this.moveLabel.Margin = new System.Windows.Forms.Padding(1, 0, 1, 0);
            this.moveLabel.Name = "moveLabel";
            this.moveLabel.Size = new System.Drawing.Size(96, 20);
            this.moveLabel.TabIndex = 10;
            this.moveLabel.Text = "Moves Taken:";
            // 
            // mapBox
            // 
            this.mapBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.mapBox.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.mapBox.BackColor = System.Drawing.Color.Black;
            this.mapBox.Location = new System.Drawing.Point(643, 83);
            this.mapBox.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.mapBox.Name = "mapBox";
            this.mapBox.Size = new System.Drawing.Size(463, 322);
            this.mapBox.TabIndex = 0;
            this.mapBox.Paint += new System.Windows.Forms.PaintEventHandler(this.mapBox_Paint);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBox1.Location = new System.Drawing.Point(1036, 6);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(1);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(71, 73);
            this.pictureBox1.TabIndex = 12;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Location = new System.Drawing.Point(27, 6);
            this.pictureBox2.Margin = new System.Windows.Forms.Padding(1);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(71, 73);
            this.pictureBox2.TabIndex = 13;
            this.pictureBox2.TabStop = false;
            // 
            // consoleTextBox
            // 
            this.consoleTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.consoleTextBox.BackColor = System.Drawing.Color.Thistle;
            this.consoleTextBox.Location = new System.Drawing.Point(27, 83);
            this.consoleTextBox.Margin = new System.Windows.Forms.Padding(1);
            this.consoleTextBox.Multiline = true;
            this.consoleTextBox.Name = "consoleTextBox";
            this.consoleTextBox.ReadOnly = true;
            this.consoleTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.consoleTextBox.Size = new System.Drawing.Size(599, 554);
            this.consoleTextBox.TabIndex = 7;
            // 
            // GameView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.AutoValidate = System.Windows.Forms.AutoValidate.EnablePreventFocusChange;
            this.BackColor = System.Drawing.Color.Pink;
            this.ClientSize = new System.Drawing.Size(1112, 702);
            this.Controls.Add(this.consoleTextBox);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.mapBox);
            this.Controls.Add(this.moveLabel);
            this.Controls.Add(this.inputLabel);
            this.Controls.Add(this.weaponBox);
            this.Controls.Add(this.turnLabel);
            this.Controls.Add(this.treasureBox);
            this.Controls.Add(this.userInputTextBox);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.inventoryLabel);
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "GameView";
            this.Text = "Mines: Hunt for the Unicorn";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion
		
		private bool mapInFront;
		private Label label1;
		private TextBox userInputTextBox;
		private ListBox treasureBox;
		private Label turnLabel;
		private Label inventoryLabel;
        private ListBox weaponBox;
        private Label inputLabel;
        private Label moveLabel;
        private Panel mapBox;
        private PictureBox pictureBox1;
        private PictureBox pictureBox2;
        public TextBox consoleTextBox;
    }
}