namespace web
{
    using System;
    using System.Drawing;
    using System.Drawing.Imaging;
    using System.IO;
    using System.Web;

    public class verifycode
    {
        private Color backgroundColor = Color.White;
        private bool chaos = false;
        private Color chaosColor = Color.LightGray;
        private string codeSerial = "0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z";
        private Color[] colors = new Color[] { Color.Black, Color.Red, Color.DarkBlue, Color.Green, Color.Orange, Color.Brown, Color.DarkCyan, Color.Purple };
        private string[] fonts = new string[] { "Arial", "Georgia" };
        private int fontSize = 12;
        private int length = 4;
        private int padding = 2;

        public Bitmap CreateImageCode(string code)
        {
            int fontSize = this.FontSize;
            int num2 = fontSize + this.Padding;
            int width = ((code.Length * num2) + 4) + (this.Padding * 2);
            int height = (fontSize * 2) + this.Padding;
            Bitmap image = new Bitmap(width, height);
            Graphics graphics = Graphics.FromImage(image);
            graphics.Clear(this.BackgroundColor);
            Random random = new Random();
            if (this.Chaos)
            {
                Pen pen = new Pen(this.ChaosColor, 0f);
                int num5 = this.Length * 10;
                for (int j = 0; j < num5; j++)
                {
                    int x = random.Next(image.Width);
                    int y = random.Next(image.Height);
                    graphics.DrawRectangle(pen, x, y, 1, 1);
                }
            }
            int num9 = 0;
            int num10 = 0;
            int num11 = 1;
            int num12 = 1;
            int num13 = (height - this.FontSize) - (this.Padding * 2);
            int num14 = num13 / 4;
            num11 = num14;
            num12 = num14 * 2;
            for (int i = 0; i < code.Length; i++)
            {
                int index = random.Next(this.Colors.Length - 1);
                int num16 = random.Next(this.Fonts.Length - 1);
                Font font = new Font(this.Fonts[num16], (float) fontSize, FontStyle.Bold);
                Brush brush = new SolidBrush(this.Colors[index]);
                if ((i % 2) == 1)
                {
                    num10 = num12;
                }
                else
                {
                    num10 = num11;
                }
                num9 = i * num2;
                graphics.DrawString(code.Substring(i, 1), font, brush, (float) num9, (float) num10);
            }
            graphics.DrawRectangle(new Pen(Color.Black, 0f), 0, 0, image.Width - 1, image.Height - 1);
            graphics.Dispose();
            return image;
        }

        public void CreateImageOnPage(string code, HttpContext context)
        {
            MemoryStream stream = new MemoryStream();
            Bitmap bitmap = this.CreateImageCode(code);
            bitmap.Save(stream, ImageFormat.Jpeg);
            context.Response.ClearContent();
            context.Response.ContentType = "image/Jpeg";
            context.Response.BinaryWrite(stream.GetBuffer());
            stream.Close();
            stream = null;
            bitmap.Dispose();
            bitmap = null;
        }

        public string CreateVerifyCode()
        {
            return this.CreateVerifyCode(0);
        }

        public string CreateVerifyCode(int codeLen)
        {
            if (codeLen == 0)
            {
                codeLen = this.Length;
            }
            string[] strArray = this.CodeSerial.Split(new char[] { ',' });
            string str = "";
            int index = -1;
            Random random = new Random((int) DateTime.Now.Ticks);
            for (int i = 0; i < codeLen; i++)
            {
                index = random.Next(0, strArray.Length - 1);
                str = str + strArray[index];
            }
            return str;
        }

        public Color BackgroundColor
        {
            get
            {
                return this.backgroundColor;
            }
            set
            {
                this.backgroundColor = value;
            }
        }

        public bool Chaos
        {
            get
            {
                return this.chaos;
            }
            set
            {
                this.chaos = value;
            }
        }

        public Color ChaosColor
        {
            get
            {
                return this.chaosColor;
            }
            set
            {
                this.chaosColor = value;
            }
        }

        public string CodeSerial
        {
            get
            {
                return this.codeSerial;
            }
            set
            {
                this.codeSerial = value;
            }
        }

        public Color[] Colors
        {
            get
            {
                return this.colors;
            }
            set
            {
                this.colors = value;
            }
        }

        public string[] Fonts
        {
            get
            {
                return this.fonts;
            }
            set
            {
                this.fonts = value;
            }
        }

        public int FontSize
        {
            get
            {
                return this.fontSize;
            }
            set
            {
                this.fontSize = value;
            }
        }

        public int Length
        {
            get
            {
                return this.length;
            }
            set
            {
                this.length = value;
            }
        }

        public int Padding
        {
            get
            {
                return this.padding;
            }
            set
            {
                this.padding = value;
            }
        }
    }
}
