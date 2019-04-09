namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class register : UserControl
    {
        protected Button btnregister;
        protected CompareValidator CompareValidator1;
        protected RadioButton rbfemale;
        protected RadioButton rbmale;
        protected RegularExpressionValidator RegularExpressionValidator1;
        protected RegularExpressionValidator RegularExpressionValidator2;
        protected RegularExpressionValidator RegularExpressionValidator4;
        protected RegularExpressionValidator RegularExpressionValidator5;
        protected RegularExpressionValidator RegularExpressionValidator6;
        protected RegularExpressionValidator RegularExpressionValidator7;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected RequiredFieldValidator RequiredFieldValidator2;
        protected RequiredFieldValidator RequiredFieldValidator3;
        protected RequiredFieldValidator Requiredfieldvalidator4;
        protected RequiredFieldValidator Requiredfieldvalidator5;
        protected TextBox tbcommend;
        protected TextBox tbemail;
        protected TextBox tbkey;
        protected TextBox tbreuserpwd;
        protected TextBox tbuserid;
        protected TextBox tbuserpwd;
        protected TextBox tbverifycode;

        private void btnregister_Click(object sender, EventArgs e)
        {
            string str = "";
            if ((this.tbkey.Text == this.tbuserid.Text) || (this.tbkey.Text == this.tbuserpwd.Text))
            {
                str = "二級密碼不可以與帳號或密碼相同";
            }
            else if ((this.tbverifycode.Text != base.Session["VerifyCode"].ToString()) && (base.Application["security.verifycode"].ToString() == "true"))
            {
                str = "驗證碼錯誤！";
            }
            else
            {
                system system = new system();
                string userid = system.ChkSql(this.tbuserid.Text.ToString());
                string userpwd = system.ChkSql(this.tbuserpwd.Text.ToString());
                string email = system.ChkSql(this.tbemail.Text.ToString());
                string key = system.ChkSql(this.tbkey.Text.ToString());
                string commend = system.ChkSql(this.tbcommend.Text.ToString());
                string sex = this.rbfemale.Checked ? "美女" : "帥哥";
                int webgold = int.Parse(base.Application["register.givegold"].ToString());
                string clientIP = system.GetClientIP();
                str = new WebLogic().register(userid, userpwd, key, sex, email, commend, clientIP, webgold);
            }
            base.Response.Write("<script language=javascript>alert('" + str + "')</script>");
        }

        private void InitializeComponent()
        {
            this.btnregister.Click += new EventHandler(this.btnregister_Click);
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
            if (base.Application["register.allow"].ToString() == "false")
            {
                base.Response.Write(base.Application["register.stopregtext"].ToString());
                base.Response.End();
            }
            if (base.Request.QueryString["u"] != null)
            {
                this.tbcommend.Text = base.Request.QueryString["u"].ToString().Trim();
            }
        }
    }
}
