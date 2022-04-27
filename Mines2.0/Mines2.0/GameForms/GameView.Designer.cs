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
			this.mapBox = new System.Windows.Forms.Panel();
			this.mapButton = new System.Windows.Forms.Button();
			this.turnBox = new System.Windows.Forms.PictureBox();
			this.label1 = new System.Windows.Forms.Label();
			this.userInputTextBox = new System.Windows.Forms.TextBox();
			this.inventoryBox = new System.Windows.Forms.ListBox();
			this.turnLabel = new System.Windows.Forms.Label();
			this.inventoryLabel = new System.Windows.Forms.Label();
			this.consoleTextBox = new System.Windows.Forms.TextBox();
			this.consoleBox = new System.Windows.Forms.Panel();
			((System.ComponentModel.ISupportInitialize)(this.turnBox)).BeginInit();
			this.consoleBox.SuspendLayout();
			this.SuspendLayout();
			// 
			// mapBox
			// 
			this.mapBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.mapBox.BackColor = System.Drawing.SystemColors.Desktop;
			this.mapBox.Location = new System.Drawing.Point(34, 77);
			this.mapBox.Name = "mapBox";
			this.mapBox.Size = new System.Drawing.Size(552, 326);
			this.mapBox.TabIndex = 0;
			this.mapBox.Paint += new System.Windows.Forms.PaintEventHandler(this.mapBox_Paint);
			// 
			// mapButton
			// 
			this.mapButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.mapButton.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.mapButton.BackColor = System.Drawing.SystemColors.Window;
			this.mapButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
			this.mapButton.ForeColor = System.Drawing.SystemColors.InfoText;
			this.mapButton.Location = new System.Drawing.Point(546, 431);
			this.mapButton.Name = "mapButton";
			this.mapButton.Size = new System.Drawing.Size(40, 26);
			this.mapButton.TabIndex = 1;
			this.mapButton.Text = "Map";
			this.mapButton.UseVisualStyleBackColor = false;
			this.mapButton.Click += new System.EventHandler(this.mapButton_Click);
			// 
			// turnBox
			// 
			this.turnBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.turnBox.BackColor = System.Drawing.SystemColors.Window;
			this.turnBox.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.turnBox.Location = new System.Drawing.Point(615, 77);
			this.turnBox.Name = "turnBox";
			this.turnBox.Size = new System.Drawing.Size(146, 105);
			this.turnBox.TabIndex = 3;
			this.turnBox.TabStop = false;
			// 
			// label1
			// 
			this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.label1.BackColor = System.Drawing.SystemColors.Control;
			this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 36F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point);
			this.label1.Location = new System.Drawing.Point(66, 9);
			this.label1.MaximumSize = new System.Drawing.Size(10000, 100000);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(660, 57);
			this.label1.TabIndex = 5;
			this.label1.Text = "Mines: Hunt for the Unicorn";
			this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// userInputTextBox
			// 
			this.userInputTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.userInputTextBox.Location = new System.Drawing.Point(34, 434);
			this.userInputTextBox.Name = "userInputTextBox";
			this.userInputTextBox.Size = new System.Drawing.Size(506, 23);
			this.userInputTextBox.TabIndex = 2;
			this.userInputTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.userInputTextBox_KeyDown);
			// 
			// inventoryBox
			// 
			this.inventoryBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.inventoryBox.FormattingEnabled = true;
			this.inventoryBox.HorizontalScrollbar = true;
			this.inventoryBox.ItemHeight = 15;
			this.inventoryBox.Location = new System.Drawing.Point(615, 228);
			this.inventoryBox.Name = "inventoryBox";
			this.inventoryBox.Size = new System.Drawing.Size(146, 229);
			this.inventoryBox.TabIndex = 4;
			// 
			// turnLabel
			// 
			this.turnLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.turnLabel.AutoSize = true;
			this.turnLabel.Location = new System.Drawing.Point(668, 123);
			this.turnLabel.Name = "turnLabel";
			this.turnLabel.Size = new System.Drawing.Size(38, 15);
			this.turnLabel.TabIndex = 6;
			this.turnLabel.Text = "label2";
			// 
			// inventoryLabel
			// 
			this.inventoryLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.inventoryLabel.AutoSize = true;
			this.inventoryLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
			this.inventoryLabel.Location = new System.Drawing.Point(647, 196);
			this.inventoryLabel.Margin = new System.Windows.Forms.Padding(0);
			this.inventoryLabel.Name = "inventoryLabel";
			this.inventoryLabel.Size = new System.Drawing.Size(86, 24);
			this.inventoryLabel.TabIndex = 7;
			this.inventoryLabel.Text = "Inventory";
			// 
			// consoleTextBox
			// 
			this.consoleTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.consoleTextBox.BackColor = System.Drawing.SystemColors.Window;
			this.consoleTextBox.Location = new System.Drawing.Point(0, 0);
			this.consoleTextBox.Multiline = true;
			this.consoleTextBox.Name = "consoleTextBox";
			this.consoleTextBox.ReadOnly = true;
			this.consoleTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.consoleTextBox.Size = new System.Drawing.Size(552, 326);
			this.consoleTextBox.TabIndex = 7;
			// 
			// consoleBox
			// 
			this.consoleBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.consoleBox.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.consoleBox.BackColor = System.Drawing.SystemColors.Desktop;
			this.consoleBox.Controls.Add(this.consoleTextBox);
			this.consoleBox.ForeColor = System.Drawing.SystemColors.ControlText;
			this.consoleBox.Location = new System.Drawing.Point(34, 77);
			this.consoleBox.Name = "consoleBox";
			this.consoleBox.Size = new System.Drawing.Size(552, 326);
			this.consoleBox.TabIndex = 0;
			// 
			// GameView
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.ClientSize = new System.Drawing.Size(809, 483);
			this.Controls.Add(this.inventoryLabel);
			this.Controls.Add(this.turnLabel);
			this.Controls.Add(this.inventoryBox);
			this.Controls.Add(this.userInputTextBox);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.consoleBox);
			this.Controls.Add(this.turnBox);
			this.Controls.Add(this.mapButton);
			this.Controls.Add(this.mapBox);
			this.Name = "GameView";
			this.Text = "Mines: Hunt for the Unicorn";
			this.Resize += new System.EventHandler(this.GameView_Resize);
			((System.ComponentModel.ISupportInitialize)(this.turnBox)).EndInit();
			this.consoleBox.ResumeLayout(false);
			this.consoleBox.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion
		
		private bool mapInFront;
		private Panel mapBox;
		private Button mapButton;
		private PictureBox turnBox;
		private Label label1;
		private TextBox userInputTextBox;
		private ListBox inventoryBox;
		private Label turnLabel;
		private Label inventoryLabel;
		public TextBox consoleTextBox;
		private Panel consoleBox;
	}
}