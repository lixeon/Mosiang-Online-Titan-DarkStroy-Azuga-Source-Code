namespace web
{
    using System;
    using System.Drawing;
    using System.Drawing.Imaging;
    using System.IO;
    using System.Web;
    using System.Web.UI;
    using System.Web.UI.WebControls;

   public class VerifyCodePage : Page
{
    // Fields
    private Color backgroundColor = Color.White;
    private bool chaos = true;
    private Color chaosColor = Color.Pink;
    private string codeSerial = "1,2,3,4,6,7,8,9";
    private Color[] colors = new Color[] { Color.Black, Color.Red, Color.DarkBlue, Color.Green, Color.Orange, Color.Brown, Color.DarkCyan, Color.Purple };
    private string[] fonts = new string[] { "Arial", "Georgia" };
    private int fontSize = 12;
    private int length = 4;
    private int padding = 1;

    // Methods
    private void InitializeComponent()
    {
        base.Load += new EventHandler(this.Page_Load);
    }

    protected override void OnInit(EventArgs e)
    {
        this.InitializeComponent();
        base.OnInit(e);
    }

    protected void Page_Load(object sender, EventArgs e)
    {
        verifycode verifycode = new verifycode();
        verifycode.Length = this.length;
        verifycode.FontSize = this.fontSize;
        verifycode.Chaos = this.chaos;
        verifycode.BackgroundColor = this.backgroundColor;
        verifycode.ChaosColor = this.chaosColor;
        verifycode.CodeSerial = this.codeSerial;
        verifycode.Colors = this.colors;
        verifycode.Fonts = this.fonts;
        verifycode.Padding = this.padding;
        string code = verifycode.CreateVerifyCode();
        verifycode.CreateImageOnPage(code, this.Context);
        this.Session.Add("VerifyCode", code);
    }
}
 

}
