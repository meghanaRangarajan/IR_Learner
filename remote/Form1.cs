using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace remote
{
    public partial class Form1 : Form
    {

		[DllImport("/home/pi/db_remote/remote/bin/Debug/IR_Decoder/decoder.so", EntryPoint = "code")]
        static extern UInt32 code();

		[DllImport("/home/pi/db_remote/remote/bin/Debug/IR_Decoder/decoder.so", EntryPoint = "create_insert")]
        static extern int create_insert(int key, UInt32 code);

        private UInt32[] key = new UInt32[42];

		private static int KEY_0 = 0;
        private static int KEY_1 = 1;
        private static int KEY_2 = 2;
        private static int KEY_3 = 3;
        private static int KEY_4 = 4;
        private static int KEY_5 = 5;
        private static int KEY_6 = 6;
        private static int KEY_7 = 7;
        private static int KEY_8 = 8;
        private static int KEY_9 = 9;
        private static int KEY_VOL_UP = 10;
        private static int KEY_VOL_DOWN = 11;
        private static int KEY_VOL_MUTE = 12;
        private static int KEY_CH_UP = 13;
        private static int KEY_CH_DOWN = 14;
        private static int KEY_POWER = 15;
        //private static int KEY_SELECT = 16;
        private static int KEY_BACK = 17;
        private static int KEY_EXIT = 18;
        private static int KEY_MENU = 19;
        private static int KEY_INFO = 20;
        private static int KEY_UP_BUTTON = 21;
        private static int KEY_DOWN_BUTTON = 22;
        private static int KEY_LEFT_BUTTON = 23;
        private static int KEY_RIGHT_BUTTON = 24;
        private static int KEY_OK = 25;
        private static int KEY_RED = 26;
        private static int KEY_GREEN = 27;
        private static int KEY_YELLOW = 28;
        private static int KEY_BLUE = 29;
        private static int KEY_PLAY = 30;
        private static int KEY_REWIND = 31;
        private static int KEY_FAST_FORWARD = 32;
        private static int KEY_RECORD = 33;
        private static int KEY_PAUSE = 34;
        private static int KEY_STOP = 35;
        private static int KEY_SOURCE = 36;
        private static int KEY_OPTIONS = 37;
        private static int KEY_LANGUAGE = 38;
        private static int KEY_GUIDE = 39;
        private static int KEY_PREVIOUS = 40;
        private static int KEY_NEXT = 41;

		private static string[] keys = new string[] { "KEY_0", "KEY_1", "KEY_2", "KEY_3", "KEY_4", "KEY_5", "KEY_6", "KEY_7", "KEY_8", "KEY_9", "KEY_VOL_UP ", "KEY_VOL_DOWN ", "KEY_VOL_MUTE ", "KEY_CH_UP ", "KEY_CH_DOWN ", "KEY_POWER ", "KEY_SELECT ", "KEY_BACK ", "KEY_EXIT ", "KEY_MENU ", "KEY_INFO ", "KEY_UP_BUTTON ", "KEY_DOWN_BUTTON ", "KEY_LEFT_BUTTON ", "KEY_RIGHT_BUTTON ", "KEY_OK ", "KEY_RED ", "KEY_GREEN ", "KEY_YELLOW ", "KEY_BLUE ", "KEY_PLAY ", "KEY_REWIND ", "KEY_FAST_FORWARD ", "KEY_RECORD ", "KEY_PAUSE ", "KEY_STOP ", "KEY_SOURCE ", "KEY_OPTIONS ", "KEY_LANGUAGE ", "KEY_GUIDE ", "KEY_PREVIOUS ", "KEY_NEXT " };
        


        private void assign(int key_code)
        {
            key[key_code] = code();
            create_insert(key_code, key[key_code]);
			MessageBox.Show(("KeyCode for " + keys[key_code] + " : 0x" + (key[key_code].ToString("X"))), ("KEY : " + key_code));
        }

        public Form1()
        {
            InitializeComponent();
        }

        private void two_Click(object sender, EventArgs e)
        {
			assign(KEY_2);

            Console.WriteLine("KEY : 2");
        }

        private void previous_Click(object sender, EventArgs e)
        {
			assign(KEY_PREVIOUS);
            Console.WriteLine("KEY : PREVIOUS");
        }

        private void green_Click(object sender, EventArgs e)
        {
			assign(KEY_GREEN);
            Console.WriteLine("KEY : GREEN");
        }

        private void Form1_Load(object sender, EventArgs e)
        {
           
        }

        private void power_Click(object sender, EventArgs e)
        {
			assign(KEY_POWER);
            Console.WriteLine("KEY : POWER");
        }

        private void info_Click(object sender, EventArgs e)
        {
			assign(KEY_INFO);
            Console.WriteLine("KEY : INFO");
        }

        private void mute_Click(object sender, EventArgs e)
        {
			assign(KEY_VOL_MUTE);
            Console.WriteLine("KEY : MUTE");
        }

        private void one_Click(object sender, EventArgs e)
        {
			assign(KEY_1);
            Console.WriteLine("KEY : 1");
        }

        private void three_Click(object sender, EventArgs e)
        {
			assign(KEY_3);
            Console.WriteLine("KEY : 3");
        }

        private void four_Click(object sender, EventArgs e)
        {
			assign(KEY_4);
            Console.WriteLine("KEY : 4");
        }

        private void five_Click(object sender, EventArgs e)
        {
			assign(KEY_5);
            Console.WriteLine("KEY : 5");
        }

        private void six_Click(object sender, EventArgs e)
        {
			assign(KEY_6);
            Console.WriteLine("KEY : 6");
        }
        
        private void seven_Click(object sender, EventArgs e)
        {
			assign(KEY_7);
            Console.WriteLine("KEY : 7");
        }

        private void eight_Click(object sender, EventArgs e)
        {
			assign(KEY_8);
            Console.WriteLine("KEY : 8");
        }

        private void nine_Click(object sender, EventArgs e)
        {
			assign(KEY_9);
            Console.WriteLine("KEY : 9");
        }

        private void zero_Click(object sender, EventArgs e)
        {
			assign(KEY_0);
            Console.WriteLine("KEY : 0");
        }

        private void vol_up_Click(object sender, EventArgs e)
        {
			assign(KEY_VOL_UP);
            Console.WriteLine("KEY : VOLUME UP");
        }

        private void vol_down_Click(object sender, EventArgs e)
        {
			assign(KEY_VOL_DOWN);
            Console.WriteLine("KEY : VOLUME DOWN");
        }

        private void ch_up_Click(object sender, EventArgs e)
        {
			assign(KEY_CH_UP);
            Console.WriteLine("KEY : CHANNEL UP");
        }

        private void ch_down_Click(object sender, EventArgs e)
        {
			assign(KEY_CH_DOWN);
            Console.WriteLine("KEY : CHANNEL DOWN");
        }

        private void back_Click(object sender, EventArgs e)
        {
			assign(KEY_BACK);
            Console.WriteLine("KEY : BACK");
        }

        private void exit_Click(object sender, EventArgs e)
        {
			assign(KEY_EXIT);
            Console.WriteLine("KEY : EXIT");
        }

        private void ok_Click(object sender, EventArgs e)
        {
			assign(KEY_OK);
            Console.WriteLine("KEY : OK");
        }

        private void up_Click(object sender, EventArgs e)
        {
			assign(KEY_UP_BUTTON);
            Console.WriteLine("KEY : UP BUTTON");
        }

        private void down_Click(object sender, EventArgs e)
        {
			assign(KEY_DOWN_BUTTON);
            Console.WriteLine("KEY : DOWN BUTTON");
        }

        private void left_Click(object sender, EventArgs e)
        {
			assign(KEY_LEFT_BUTTON);
            Console.WriteLine("KEY : LEFT BUTTON");
        }

        private void right_Click(object sender, EventArgs e)
        {
			assign(KEY_RIGHT_BUTTON);
            Console.WriteLine("KEY : RIGHT BUTTON");
        }

        private void next_Click(object sender, EventArgs e)
        {
			assign(KEY_NEXT);
            Console.WriteLine("KEY : NEXT");
        }

        private void language_Click(object sender, EventArgs e)
        {
			assign(KEY_LANGUAGE);
            Console.WriteLine("KEY : LANGUAGE");
        }

        private void guide_Click(object sender, EventArgs e)
        {
			assign(KEY_GUIDE);
            Console.WriteLine("KEY : GUIDE");
        }

        private void red_Click(object sender, EventArgs e)
        {
			assign(KEY_RED);
            Console.WriteLine("KEY : RED");
        }

        private void yellow_Click(object sender, EventArgs e)
        {
			assign(KEY_YELLOW);
            Console.WriteLine("KEY : YELLOW");
        }

        private void blue_Click(object sender, EventArgs e)
        {
			assign(KEY_BLUE);
            Console.WriteLine("KEY : BLUE");
        }

        private void rewind_Click(object sender, EventArgs e)
        {
			assign(KEY_REWIND);
            Console.WriteLine("KEY : REWIND");
        }

        private void fast_forward_Click(object sender, EventArgs e)
        {
			assign(KEY_FAST_FORWARD);
            Console.WriteLine("KEY : FAST FORWARD");
        }

        private void play_Click(object sender, EventArgs e)
        {
			assign(KEY_PLAY);
            Console.WriteLine("KEY : PLAY");
        }

        private void pause_Click(object sender, EventArgs e)
        {
			assign(KEY_PAUSE);
            Console.WriteLine("KEY : PAUSE");
        }

        private void stop_Click(object sender, EventArgs e)
        {
			assign(KEY_STOP);
            Console.WriteLine("KEY : STOP");
        }

        private void record_Click(object sender, EventArgs e)
        {
			assign(KEY_RECORD);
            Console.WriteLine("KEY : RECORD");
        }

        private void menu_Click(object sender, EventArgs e)
        {
			assign(KEY_MENU);
            Console.WriteLine("KEY : MENU");
        }

        private void source_Click(object sender, EventArgs e)
        {
			assign(KEY_SOURCE);
            Console.WriteLine("KEY : SOURCE");
        }

        private void options_Click(object sender, EventArgs e)
        {
			assign(KEY_OPTIONS);
            Console.WriteLine("KEY : OPTIONS");
        }

		private void close_form_Click(object sender, EventArgs e)
        {
            //stop
            Console.WriteLine("____________________________DONE____________________________");
            Application.Exit();
        }


    }
}
