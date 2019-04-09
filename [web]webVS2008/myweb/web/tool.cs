namespace web
{
    using System;
    using System.Text;
    using System.Web.Security;
    using System.Web.UI;
    using System.Web.UI.WebControls;

    public class tool : Page
    {
        protected Button btnbase64de;
        protected Button btnchecklen;
        protected Button btnClear;
        protected Button btnDecrypt;
        protected Button btnEncrypt;
        protected Button btnfullmd5;
        protected Button btngbtobig;
        protected Button btnmd5;
        protected Button btnsha1;
        private static Encoding ens = Encoding.Default;
        protected Label Label1;
        protected TextBox strKey;
        protected TextBox strText;
        protected system sys = new system();

        private void btnbase64de_Click(object sender, EventArgs e)
        {
            this.strText.Text = ens.GetString(Convert.FromBase64String(this.strText.Text.ToString()));
        }

        private void btnchecklen_Click(object sender, EventArgs e)
        {
            this.strText.Text = new system().getStrLen(this.strText.Text.ToString()).ToString();
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            this.strText.Text = this.Page.Request.UserHostAddress.ToString();
        }

        private void btnDecrypt_Click(object sender, EventArgs e)
        {
            this.strText.Text = this.sys.Decrypt(this.strText.Text, this.strKey.Text);
        }

        private void btnEncrypt_Click(object sender, EventArgs e)
        {
            this.strText.Text = this.sys.Encrypt(this.strText.Text, this.strKey.Text);
        }

        private void btnfullmd5_Click(object sender, EventArgs e)
        {
            this.strText.Text = FormsAuthentication.HashPasswordForStoringInConfigFile(this.strText.Text.ToString(), "MD5");
        }

        private void btngbtobig_Click(object sender, EventArgs e)
        {
            this.strText.Text.ToString();
            this.strText.Text = new system().ConvertToBig5(new system().ConvertToBig5(this.strText.Text.ToString(), 0x3a8), 950);
        }

        private void btnmd5_Click(object sender, EventArgs e)
        {
            this.strText.Text = FormsAuthentication.HashPasswordForStoringInConfigFile(this.strText.Text.ToString(), "MD5").Substring(0, 0x12);
        }

        private void btnsha1_Click(object sender, EventArgs e)
        {
            this.strText.Text = FormsAuthentication.HashPasswordForStoringInConfigFile(this.strText.Text.ToString(), "SHA1").ToString().ToLower();
        }

        private void InitializeComponent()
        {
            this.btnbase64de.Click += new EventHandler(this.btnbase64de_Click);
            this.btnchecklen.Click += new EventHandler(this.btnchecklen_Click);
            this.btngbtobig.Click += new EventHandler(this.btngbtobig_Click);
            this.btnfullmd5.Click += new EventHandler(this.btnfullmd5_Click);
            this.btnmd5.Click += new EventHandler(this.btnmd5_Click);
            this.btnClear.Click += new EventHandler(this.btnClear_Click);
            this.btnDecrypt.Click += new EventHandler(this.btnDecrypt_Click);
            this.btnEncrypt.Click += new EventHandler(this.btnEncrypt_Click);
            this.btnsha1.Click += new EventHandler(this.btnsha1_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            object obj1 = this.Session["admin_id"];
        }
    }
}
