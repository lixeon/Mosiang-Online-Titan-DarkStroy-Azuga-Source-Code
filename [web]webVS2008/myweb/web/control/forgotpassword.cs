namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class forgotpassword : UserControl
    {
        protected Button btnreset;
        protected RegularExpressionValidator RegularExpressionValidator1;
        protected RegularExpressionValidator RegularExpressionValidator2;
        protected RegularExpressionValidator RegularExpressionValidator3;
        protected RegularExpressionValidator RegularExpressionValidator6;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected RequiredFieldValidator RequiredFieldValidator2;
        protected RequiredFieldValidator Requiredfieldvalidator5;
        protected RequiredFieldValidator Requiredfieldvalidator6;
        protected TextBox tbemail;
        protected TextBox tbkey;
        protected TextBox tbuserid;
        protected TextBox tbverifycode;

        private void btnreset_Click(object sender, EventArgs e)
        {
            string str = "";
            if (this.tbverifycode.Text != base.Session["VerifyCode"].ToString())
            {
                str = "驗證碼錯誤！";
            }
            else
            {
                system system = new system();
                string userid = system.ChkSql(this.tbuserid.Text.ToString());
                string key = system.ChkSql(this.tbkey.Text.ToString());
                string email = system.ChkSql(this.tbemail.Text.ToString());
                str = new WebLogic().forgetpassword(userid, key, email);
            }
            base.Response.Write("<script language=javascript>alert('" + str + "')</script>");
        }

        private void InitializeComponent()
        {
            this.btnreset.Click += new EventHandler(this.btnreset_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new system().loadConfig(0);
        }
    }
}
